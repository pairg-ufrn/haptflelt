"""
@file keyboard_picker.py
@brief KeyboardPicker class definition
@author Franklin
"""

import tkinter as tk

# Rows of (label, key_name, relative_width) tuples. key_name follows the `keyboard` library naming.
_MAIN_ROWS = [
    [("Esc", "esc", 1.5)] + [(f"F{i}", f"f{i}", 1) for i in range(1, 13)],
    [("`", "`", 1)] + [(str(n), str(n), 1) for n in range(1, 10)] + [("0", "0", 1), ("-", "-", 1), ("=", "=", 1), ("Backspace", "backspace", 2)],
    [("Tab", "tab", 1.5)] + [(c, c, 1) for c in "qwertyuiop"] + [("[", "[", 1), ("]", "]", 1), ("\\", "\\", 1.5)],
    [("Caps", "caps lock", 1.75)] + [(c, c, 1) for c in "asdfghjkl"] + [(";", ";", 1), ("'", "'", 1), ("Enter", "enter", 2.25)],
    [("Shift", "shift", 2.25)] + [(c, c, 1) for c in "zxcvbnm"] + [(",", ",", 1), (".", ".", 1), ("/", "/", 1), ("Shift", "shift", 2.25)],
    [("Ctrl", "ctrl", 1.25), ("Win", "windows", 1.25), ("Alt", "alt", 1.25), ("Space", "space", 6.25),
     ("Alt", "alt", 1.25), ("Win", "windows", 1.25), ("Menu", "menu", 1.25), ("Ctrl", "ctrl", 1.25)],
]

_ARROW_ROWS = [
    [None, ("^", "up"), None],
    [("<", "left"), ("v", "down"), (">", "right")],
]  # Rows of (label, key_name) tuples. `None` is an empty cell.

_UNIT = 10  # Base width unit, in characters.


class KeyboardPicker(tk.Toplevel):
    """
    @brief Popup that shows a keyboard layout and lets the user click a key.
    """

    def __init__(self, parent):
        """
        @brief Constructor with parameters.
        @param parent Parent window.
        """
        super().__init__(parent)
        self.title("Choose a key")
        self.resizable(False, False)
        self.transient(parent)
        self.grab_set()

        self.selected_key = None  # Chosen key name, or `None` if cancelled.

        main = tk.Frame(self, padx=8, pady=8)
        main.pack(side="left", fill="both")

        for row in _MAIN_ROWS:
            row_frame = tk.Frame(main)
            row_frame.pack(pady=1, anchor="w")
            for label, key_name, rel_width in row:
                btn = tk.Button(
                    row_frame,
                    text=label,
                    width=max(1, int(rel_width * 2)),
                    command=lambda k=key_name: self._select(k),
                )
                btn.pack(side="left", padx=1)

        arrows = tk.Frame(self, padx=8, pady=8)
        arrows.pack(side="left", anchor="s")

        for row in _ARROW_ROWS:
            row_frame = tk.Frame(arrows)
            row_frame.pack()
            for cell in row:
                if cell is None:
                    tk.Frame(row_frame, width=40, height=32).pack(side="left", padx=1, pady=1)
                else:
                    label, key_name = cell
                    tk.Button(
                        row_frame, text=label, width=3,
                        command=lambda k=key_name: self._select(k),
                    ).pack(side="left", padx=1, pady=1)

        cancel = tk.Button(self, text="Cancel", command=self.destroy)
        cancel.pack(side="bottom", pady=(0, 8))

    def _select(self, key_name):
        """
        @brief Selects a key and closes the popup.
        @param key_name Key name.
        """
        self.selected_key = key_name
        self.destroy()


def ask_key(parent):
    """
    @brief Opens the keyboard picker and waits until a key is chosen or the popup is closed.
    @param parent Parent window.
    @return Chosen key name, or `None` if cancelled.
    """
    picker = KeyboardPicker(parent)
    parent.wait_window(picker)
    return picker.selected_key
