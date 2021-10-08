from enum import IntEnum

class Response(IntEnum):
    CONN_TERMINATED = 0
    REQUEST_ACCEPT = 1
    REQUEST_DENIED = 2

class ResponsePacket:
    # Size of the packet in bytes
    PACKET_SIZE = 1

    @staticmethod
    def get_packet_values(packet_bytes : bytearray):
        response : int = packet_bytes[0]

        return response
