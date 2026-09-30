"""
@file gui_app.py
@brief HaptFleltApp class definition
@author Franklin
"""

import queue
import threading
import tkinter as tk
from datetime import datetime
from tkinter import ttk, messagebox

import utils
from mappings import (
    Mappings, ACTION_LOG, ACTION_KEY,
    TOUCH_COMMANDS, MOVEMENT_COMMANDS,
    FEEDBACK_OPTIONS, FEEDBACK_LABEL_TO_COMMAND, FEEDBACK_COMMAND_TO_LABEL,
)
from connection_worker import ConnectionWorker
from keyboard_picker import ask_key

try:
    import pystray
    from PIL import Image, ImageDraw
    TRAY_AVAILABLE = True  # `True` if the system tray is available.
except Exception:
    # Without a system tray backend, closing the window exits the app.
    TRAY_AVAILABLE = False


def _create_tray_image():
    """
    @brief Draws the system tray icon (a green dot).
    @return Icon image.
    """
    image = Image.new("RGB", (64, 64), color=(30, 30, 30))
    draw = ImageDraw.Draw(image)
    draw.ellipse((8, 8, 56, 56), fill=(46, 204, 113))
    return image


class HaptFleltApp(tk.Tk):
    """
    @brief Desktop app to connect to the belt and configure the command mappings.
    """

    def __init__(self):
        """
        @brief Default constructor.
        """
        super().__init__()

        self.title("HaptFlelt Desktop")
        self.geometry("640x480")
        self.minsize(600, 420)

        self.mappings = Mappings()  # PC to belt and belt to PC mappings.
        self.event_queue = queue.Queue()  # Events sent by the ConnectionWorker.
        self.worker = ConnectionWorker(self.mappings, self.event_queue)  # Connection to the belt.

        self.tray_icon = None  # System tray icon.

        self._build_ui()
        self._refresh_ports()

        self.protocol("WM_DELETE_WINDOW", self._on_window_close)
        self.after(100, self._poll_events)

        if TRAY_AVAILABLE:
            self._start_tray_icon()
        else:
            print("Note: system tray isn't available on this platform - closing the window will exit the app.")

    def _build_ui(self):
        """
        @brief Builds the window tabs and the bottom bar.
        """
        notebook = ttk.Notebook(self)
        notebook.pack(fill="both", expand=True, padx=8, pady=(8, 0))

        connection_tab = ttk.Frame(notebook)
        feedback_tab = ttk.Frame(notebook)
        received_tab = ttk.Frame(notebook)

        notebook.add(connection_tab, text="Connection")
        notebook.add(feedback_tab, text="Feedback háptico")
        notebook.add(received_tab, text="Comandos de Toque e Movimento")

        self._build_connection_tab(connection_tab)
        self._build_feedback_tab(feedback_tab)
        self._build_received_tab(received_tab)

        bottom_bar = ttk.Frame(self)
        bottom_bar.pack(fill="x", padx=8, pady=8)
        ttk.Button(bottom_bar, text="Save mappings", command=self._save_mappings).pack(side="right")

    def _build_connection_tab(self, parent):
        """
        @brief Builds the connection tab.
        @param parent Parent widget.
        """
        port_row = ttk.Frame(parent)
        port_row.pack(fill="x", padx=8, pady=(8, 4))

        ttk.Label(port_row, text="Port:").pack(side="left")

        self.port_var = tk.StringVar()
        self.port_combo = ttk.Combobox(port_row, textvariable=self.port_var, state="readonly", width=14)
        self.port_combo.pack(side="left", padx=(4, 8))

        ttk.Button(port_row, text="Scan", command=self._refresh_ports).pack(side="left")

        actions_row = ttk.Frame(parent)
        actions_row.pack(fill="x", padx=8, pady=(0, 4))

        ttk.Button(actions_row, text="Test connection", command=self._on_test_connection).pack(side="left")

        self.connect_button = ttk.Button(actions_row, text="Connect", command=self._on_connect_toggle)
        self.connect_button.pack(side="left", padx=4)

        status_frame = ttk.Frame(parent)
        status_frame.pack(fill="x", padx=8)

        self.status_canvas = tk.Canvas(status_frame, width=14, height=14, highlightthickness=0)
        self.status_dot = self.status_canvas.create_oval(2, 2, 12, 12, fill="red", outline="")
        self.status_canvas.pack(side="left")

        self.status_label = ttk.Label(status_frame, text="Disconnected")
        self.status_label.pack(side="left", padx=(6, 0))

        last_frame = ttk.Frame(parent)
        last_frame.pack(fill="x", padx=8, pady=(8, 0))

        self.last_sent_label = ttk.Label(last_frame, text="Last sent: -")
        self.last_sent_label.pack(anchor="w")

        self.last_received_label = ttk.Label(last_frame, text="Last received: -")
        self.last_received_label.pack(anchor="w")

        ttk.Label(parent, text="Log:").pack(anchor="w", padx=8, pady=(8, 0))

        log_frame = ttk.Frame(parent)
        log_frame.pack(fill="both", expand=True, padx=8, pady=(0, 8))

        self.log_text = tk.Text(log_frame, state="disabled", wrap="word", height=6)
        log_scroll = ttk.Scrollbar(log_frame, command=self.log_text.yview)
        self.log_text.configure(yscrollcommand=log_scroll.set)

        self.log_text.pack(side="left", fill="both", expand=True)
        log_scroll.pack(side="right", fill="y")

    def _refresh_ports(self):
        """
        @brief Scans for the belt's candidate ports.
        """
        ports = utils.find_esp32_ports()
        self.port_combo["values"] = ports
        if ports:
            self.port_var.set(ports[0])
        self._log(f"Found ports: {', '.join(ports) if ports else '(none)'}")

    def _on_test_connection(self):
        """
        @brief Tests the connection to the selected port in a helper thread.
        """
        port = self.port_var.get()
        if not port:
            messagebox.showwarning("No port", "Scan for a port first.")
            return

        def run_test():
            ok = self.worker.test_connection(port)
            self.event_queue.put(("log", f"Test on {port}: {'OK' if ok else 'no response'}"))

        threading.Thread(target=run_test, daemon=True).start()

    def _on_connect_toggle(self):
        """
        @brief Connects to or disconnects from the belt.
        """
        if self.worker.is_running():
            self.worker.stop()
            self.connect_button.config(text="Connect")
        else:
            self.worker.start()
            self.connect_button.config(text="Disconnect")

    def _set_status(self, connected):
        """
        @brief Updates the connection status indicator.
        @param connected `True` if connected, `False` otherwise.
        """
        color = "#2ecc71" if connected else "#e74c3c"
        self.status_canvas.itemconfig(self.status_dot, fill=color)
        self.status_label.config(text="Connected" if connected else "Disconnected")

    def _log(self, text):
        """
        @brief Appends a timestamped line to the log.
        @param text Line to be logged.
        """
        timestamp = datetime.now().strftime("%H:%M:%S")
        self.log_text.configure(state="normal")
        self.log_text.insert("end", f"[{timestamp}] {text}\n")
        self.log_text.see("end")
        self.log_text.configure(state="disabled")

    def _build_feedback_tab(self, parent):
        """
        @brief Builds the haptic feedback tab (PC to belt).
        @param parent Parent widget.
        """
        ttk.Label(
            parent,
            text="Qual vibração é disparada no cinto quando uma tecla do PC é pressionada.",
        ).pack(anchor="w", padx=8, pady=(8, 4))

        tree_frame = ttk.Frame(parent)
        tree_frame.pack(fill="both", expand=True, padx=8)

        self.pc_to_belt_tree = ttk.Treeview(
            tree_frame, columns=("key", "command"), show="headings", height=8
        )
        self.pc_to_belt_tree.heading("key", text="Tecla do PC")
        self.pc_to_belt_tree.heading("command", text="Vibração no cinto")
        self.pc_to_belt_tree.pack(side="left", fill="both", expand=True)

        tree_scroll = ttk.Scrollbar(tree_frame, command=self.pc_to_belt_tree.yview)
        self.pc_to_belt_tree.configure(yscrollcommand=tree_scroll.set)
        tree_scroll.pack(side="right", fill="y")

        form = ttk.Frame(parent)
        form.pack(fill="x", padx=8, pady=8)

        key_row = ttk.Frame(form)
        key_row.pack(fill="x", pady=(0, 4))

        ttk.Label(key_row, text="Tecla do PC:").pack(side="left")

        self.pc_key_var = tk.StringVar()
        self.pc_key_display = ttk.Entry(key_row, textvariable=self.pc_key_var, width=14, state="readonly")
        self.pc_key_display.pack(side="left", padx=(4, 4))

        ttk.Button(key_row, text="Escolher tecla...", command=self._on_choose_key).pack(side="left")

        vibration_row = ttk.Frame(form)
        vibration_row.pack(fill="x")

        ttk.Label(vibration_row, text="Vibração:").pack(side="left")

        self.feedback_label_var = tk.StringVar()
        feedback_combo = ttk.Combobox(
            vibration_row, textvariable=self.feedback_label_var, state="readonly", width=18,
            values=[label for label, _ in FEEDBACK_OPTIONS],
        )
        feedback_combo.pack(side="left", padx=(4, 12))

        ttk.Button(vibration_row, text="Adicionar / atualizar", command=self._add_pc_to_belt_row).pack(side="left", padx=4)
        ttk.Button(vibration_row, text="Remover selecionado", command=self._remove_pc_to_belt_row).pack(side="left", padx=4)

        self._refresh_pc_to_belt_tree()

    def _on_choose_key(self):
        """
        @brief Opens the keyboard picker to choose the PC key.
        """
        key = ask_key(self)
        if key:
            self.pc_key_var.set(key)

    def _refresh_pc_to_belt_tree(self):
        """
        @brief Updates the PC to belt mappings table.
        """
        self.pc_to_belt_tree.delete(*self.pc_to_belt_tree.get_children())
        for key, command in self.mappings.pc_to_belt.items():
            label = FEEDBACK_COMMAND_TO_LABEL.get(command, command)
            self.pc_to_belt_tree.insert("", "end", iid=key, values=(key, label))

    def _add_pc_to_belt_row(self):
        """
        @brief Adds or updates a PC to belt mapping.
        """
        key = self.pc_key_var.get().strip()
        label = self.feedback_label_var.get().strip()

        if not key or not label:
            messagebox.showwarning("Dados incompletos", "Escolha a tecla do PC e a vibração desejada.")
            return

        command = FEEDBACK_LABEL_TO_COMMAND.get(label)
        self.mappings.set_pc_to_belt(key, command)
        self._refresh_pc_to_belt_tree()
        self.pc_key_var.set("")
        self.feedback_label_var.set("")

    def _remove_pc_to_belt_row(self):
        """
        @brief Removes the selected PC to belt mappings.
        """
        selected = self.pc_to_belt_tree.selection()
        for key in selected:
            self.mappings.remove_pc_to_belt(key)
        self._refresh_pc_to_belt_tree()

    def _build_received_tab(self, parent):
        """
        @brief Builds the touch and movement commands tab (belt to PC).
        @param parent Parent widget.
        """
        ttk.Label(
            parent,
            text="O que acontece no PC quando o cinto envia cada comando.",
        ).pack(anchor="w", padx=8, pady=(8, 4))

        container = ttk.Frame(parent)
        container.pack(fill="both", expand=True, padx=8, pady=4)
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        canvas = tk.Canvas(container, highlightthickness=0)
        v_scroll = ttk.Scrollbar(container, orient="vertical", command=canvas.yview)
        h_scroll = ttk.Scrollbar(container, orient="horizontal", command=canvas.xview)
        rows_frame = ttk.Frame(canvas)

        # Horizontal scrolling keeps rows reachable when they don't fit the window width.
        rows_frame.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
        canvas.create_window((0, 0), window=rows_frame, anchor="nw")
        canvas.configure(yscrollcommand=v_scroll.set, xscrollcommand=h_scroll.set)

        canvas.grid(row=0, column=0, sticky="nsew")
        v_scroll.grid(row=0, column=1, sticky="ns")
        h_scroll.grid(row=1, column=0, sticky="ew")

        self.belt_to_pc_vars = {}

        touch_section = ttk.LabelFrame(rows_frame, text="Toque")
        touch_section.pack(fill="x", padx=4, pady=(4, 8))
        self._build_command_rows(touch_section, TOUCH_COMMANDS)

        movement_section = ttk.LabelFrame(rows_frame, text="Movimento")
        movement_section.pack(fill="x", padx=4, pady=(0, 8))
        self._build_command_rows(movement_section, MOVEMENT_COMMANDS)

    def _build_command_rows(self, parent, commands):
        """
        @brief Builds one row for each belt command.
        @param parent Parent widget.
        @param commands Belt commands.
        """
        for row, command in enumerate(commands):
            config = self.mappings.belt_to_pc.get(command, {"action": ACTION_LOG, "key": ""})

            ttk.Label(parent, text=command, width=18).grid(row=row, column=0, sticky="w", pady=2, padx=(4, 0))

            action_var = tk.StringVar(value=config.get("action", ACTION_LOG))
            key_var = tk.StringVar(value=config.get("key", ""))

            key_display = ttk.Entry(parent, textvariable=key_var, width=10, state="readonly")
            choose_button = ttk.Button(parent, text="Escolher tecla...")

            def make_toggle(entry=key_display, button=choose_button, var=action_var):
                def toggle():
                    state = "readonly" if var.get() == ACTION_KEY else "disabled"
                    entry.configure(state=state)
                    button.configure(state="normal" if var.get() == ACTION_KEY else "disabled")
                return toggle

            toggle = make_toggle()

            def make_choose(var=key_var):
                def choose():
                    key = ask_key(self)
                    if key:
                        var.set(key)
                return choose

            choose_button.configure(command=make_choose())

            ttk.Radiobutton(
                parent, text="Só registrar", variable=action_var, value=ACTION_LOG, command=toggle
            ).grid(row=row, column=1, sticky="w")

            ttk.Radiobutton(
                parent, text="Simular tecla:", variable=action_var, value=ACTION_KEY, command=toggle
            ).grid(row=row, column=2, sticky="w")

            key_display.grid(row=row, column=3, sticky="w", padx=(4, 4))
            choose_button.grid(row=row, column=4, sticky="w", padx=(0, 4))
            toggle()

            self.belt_to_pc_vars[command] = (action_var, key_var)

    def _collect_belt_to_pc(self):
        """
        @brief Copies the belt to PC mappings from the widgets to the mappings.
        """
        for command, (action_var, key_var) in self.belt_to_pc_vars.items():
            self.mappings.set_belt_to_pc(command, action_var.get(), key_var.get().strip())

    def _save_mappings(self):
        """
        @brief Saves the mappings to the mappings file.
        """
        self._collect_belt_to_pc()
        self.mappings.save()
        self._log("Mappings saved.")

    def _poll_events(self):
        """
        @brief Handles the ConnectionWorker events. Runs every 100 ms.
        """
        try:
            while True:
                kind, payload = self.event_queue.get_nowait()

                if kind == "status":
                    self._set_status(payload)
                elif kind == "log":
                    self._log(payload)
                elif kind == "sent":
                    self.last_sent_label.config(text=f"Last sent: {payload}")
                    self._log(f"Sent: {payload}")
                elif kind == "received":
                    self.last_received_label.config(text=f"Last received: {payload}")
                    self._log(f"Received: {payload}")
        except queue.Empty:
            pass

        self.after(100, self._poll_events)

    def _start_tray_icon(self):
        """
        @brief Starts the system tray icon.
        """
        menu = pystray.Menu(
            pystray.MenuItem("Show", self._tray_show, default=True),
            pystray.MenuItem("Exit", self._tray_exit),
        )
        self.tray_icon = pystray.Icon("haptflelt", _create_tray_image(), "HaptFlelt Desktop", menu)

        # Icon.run() blocks with its own event loop, so it needs its own thread.
        threading.Thread(target=self.tray_icon.run, daemon=True).start()

    def _on_window_close(self):
        """
        @brief Handles the window close button.
        """
        # Only hides the window, keeping the connection running. The app is closed from the tray menu.
        if TRAY_AVAILABLE:
            self.withdraw()
        else:
            self._shutdown()

    def _tray_show(self, icon=None, item=None):
        """
        @brief Shows the window. Called from the tray menu.
        """
        # pystray calls this from its own thread, so the widget call is scheduled on Tkinter's thread.
        self.after(0, self.deiconify)

    def _tray_exit(self, icon=None, item=None):
        """
        @brief Exits the app. Called from the tray menu.
        """
        self.after(0, self._shutdown)

    def _shutdown(self):
        """
        @brief Stops the connection and closes the app.
        """
        self.worker.stop()
        if self.tray_icon is not None:
            self.tray_icon.stop()
        self.destroy()


if __name__ == "__main__":
    app = HaptFleltApp()
    app.mainloop()
