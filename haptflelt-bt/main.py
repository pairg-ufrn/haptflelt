"""
@file main.py
@brief Command line daemon
@author Franklin
"""
import time
import serial
import keyboard
from receiver import BluetoothReceiver
from sender import BluetoothSender

from config import BAUD_RATE, BLUETOOTH_PORTS

HANDSHAKE_TIMEOUT = 2  # Time (s) to wait for the belt's "PONG".


def process_keypress(event, sender: BluetoothSender):
    """
    @brief Sends the pressed arrow key to the belt.
    @param event Keyboard event.
    @param sender Bluetooth sender.
    """
    if event.name in ['up', 'down', 'left', 'right']:
        print(f"🔹 Tecla pressionada: {event.name}")
        sender.send_message(event.name)


def try_connect(port, baud_rate):
    """
    @brief Opens a port and confirms, via handshake, that it is the HaptFlelt.
    @param port Port name.
    @param baud_rate Serial baud rate.
    @return Open serial connection, or `None` if the port didn't respond.
    """
    try:
        conn = serial.Serial(port, baud_rate, timeout=1)
    except serial.SerialException as e:
        print(f"Não foi possível abrir {port}: {e}")
        return None

    if not conn.is_open:
        return None

    try:
        conn.reset_input_buffer()
        conn.write(b'PING\n')
        conn.flush()

        start = time.time()
        while time.time() - start < HANDSHAKE_TIMEOUT:
            if conn.in_waiting > 0:
                resposta = conn.readline().decode('utf-8', errors='replace').strip()
                if resposta == 'PONG':
                    return conn
            time.sleep(0.05)
    except serial.SerialException as e:
        print(f"Erro durante handshake em {port}: {e}")

    print(f"{port} não respondeu como HaptFlelt, tentando próxima porta...")
    conn.close()
    return None


def connect_to_belt():
    """
    @brief Tries to connect to the belt on each candidate port.
    @return Open serial connection, or `None` if no port responded.
    """
    for port in BLUETOOTH_PORTS:
        print(f"Tentando conectar em {port}...")
        conn = try_connect(port, BAUD_RATE)
        if conn:
            print(f"✅ Conectado ao HaptFlelt em {port}!")
            return conn
    return None


def main():
    """
    @brief Connects to the belt and exchanges commands, reconnecting when the connection is lost.
    """
    print("🔵 Iniciando comunicação Bluetooth com o cinto...")
    active_hook = None

    try:
        while True:
            serial_conn = connect_to_belt()

            if serial_conn is None:
                print("\nFalha na conexão... Tentando novamente em 5 segundos.")
                time.sleep(5)
                continue

            try:
                receiver = BluetoothReceiver(serial_conn)
                sender = BluetoothSender(serial_conn)

                # Removes the previous connection's hook, otherwise each key
                # press would be sent once per reconnection.
                if active_hook is not None:
                    keyboard.unhook(active_hook)

                active_hook = keyboard.on_press(lambda event: process_keypress(event, sender))

                while serial_conn.is_open:
                    mensagem = receiver.receive_message()
                    if mensagem:
                        print(f"Recebido: {mensagem}")

                    time.sleep(0.01)

            except serial.SerialException:
                print("\nConexão perdida... Tentando novamente em 5 segundos.")

            finally:
                if serial_conn is not None and serial_conn.is_open:
                    serial_conn.close()

            time.sleep(5)

    except KeyboardInterrupt:
        print("\n⏹️ Encerrando comunicação...")

    finally:
        if active_hook is not None:
            keyboard.unhook(active_hook)


if __name__ == "__main__":
    main()
