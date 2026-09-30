"""
@file receiver.py
@brief BluetoothReceiver class definition
@author Franklin
"""
import serial


class BluetoothReceiver:
    """
    @brief Reads the commands received from the belt.
    """

    def __init__(self, serial_conn: serial.Serial):
        """
        @brief Constructor with parameters.
        @param serial_conn Bluetooth serial connection.
        """
        self.serial_conn = serial_conn  # Bluetooth serial connection.

    def receive_message(self):
        """
        @brief Reads a message received from the belt.
        @return Received message, or `None` if there is no message.
        @exception serial.SerialException If the connection is lost.
        """
        try:
            if self.serial_conn.in_waiting > 0:
                raw = self.serial_conn.readline()
                mensagem = raw.decode('utf-8', errors='replace').strip()
                return mensagem or None
        except serial.SerialException:
            # Connection loss is handled by the caller.
            raise
        except UnicodeDecodeError:
            print("⚠️ Mensagem recebida corrompida, ignorando.")

        return None
