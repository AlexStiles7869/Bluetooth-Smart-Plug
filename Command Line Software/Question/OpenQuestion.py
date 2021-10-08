from typing import Callable

from Question.Question import Question, IllegalAnswer

class OpenQuestion(Question):
    def __init__(self, question : str, validity_func : Callable, error_msg : str):
        # Call the super class's constructor
        super().__init__(question)
        self.validity_func = validity_func
        self.error_msg = error_msg
    
    def ask_question(self) -> str:
        answer = None

        while answer is None:
            try:
                user_input : str = input(self.question)

                if (self.validity_func(user_input)):
                    answer = user_input
                else:
                    raise IllegalAnswer()
            except IllegalAnswer:
                print(self.error_msg)
        
        return answer

    