from Types.User import UserPrefs
from Types.Timeblock import TimeBlock, Time
from Types.Temperature import Temperature, TemperatureRepresentation

class Preset:
    PRESET_BYTE_SIZE = 5 # Number of bytes in a preset
    ID = 0 # ID of the preset

    def __init__(self, time_block : TimeBlock, temperature : Temperature, user_prefs : UserPrefs = UserPrefs()):
        self.user_prefs = user_prefs # This is not serialised

        self.time_block : TimeBlock = time_block
        self.temperature : Temperature = temperature

    @staticmethod
    def deserialise(preset_bytes) -> 'Preset':
        if (len(preset_bytes) > Preset.PRESET_BYTE_SIZE):
            raise ValueError(f"Cannot create Preset from {len(preset_bytes)} bytes, Preset has {Preset.PRESET_BYTE_SIZE} bytes")
        
        preset : Preset = Preset(TimeBlock.deserialise(preset_bytes[0 : 4]), Temperature.deserialise(preset_bytes[4]))

        return preset

    def serialise(self) -> bytearray:
        serialised_data : bytearray = bytearray(Preset.PRESET_BYTE_SIZE)

        serialised_data[0 : 4] = self.time_block.serialise()
        serialised_data[4] = self.temperature.serialise()

        return serialised_data

    def __str__(self):
        return f"{self.temperature.temp_value}{self.user_prefs.temp_pref_string()} | {self.time_block}"

def main():
    preset : Preset = Preset(TimeBlock(Time(9,00), Time(16,00)), Temperature(18))
    print(preset)
    print(preset.serialise())
    print(Preset.deserialise(preset.serialise()))

if __name__ == "__main__":
    main()