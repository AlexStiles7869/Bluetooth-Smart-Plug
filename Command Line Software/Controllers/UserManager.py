from Controllers.CoreController import CoreController

from Types.Temperature import TemperatureRepresentation
from Types.User import User, UserPrefs

from Question.RestrictedQuestion import RestrictedQuestion, OptionsContainer

class UserManager:
    def __init__(self, core_controller : CoreController):
        self.core_controller = core_controller
        self.user = None

    def add_user(self):
        # Ask the user their temperature representation preference
        user_temperature_rep_question : RestrictedQuestion = RestrictedQuestion("Temperature Representation (°C = 0 or °F = 1)? ", OptionsContainer(TemperatureRepresentation, False))
        user_temperature_rep = user_temperature_rep_question.ask_question()

        # Create the user prefs and user based on input
        user_prefs : UserPrefs = UserPrefs(TemperatureRepresentation(user_temperature_rep))
        user : User = User(user_prefs)

        # Add user to UserManager for future use
        self.user = user

        # Serialise the data
        user_data = user.serialise()

        # Send to the Arduino
        self.core_controller.send_add_user_request(user_data)


    def edit_user(self):
        print("Not implemented")