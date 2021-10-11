from enum import IntEnum
from typing import Tuple

from Controllers.HC05Controller import HC05_Controller
from Controllers.CoreController import CoreController
from Controllers.UserManager import UserManager
from Controllers.PresetManager import PresetManager

from Question.Options import OptionsContainer, StandardOptions
from Question.RestrictedQuestion import RestrictedQuestion

class CoreManager:
    class ManagementOptionsEnum(IntEnum):
        LIST_OPT: int = 0
        LIST_BLOCK : int = 1
        ADD : int = 2
        EDIT : int = 3
        REMOVE : int = 4

    def __init__(self, bluetooth_radio : HC05_Controller):
        # Options
        self.options_container = OptionsContainer(self.ManagementOptionsEnum)
        
        # Bluetooth Radio
        self.bluetooth_radio = bluetooth_radio

        # Controllers
        self.core_controller = CoreController(self.bluetooth_radio)

        # Managers
        self.preset_manager = PresetManager(self.core_controller)
        self.user_manager = UserManager(self.core_controller)

        # Establish Connection
        # conn_estab and error are provided for future uses. They are handled in HC05Controller.py.
        conn_estab, error, setup = self.establish_connection()

        if setup:
            self.setup()
        else:
            # Get the user data from the Arduino
            self.user_manager.retrieve_user()

    def establish_connection(self) -> Tuple[int, int, int]:
        # Establish link with bluetooth radio
        return self.bluetooth_radio.establish_connection()

    def end_connection(self):
        # End link with bluetooth radio
        self.bluetooth_radio.end_connection()

    def setup(self):
        print()
        print("***SMART PLUG SETUP***")
        print()
        # Create and add a new user
        self.user_manager.add_user()

    def ask_command_question(self) -> bool:
        command_question : RestrictedQuestion = RestrictedQuestion("What command would you like to run? ", OptionsContainer(self.ManagementOptionsEnum))
        # Ask the question, until a valid response is received
        command : int = command_question.ask_question()
        # Handle the option appropriately
        continuing : bool = self.handle_option(command_question, command)

        return continuing

    def handle_option(self, command_question : RestrictedQuestion, option : int) -> bool:
        if option == StandardOptions.EXIT:
            # Cleanup and exit
            self.end_connection()
            return False
        elif option == self.ManagementOptionsEnum.LIST_OPT:
            # List the available options
            command_question.valid_answers.list_options()
        elif option == self.ManagementOptionsEnum.LIST_BLOCK:
            # List presets
            self.preset_manager.get_presets()
        elif option == self.ManagementOptionsEnum.ADD:
            # Add preset
            self.preset_manager.add_preset(self.user_manager.user.prefs)
        elif option == self.ManagementOptionsEnum.EDIT:
            # Edit time block
            self.preset_manager.update_preset()
        elif option == self.ManagementOptionsEnum.REMOVE:
            self.preset_manager.remove_preset()

        return True

def intro():
    print(39 * "*")
    print(f"{5 * '*'} SMART PLUG CONTROL SOFTWARE {5 * '*'}")
    print(39 * "*")
    print()

def main():
    # Display nice intro banner
    intro()

    # Create serial connection to bluetooth radio (doesn't actually connect)
    # Let's hope no one changes the name of the HC-05
    bluetooth_radio = HC05_Controller("/dev/cu.SMARTPLUG-SPPDev", baudrate=9600, timeout=2)

    # Create a management controller
    management_controller : CoreManager = CoreManager(bluetooth_radio)

    # Check if command line arguments have been passed in
    asking_command_questions : bool = True
    while asking_command_questions:
        asking_command_questions = management_controller.ask_command_question()

if __name__ == "__main__":
    main()