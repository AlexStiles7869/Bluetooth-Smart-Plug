from Question.Question import Question, IllegalAnswer
from Question.Options import OptionsContainer
from typing import List

class RestrictedQuestion(Question):
    EXIT_STRING : str = "EXIT"
    OPTION_STRING : str = "OPT"

    def __init__(self, question : str, valid_answers : OptionsContainer):
        # Call the super class's constructor
        super().__init__(question)
        self.valid_answers : OptionsContainer = valid_answers
        self.valid_answer_values : List[int] = self.valid_answers.option_values()

    def ask_question(self) -> int:
        answer = None

        while answer is None:
            try:
                user_input : str = input(self.question)

                # Check if user response is a special command
                if user_input == RestrictedQuestion.EXIT_STRING:
                    # Return -1 indicating exit string was passed
                    return -1
                elif user_input == RestrictedQuestion.OPTION_STRING:
                    # List all the options, then return to start and ask question again
                    self.list_options()
                    # Reask the question (go to top of while loop, could've made next if statement else if, preferred this way)
                    continue

                # Check if user response is in the range of valid inputs
                if int(user_input) in self.valid_answer_values:
                    # Set the answer
                    answer = int(user_input)
                else:
                    raise IllegalAnswer()

            except IllegalAnswer:
                print(f"Your answer was not within the allowed range of {self.valid_answer_values[0]} and {self.valid_answer_values[-1]}.")
            except ValueError:
                print("Your answer was not an integer.")
            except Exception:
                print("There was an unknown error with your answer. Please try again")

        return answer