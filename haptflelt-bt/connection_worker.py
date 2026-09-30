"""
@file connection_worker.py
@brief ConnectionWorker class definition
@author Franklin
"""

import threading
import queue
import time

import serial
import keyboard

import utils
from config import BAUD_RATE
from sender import BluetoothSender
from receiver import BluetoothReceiver
from mappings import ACTION_KEY

HANDSHAKE_TIMEOUT = 2  # Time (s) to wait for the belt's "PONG".
RECONNECT_DELAY = 5    # Time (s) between reconnection attempts.


class ConnectionWorker:
    """
    @brief Manages the connection to the belt in a background thread.

    Events are sent to the GUI through a queue, as `(kind, payload)` tuples:
    - `"status"`: `bool`, `True` if connected.
    - `"log"`: `str`, line to be logged.
    - `"sent"`: `str`, command sent to the belt.
    - `"received"`: `str`, command received from the belt.
    """

    def __init__(self, mappings, event_queue: queue.Queue):
        """
        @brief Constructor with parameters.
        @param mappings PC to belt and belt to PC mappings.
        @param event_queue Queue where the events are sent to the GUI.
        """
        self.mappings = mappings  # PC to belt and belt to PC mappings.
        self.events = event_queue  # Queue where the events are sent to the GUI.

        self._stop = threading.Event()  # Set when the worker must stop.
        self._thread = None  # Background thread.

        self.serial_conn = None  # Bluetooth serial connection.
        self.sender = None  # Sends commands to the belt.
        self.receiver = None  # Reads commands from the belt.

        self._key_hook = None  # Keyboard hook of the current connection.
        self._last_belt_command = None  # Last command received from the belt.
        self._pressed_key = None  # Key currently being simulated.

    def start(self):
        """
        @brief Starts the background thread, if it isn't running.
        """
        if self._thread is not None and self._thread.is_alive():
            return

        self._stop.clear()
        self._thread = threading.Thread(target=self._run, daemon=True)
        self._thread.start()

    def stop(self):
        """
        @brief Stops the background thread and closes the connection.
        """
        self._stop.set()
        self._teardown_connection()

    def is_running(self):
        """
        @brief Checks if the background thread is running.
        @return `True` if the thread is running, `False` otherwise.
        """
        return self._thread is not None and self._thread.is_alive()

    def _try_connect(self, port):
        """
        @brief Opens a port and confirms, via handshake, that it is the HaptFlelt.
        @param port Port name.
        @return Open serial connection, or `None` if the port didn't respond.
        """
        try:
            conn = serial.Serial(port, BAUD_RATE, timeout=1)
        except serial.SerialException:
            return None

        if not conn.is_open:
            return None

        try:
            conn.reset_input_buffer()
            conn.write(b"PING\n")
            conn.flush()

            start = time.time()
            while time.time() - start < HANDSHAKE_TIMEOUT:
                if conn.in_waiting > 0:
                    reply = conn.readline().decode("utf-8", errors="replace").strip()
                    if reply == "PONG":
                        return conn
                time.sleep(0.05)
        except serial.SerialException:
            pass

        conn.close()
        return None

    def test_connection(self, port):
        """
        @brief Tests the connection to a port.
        @param port Port name.
        @return `True` if the port responded as the HaptFlelt, `False` otherwise.
        @note Blocks for up to `HANDSHAKE_TIMEOUT` seconds, so it must not be called from the GUI thread.
        """
        conn = self._try_connect(port)
        if conn:
            conn.close()
            return True
        return False

    def _connect_any(self):
        """
        @brief Tries to connect to the belt on each candidate port.
        @return Open serial connection, or `None` if no port responded.
        """
        ports = utils.find_esp32_ports()

        if not ports:
            self.events.put(("log", "No candidate Bluetooth ports found."))
            return None

        self.events.put(("log", f"Candidate ports: {', '.join(ports)}"))

        for port in ports:
            self.events.put(("log", f"Trying {port}..."))
            conn = self._try_connect(port)
            if conn:
                self.events.put(("log", f"Connected on {port}."))
                return conn

        self.events.put(("log", "None of the candidate ports responded."))
        return None

    def _run(self):
        """
        @brief Background thread loop: connects, reads the belt commands and reconnects when the connection is lost.
        """
        while not self._stop.is_set():
            self.serial_conn = self._connect_any()

            if self.serial_conn is None:
                self.events.put(("status", False))
                self._wait(RECONNECT_DELAY)
                continue

            self.sender = BluetoothSender(self.serial_conn)
            self.receiver = BluetoothReceiver(self.serial_conn)
            self.events.put(("status", True))

            self._register_key_hook()

            try:
                while not self._stop.is_set() and self.serial_conn.is_open:
                    message = self.receiver.receive_message()
                    if message:
                        self._handle_belt_message(message)
                    time.sleep(0.01)
            except serial.SerialException:
                self.events.put(("log", "Connection lost, retrying..."))

            self._teardown_connection()
            self.events.put(("status", False))

            self._wait(RECONNECT_DELAY)

    def _wait(self, seconds):
        """
        @brief Waits for a certain period of time, or until the worker is stopped.
        @param seconds Waiting duration.
        """
        # Small steps, so stop() can interrupt the wait quickly.
        end = time.time() + seconds
        while time.time() < end and not self._stop.is_set():
            time.sleep(0.1)

    def _teardown_connection(self):
        """
        @brief Removes the keyboard hook, releases the simulated key and closes the connection.
        """
        if self._key_hook is not None:
            keyboard.unhook(self._key_hook)
            self._key_hook = None

        self._release_key()

        if self.serial_conn is not None and self.serial_conn.is_open:
            self.serial_conn.close()

        self.sender = None
        self.receiver = None
        self._last_belt_command = None

    def _register_key_hook(self):
        """
        @brief Registers the keyboard hook that sends the mapped commands to the belt.
        """
        self._key_hook = keyboard.on_press(self._on_keypress)

    def _on_keypress(self, event):
        """
        @brief Sends the command mapped to the pressed key to the belt.
        @param event Keyboard event.
        """
        if self.sender is None:
            return

        belt_command = self.mappings.pc_to_belt.get(event.name)
        if not belt_command:
            return

        self.sender.send_message(belt_command)
        self.events.put(("sent", belt_command))

    def _handle_belt_message(self, message):
        """
        @brief Executes the PC action mapped to a command received from the belt.
        @param message Command received from the belt.
        """
        # The belt repeats the active command continuously, so only changes are handled.
        if message == self._last_belt_command:
            return

        self.events.put(("received", message))

        self._release_key()

        config = self.mappings.belt_to_pc.get(message)
        if config and config.get("action") == ACTION_KEY and config.get("key"):
            keyboard.press(config["key"])
            self._pressed_key = config["key"]

        self._last_belt_command = message

    def _release_key(self):
        """
        @brief Releases the simulated key, if any.
        """
        if self._pressed_key:
            keyboard.release(self._pressed_key)
            self._pressed_key = None
