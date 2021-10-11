class UserDataPacket:
    # Size of the packet in bytes
    PACKET_SIZE = 1

    @staticmethod
    def get_packet_values(packet_bytes : bytearray):
        temp_pref : int = packet_bytes[0]

        return temp_pref