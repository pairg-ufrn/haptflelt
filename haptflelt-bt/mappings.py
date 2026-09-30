"""
@file mappings.py
@brief Mappings class definition
@author Franklin
"""

import json
import os

FEEDBACK_OPTIONS = [
    ("Vibração frontal", "up"),
    ("Vibração traseira", "down"),
    ("Vibração esquerda", "left"),
    ("Vibração direita", "right"),
]  # (GUI label, command) pairs recognized by the firmware's BluetoothReceiver.

FEEDBACK_LABEL_TO_COMMAND = {label: command for label, command in FEEDBACK_OPTIONS}  # GUI label to command.
FEEDBACK_COMMAND_TO_LABEL = {command: label for label, command in FEEDBACK_OPTIONS}  # Command to GUI label.

DEFAULT_PC_TO_BELT = {
    "up": "up",
    "down": "down",
    "left": "left",
    "right": "right",
}  # Default PC key to belt command mapping.

TOUCH_COMMANDS = [
    "TOUCH_LEFT", "TOUCH_RIGHT", "TOUCH_FRONT_RIGHT", "TOUCH_FRONT_LEFT",
    "TOUCH_L_R", "TOUCH_L_FR", "TOUCH_R_FL", "TOUCH_FR_FL",
]  # Touch commands sent by the belt (Bluetooth::sent_command_t in the firmware).

MOVEMENT_COMMANDS = [
    "MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT", "MOVE_FRONT", "MOVE_BACK",
]  # Movement commands sent by the belt (Bluetooth::sent_command_t in the firmware).

BELT_COMMANDS = TOUCH_COMMANDS + MOVEMENT_COMMANDS  # All commands sent by the belt.

ACTION_LOG = "log"  # Received command is only logged.
ACTION_KEY = "key"  # Received command simulates a key press while active.

MAPPINGS_FILE = os.path.join(os.path.dirname(os.path.abspath(__file__)), "mappings.json")  # Mappings file path.


class Mappings:
    """
    @brief Represents the PC to belt and belt to PC mappings.
    """

    def __init__(self):
        """
        @brief Default constructor. Loads the saved mappings, if any.
        """
        self.pc_to_belt = dict(DEFAULT_PC_TO_BELT)  # PC key to belt command.
        self.belt_to_pc = {cmd: {"action": ACTION_LOG, "key": ""} for cmd in BELT_COMMANDS}  # Belt command to PC action.
        self.load()

    def load(self):
        """
        @brief Loads the mappings from the mappings file.
        """
        if not os.path.exists(MAPPINGS_FILE):
            return

        try:
            with open(MAPPINGS_FILE, "r", encoding="utf-8") as f:
                data = json.load(f)
        except (OSError, json.JSONDecodeError) as e:
            print(f"Warning: could not read {MAPPINGS_FILE} ({e}), using defaults.")
            return

        self.pc_to_belt.update(data.get("pc_to_belt", {}))

        for cmd, cfg in data.get("belt_to_pc", {}).items():
            if cmd in self.belt_to_pc and isinstance(cfg, dict):
                self.belt_to_pc[cmd].update(cfg)

    def save(self):
        """
        @brief Saves the mappings to the mappings file.
        """
        data = {"pc_to_belt": self.pc_to_belt, "belt_to_pc": self.belt_to_pc}

        with open(MAPPINGS_FILE, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=2, ensure_ascii=False)

    def set_pc_to_belt(self, key, belt_command):
        """
        @brief Sets the belt command sent when a PC key is pressed.
        @param key PC key name.
        @param belt_command Belt command. If empty, the mapping is removed.
        """
        if belt_command:
            self.pc_to_belt[key] = belt_command
        else:
            self.pc_to_belt.pop(key, None)

    def remove_pc_to_belt(self, key):
        """
        @brief Removes the mapping of a PC key.
        @param key PC key name.
        """
        self.pc_to_belt.pop(key, None)

    def set_belt_to_pc(self, belt_command, action, key=""):
        """
        @brief Sets the PC action for a belt command.
        @param belt_command Belt command.
        @param action `ACTION_LOG` or `ACTION_KEY`.
        @param key Key simulated when action is `ACTION_KEY`.
        """
        if belt_command in self.belt_to_pc:
            self.belt_to_pc[belt_command] = {"action": action, "key": key}
