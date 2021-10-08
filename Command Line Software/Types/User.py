from Types.Temperature import TemperatureRepresentation

class UserPrefs:
    def __init__(self, temp_pref : TemperatureRepresentation = TemperatureRepresentation.CELSIUS):
            self.temp_pref : TemperatureRepresentation = temp_pref

    def set_temp_pref(self, temp_pref_int : int):
        self.temp_pref = TemperatureRepresentation(temp_pref_int)

    def temp_pref_string(self) -> str:
        temp_string : str = ""

        if self.temp_pref == TemperatureRepresentation.CELSIUS:
            temp_string = "°C"
        elif self.temp_pref == TemperatureRepresentation.FAHRENHEIT:
            temp_string = "°F"

        return temp_string

    def serialise(self) -> bytearray:
        serialised_data : bytearray = bytearray(1)

        serialised_data[0] = self.temp_pref.value

        return serialised_data

class User:
    """
    At this point, the user only contains user prefs, but I decided to create an external User class incase future developments
    were to involve more user related settings on the computer.
    """
    def __init__(self, prefs : UserPrefs = UserPrefs()):
        self.prefs = prefs
    
    def serialise(self) -> bytearray:
        serialised_data : bytearray = bytearray(1)

        serialised_data[0] = self.prefs.temp_pref.value

        return serialised_data