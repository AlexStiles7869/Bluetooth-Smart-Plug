class ConnectionPacket:
    # Size of the packet in bytes
    PACKET_SIZE = 1
    # Magic number sent to indicate that the computer can be trusted, the connection has already been established at this point so this is a vanity measure
    MAGIC_NO = 100
    
    @staticmethod
    def create_packet():
        packet : bytearray = bytearray(ConnectionPacket.PACKET_SIZE)

        packet[0] = ConnectionPacket.MAGIC_NO

        return packet
