class ConnectionConfirmPacket:
    # Size of the packet in bytes
    PACKET_SIZE = 3

    @staticmethod
    def get_packet_values(packet_bytes : bytearray):
        conn_estab : int = packet_bytes[0]
        error : int = packet_bytes[1]
        setup : int = packet_bytes[2]

        return (conn_estab, error, setup)

    
