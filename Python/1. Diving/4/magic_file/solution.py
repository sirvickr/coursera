import os
import uuid

class File:
    """Класс для работы с файлами"""

    def __init__(self, path):
        """ В конструктор передается полный путь до файла на файловой системе.
        Если файла с таким путем не существует, он должен быть создан при инициализации. """
        self.path = path
        self.current_position = 0
        if not os.path.exists(self.path):
            open(self.path, 'w').close()

    def __str__(self):
        """ Возвращает полный путь до файла """
        return self.path

    def __add__(self, other):
        """ Сложение объектов типа File, результатом сложения является объект класса File, при этом создается
        новый файл и файловый объект, в котором содержимое второго файла добавляется к содержимому первого файла.
        Новый файл должен создаваться в директории, полученной с помощью функции tempfile.gettempdir """
        new_path = os.path.join(
            os.path.dirname(self.path),
            str(uuid.uuid4().hex)
        )
        new_file = type(self)(new_path)
        new_file.write(self.read() + other.read())
        return new_file

    def __iter__(self):
        return self

    def __next__(self):
        with open(self.path, 'r') as f:
            f.seek(self.current_position)
            line = f.readline()
            if not line:
                self.current_position = 0
                raise StopIteration('EOF')
            self.current_position = f.tell()
            return line

    def read(self):
        """Возвращает строку с текущим содержанием файла"""
        with open(self.path, 'r') as f:
            return f.read()

    def write(self, text):
        """Принимает в качестве аргумента строку с новым содержанием файла"""
        with open(self.path, "w") as f:
            return f.write(text)
