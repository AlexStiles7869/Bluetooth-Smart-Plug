from abc import abstractmethod

class IllegalAnswer(Exception):
    """
    Exception created to get more helpful exception name
    """
    pass

class Question:
    def __init__(self, question : str):
        self.question = question

    @abstractmethod
    def ask_question(self) -> str:
        # To be implemented by all subclasses
        return