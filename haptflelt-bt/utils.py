"""
@file utils.py
@brief Serial ports utilities
@author Franklin
"""
import serial.tools.list_ports


def find_esp32_ports():
    """
    @brief Finds the ports where the ESP32 may be connected via Bluetooth Serial.

    On Windows, a paired Bluetooth device usually creates two COM ports (incoming
    and outgoing), so all candidates are returned to be tested in order.

    @return List of candidate port names.
    """
    ports = serial.tools.list_ports.comports()
    return [
        port.device for port in ports
        if "bluetooth" in port.description.lower() or "esp32" in port.description.lower()
    ]


if __name__ == "__main__":
    found = find_esp32_ports()
    if found:
        print(f"✅ Possíveis portas do ESP32 encontradas: {', '.join(found)}")
    else:
        print("⚠️ Nenhuma porta Bluetooth do ESP32 foi encontrada.")
