"""
@file config.py
@brief Connection settings
@author Franklin
"""
from utils import find_esp32_ports

BAUD_RATE = 115200  # Serial baud rate.

BLUETOOTH_PORTS = find_esp32_ports()  # Candidate ports, validated by handshake when connecting.

# Manual override, if auto-detection doesn't find the belt:
# BLUETOOTH_PORTS = ['COM7']

print(f"Portas candidatas: {BLUETOOTH_PORTS}")

if not BLUETOOTH_PORTS:
    print("Warning: no ESP32 Bluetooth port found yet. Pair the device and rescan.")
