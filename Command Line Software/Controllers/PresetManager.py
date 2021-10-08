"""
Brief: Source file for preset helper for user preset creation on a computer.
The file produces the output file required to create the desired time block on the smart plug.
The validity of the times entered are checked, and extreme temperatures raise warnings.

Author: Alex Stiles
Date: 06/10/21
"""

from enum import IntEnum
from typing import List

from Controllers.CoreController import CoreController

from Question.OpenQuestion import OpenQuestion

from Types.Preset import Preset
from Types.Temperature import Temperature
from Types.Timeblock import TimeBlock, Time
from Types.User import UserPrefs

class PresetManager:
    class PresetFields(IntEnum):
        START_TIME : int = 0
        END_TIME : int = 1
        TEMP : int = 2

    def __init__(self, core_controller : CoreController):
        self.core_controller = core_controller

    def add_preset(self, user_prefs : UserPrefs):
        # Get the start time
        start_time_question = OpenQuestion("Start Time (HH:MM): ", Time.check_validity, "Input doesn't follow format HH:MM")
        start_time_str = start_time_question.ask_question()

        # Get the end time
        end_time_question = OpenQuestion("End Time (HH:MM): ", Time.check_validity, "Input doesn't follow format HH:MM")
        end_time_str = end_time_question.ask_question()

        # Create time block
        start_time_hour, start_time_minute = [int(i) for i in start_time_str.split(":")]
        end_time_hour, end_time_minute = [int(i) for i in end_time_str.split(":")]

        timeblock : TimeBlock = TimeBlock(Time(start_time_hour, start_time_minute), Time(end_time_hour, end_time_minute))

        # Get all the other timeblocks
        presets : List[Preset] = self.get_presets()

        # Check if the timeblock overlaps with any other time blocks
        

        # Ask for the temperature
        temperature_question = OpenQuestion(f"Temperature ({user_prefs.temp_pref_string()}): ", lambda user_input: Temperature.check_validity(user_input, user_prefs.temp_pref), f"Temperature isn't between 0 and 40 °C")
        temperature_value = int(temperature_question.ask_question())

        temperature : Temperature = Temperature(temperature_value)

        preset : Preset = Preset(timeblock, temperature)
        preset_data : bytearray = preset.serialise()

        self.core_controller.send_add_preset_request(preset_data)
    
    def edit_preset(self):
        preset_id : int = None
        # Get the preset
        preset : Preset = self.preset_manager.get_preset(preset_id)
        # Update the edited preset

    def remove_preset(self, presed_id : int):
        pass

    def get_preset(self, preset_id : int) -> Preset:
        pass

    def get_presets(self) -> List[Preset]:
        self.core_controller.send_get_presets_request()
    
        

def main():
    """ Entry point of program when run directly. """


if __name__ == "__main__":
    main()