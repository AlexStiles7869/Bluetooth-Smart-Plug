import time

class Time:
    def __init__(self, hour : int, minute : int):
        self.hour : int = hour
        self.minute : int = minute

    @staticmethod
    def check_validity(time_string : str) -> bool:
        valid : bool = None

        try:
            time.strptime(time_string, "%H:%M")
            valid = True
        except ValueError:
            valid = False

        return valid

    def serialise(self) -> bytearray:
        serialised_data : bytearray = bytearray(2)
        
        serialised_data[0] = self.hour
        serialised_data[1] = self.minute
        
        return serialised_data

    def deserialise(time_bytes : bytearray) -> 'Time':
        time : Time = Time(time_bytes[0], time_bytes[1])

        return time

    def __ge__(self, other : 'Time') -> bool:
        return (self.hour > other.hour or (self.hour >= other.hour and self.minute >= other.minute))

    def __str__(self) -> str:
        return f"{self.hour:02}:{self.minute:02}"


class TimeBlock:
    HOURS : int = 24
    MINUTES : int = 60

    def __init__(self, start_time : Time, end_time : Time):
        self.start_time : Time = start_time
        self.end_time : Time = end_time

    def time_block_duration(self) -> Time:
        hour_duration : int = 0
        minute_duration : int = 0

        # Determine hour duration

        if self.start_time.hour < self.end_time.hour:
            hour_duration = self.end_time.hour - self.start_time.hour
        else:
            hour_duration = TimeBlock.HOURS - (self.start_time.hour - self.end_time.hour)

        # Determine minute duration

        if self.start_time.minute <= self.end_time.minute:
            minute_duration = self.end_time.minute - self.start_time.minute
        else:
            hour_duration -= 1
            minute_duration = TimeBlock.MINUTES - (self.start_time.minute - self.end_time.minute)
    
        duration_time : Time = Time(hour_duration, minute_duration)

        return duration_time

    @staticmethod
    def overlap(timeblock_1 : 'TimeBlock', timeblock_2 : 'TimeBlock') -> bool:
        """
        Static method that returns if two timeblocks overlap
        """

        if (timeblock_1.start_time >= timeblock_2.start_time and timeblock_1.start_time <= timeblock_2.end_time):
            return True
        
        if (timeblock_2.start_time >= timeblock_1.start_time and timeblock_2.start_time <= timeblock_1.end_time):
            return True

        if (timeblock_1.end_time <= timeblock_2.end_time and timeblock_1.end_time >= timeblock_2.start_time):
            return True
        
        if (timeblock_2.end_time <= timeblock_1.end_time and timeblock_2.end_time >= timeblock_1.start_time):
            return True

        return False
    
    def serialise(self) -> bytearray:
        serialised_data : bytearray = bytearray(4)

        serialised_data[0 : 2] = self.start_time.serialise()
        serialised_data[2 : 4] = self.end_time.serialise()

        return serialised_data

    def deserialise(timeblock_bytes : bytearray) -> 'TimeBlock':
        timeblock : TimeBlock = TimeBlock(Time.deserialise(timeblock_bytes[0 : 2]), Time.deserialise(timeblock_bytes[2 : 4]))

        return timeblock

    def __str__(self) -> str:
        return f"Start Time: {self.start_time} | End Time: {self.end_time} | Duration: {self.time_block_duration()}"


def main():
    time_block : TimeBlock = TimeBlock(Time(9,30), Time(17,0))
    time_block_2 : TimeBlock = TimeBlock(Time(17, 1), Time(22, 00))
    print(time_block)
    print(time_block_2)
    print(TimeBlock.overlap(time_block, time_block_2))


if __name__ == "__main__":
    main()