"""
@file ports.py
@brief Lists the available COM ports and the possible ESP32 ports
@author Franklin
"""
import serial.tools.list_ports

ports = list(serial.tools.list_ports.comports())

print("Portas COM disponíveis:")
for port in ports:
    print(f"{port.device} - {port.description}")

esp_ports = [port.device for port in ports if "Bluetooth" in port.description or "ESP32" in port.description]

if len(esp_ports) == 1:
    print(f"\nA ESP32 está na porta: {esp_ports[0]}")
elif len(esp_ports) == 2:
    print(f"\nPossíveis portas da ESP32 (Entrada e Saída): {esp_ports[0]} e {esp_ports[1]}")
else:
    print("\nNão foi possível identificar automaticamente as portas da ESP32. Verifique manualmente.")
