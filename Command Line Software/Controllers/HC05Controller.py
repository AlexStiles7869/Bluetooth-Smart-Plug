from serial import Serial, SerialException
from time import sleep

from typing import Tuple

from Packets.ConnectionPacket import ConnectionPacket
from Packets.ConnectionConfirmPacket import ConnectionConfirmPacket

class HC05_Controller:
    def __init__(self, port : str, baudrate : int, timeout : int):
        self.port = port
        self.baudrate = baudrate
        self.timeout = timeout

        self.connection = None

    def establish_connection(self) -> Tuple[int, int, int]:
        # Establish a serial connection with the HC-05 attached to the Arduino
        connected : bool = False
        attempt_count : int = 0
        conn_estab = 0; error = 0; setup = 0
        print("Establishing Connection...")

        while not connected and attempt_count < 3:
            try:
                self.connection = Serial(port="/dev/cu.SMARTPLUG-SPPDev", baudrate=9600, timeout=1)
                print("Connection Possible")

                # Delay to avoid retriggering the bootloader https://arduino.stackexchange.com/questions/58061/problem-sending-string-with-python-to-arduino-through-serial-port
                sleep(2)

                # Create and send connection proper (proper in this sense meaning allowed to issue preset commands, somewhat pointless but provides starting point for security improvements) request
                conn_packet : ConnectionPacket = ConnectionPacket.create_packet()
                self.write_bytes(conn_packet)

                # Wait for confirmation and act accordingly
                try:
                    conn_confirm_packet : bytearray = self.read_bytes(ConnectionConfirmPacket.PACKET_SIZE)
                    conn_estab, error, setup = ConnectionConfirmPacket.get_packet_values(conn_confirm_packet)
                except IndexError as e:
                    raise SerialException("Didn't receive connection confirmation")

                if conn_estab:
                    print("Permission Granted")
                    connected = True
                    if error:
                        raise SystemExit("Unknown Error on Smartplug")
                else:
                    raise SystemExit("Permission Denied")
            except SerialException as e:
                print(e)
                print("Connection Failed")
                print("Attempting connection again in 1 second")
                sleep(1)
            finally:
                attempt_count += 1

        if not connected:
            raise SystemExit("Unable to establish connection.")

        return (conn_estab, error, setup)
    
    def end_connection(self):
        print("Ending Connection")
        # End the connection
        self.connection.close()

    def write_bytes(self, bytes : bytes):
        self.connection.write(bytes)

    def read_bytes(self, read_size : int) -> bytearray:
        read_bytes : bytearray = self.connection.read(read_size)

        return read_bytes