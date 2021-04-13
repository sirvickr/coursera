class Value:

    def __init__(self, value=None):
        self.value = value or 0

    def __get__(self, instance, owner):
        return self.value

    def __set__(self, instance, value):
        self.value = value - instance.commission * value
