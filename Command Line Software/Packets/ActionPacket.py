from enum import IntEnum

class Action(IntEnum):
    TERMINATE_CONN = 0
    ADD_USER = 1
    EDIT_USER = 2
    GET_ALL = 3
    GET = 4
    ADD = 5
    EDIT = 6
    REMOVE = 7

class ActionPacket:
    # Size of the packet in bytes
    PACKET_SIZE = 1
    
    @staticmethod
    def create_packet(action : Action) -> bytearray:
        packet : bytearray = bytearray(ActionPacket.PACKET_SIZE)

        packet[0] = int(action)

        return packet
