from Controllers.HC05Controller import HC05_Controller

from Packets.ActionPacket import ActionPacket, Action
from Packets.ResponsePacket import Response, ResponsePacket
from Packets.UserDataPacket import UserDataPacket

class CoreController:
    def __init__(self, bluetooth_radio : HC05_Controller):
        self.bluetooth_radio = bluetooth_radio

    # USER

    def send_add_user_request(self, user_data : bytearray):
        # Create and send action packet
        action_packet : ActionPacket = ActionPacket.create_packet(Action.ADD_USER)
        self.bluetooth_radio.write_bytes(action_packet)

        # Wait for response
        response_bytes : bytearray = self.bluetooth_radio.read_bytes(ResponsePacket.PACKET_SIZE)
        response = ResponsePacket.get_packet_values(response_bytes)

        if (response == Response.REQUEST_ACCEPT):
            # The request has been accepted, send user data
            self.bluetooth_radio.write_bytes(user_data)
        if (response == Response.REQUEST_DENIED):
            # The request has been denied
            print("Bad")
        elif (response == Response.CONN_TERMINATED):
            # The connection has been terminated
            print("Bad")

    def receive_user_data(self) -> bytearray:
        return self.bluetooth_radio.read_bytes(UserDataPacket.PACKET_SIZE)

    def send_edit_user_request(self):
        pass

    # PRESETS

    def send_add_preset_request(self, preset_data : bytearray):
        # Inform the HC-05 that a timeblock is being added
        action_packet : ActionPacket = ActionPacket.create_packet(Action.ADD)
        self.bluetooth_radio.write_bytes(action_packet)

        # Wait for response
        response_bytes : bytearray = self.bluetooth_radio.read_bytes(ResponsePacket.PACKET_SIZE)
        response = ResponsePacket.get_packet_values(response_bytes)

        # Send the timeblock information
        if (response == Response.REQUEST_ACCEPT):
            # The request has been accepted, send user data
            self.bluetooth_radio.write_bytes(preset_data)
        if (response == Response.REQUEST_DENIED):
            # The request has been denied
            print("Bad")
        elif (response == Response.CONN_TERMINATED):
            # The connection has been terminated
            print("Bad")

    def send_edit_preset_request(self):
        # Inform the HC-05 that a timeblock is being edited

        # Send the id of the timeblock that was edited and new timeblock information
        pass

    def send_remove_preset_request(self):
        # Inform the HC-05 that a timeblock is being removed

        # Send the timeblock id for the Arduino to remove
        pass

    def send_get_preset_request(self, preset_id : int):
        # Inform the HC-05 that we want a specific preset

        # Receive the preset
        pass

    def send_get_presets_request(self):
        # Inform the HC-05 that we want all the presets
        
        # Receive the presets
        presets : bytearray = bytearray() 

        # Wait for confirmation