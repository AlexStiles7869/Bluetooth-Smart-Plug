from typing import List

from enum import IntEnum

class StandardOptions(IntEnum):
    EXIT = -1

class OptionsContainer:
    def __init__(self, options : IntEnum, standard_options = True):
        self.options : IntEnum = options
        self.standard_options = standard_options

    def list_options(self) -> None:
        if self.standard_options:
            for standard_option in StandardOptions:
                print(f"{standard_option.name} = {standard_option.value}")

        for option in self.options:
            print(f"{option.name} = {option.value}")
    
    def option_values(self) -> List[int]:
        values : List = [option.value for option in self.options]

        if self.standard_options:
            values += [standard_option.value for standard_option in StandardOptions]

        return values