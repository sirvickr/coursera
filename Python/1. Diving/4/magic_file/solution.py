import os
import tempfile

class File:
    """Класс для работы с файлами"""

    def __init__(self, path):
        """ В конструктор передается полный путь до файла на файловой системе.
        Если файла с таким путем не существует, он должен быть создан при инициализации. """
        self.path = path
        if not os.path.exists(self.path):
            with open(self.path, "w") as f:
                pass

    def __str__(self):
        """ Возвращает полный путь до файла """
        return self.path

    def __add__(self, other):
        """ Сложение объектов типа File, результатом сложения является объект класса File, при этом создается
        новый файл и файловый объект, в котором содержимое второго файла добавляется к содержимому первого файла.
        Новый файл должен создаваться в директории, полученной с помощью функции tempfile.gettempdir """
        _, a = os.path.split(self.path)
        _, b = os.path.split(other.path)
        obj = File(os.path.join(tempfile.gettempdir(), a + b))
        obj.write(self.read() + other.read())
        return obj

    def __getitem__(self, index):
        """Поддерживает протокол итерации, причем итерация проходит по строкам файла"""
        lines = []
        with open(self.path) as f:
            lines = f.readlines()
        return lines[index]

    def read(self):
        """Возвращает строку с текущим содержанием файла"""
        with open(self.path) as f:
            return f.read()

    def write(self, text):
        """Принимает в качестве аргумента строку с новым содержанием файла"""
        with open(self.path, "w") as f:
            return f.write(text)
