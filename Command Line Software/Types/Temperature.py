from enum import IntEnum

class TemperatureRepresentation(IntEnum):
    CELSIUS : int = 0
    FAHRENHEIT : int = 1

class Temperature:
    def __init__(self, temp_value : int):
        self.temp_value : int = temp_value

    @staticmethod
    def check_validity(temp_str : str, current_temp_rep : TemperatureRepresentation) -> bool:
        valid : bool = None
        
        try:
            # Attempt to convert the temp value to an integer
            temp_value : int = int(temp_str)
            # Convert the values to celsius for bound checking
            converted_value : int = Temperature.convert(temp_value, current_temp_rep)

            if (converted_value >= 0 and converted_value <= 40):
                valid = True
        except ValueError:
            valid = False
        
        return valid

    @staticmethod
    def convert(temp_value : int, current_temp_rep : TemperatureRepresentation, converted_temp_rep : TemperatureRepresentation = TemperatureRepresentation.CELSIUS):
        converted_temp_value : int = None

        # Handle the easy case first

        if (current_temp_rep == converted_temp_rep):
            converted_temp_value = temp_value
            return converted_temp_value

        if (current_temp_rep == TemperatureRepresentation.CELSIUS):
            # This will always be true since there are currently only two options, but this could explain in future
            if (converted_temp_rep == TemperatureRepresentation.FAHRENHEIT):
                converted_temp_value = (temp_value * (9.0 / 5.0)) + 32
        elif (current_temp_rep == TemperatureRepresentation.FAHRENHEIT):
            # Again, currently will always be true with the current two options
            if (converted_temp_rep == TemperatureRepresentation.CELSIUS):
                converted_temp_value = (temp_value - 32) * (5.0 / 9.0)

        else:
            raise NotImplementedError("Conversion not handled")

        return converted_temp_value

    def serialise(self) -> int:
        return self.temp_value
    
    def deserialise(temperature_bytes : bytearray) -> 'Temperature':
        temperature : Temperature = Temperature(temperature_bytes)
        
        return temperature