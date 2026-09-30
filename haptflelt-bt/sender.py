"""
@file sender.py
@brief BluetoothSender class definition
@author Franklin
"""
import serial


class BluetoothSender:
    """
    @brief Sends commands to the belt.
    """

    def __init__(self, serial_conn: serial.Serial):
        """
        @brief Constructor with parameters.
        @param serial_conn Bluetooth serial connection.
        """
        self.serial_conn = serial_conn  # Bluetooth serial connection.

    def send_message(self, message: str) -> bool:
        """
        @brief Sends a command to the belt.
        @param message Command to be sent.
        @return `True` if the message was sent, `False` otherwise.
        """
        payload = (message + '\n').encode('utf-8')

        try:
            written = self.serial_conn.write(payload)
            self.serial_conn.flush()
        except serial.SerialException as e:
            print(f"Erro ao enviar mensagem: {e}")
            return False

        if written == len(payload):
            print(f"Mensagem enviada com sucesso: {message}")
            return True
        else:
            print(f"Erro ao enviar mensagem: {written}/{len(payload)} bytes escritos")
            return False
