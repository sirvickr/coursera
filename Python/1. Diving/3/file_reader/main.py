from solution import FileReader

def read(filename):
    reader = FileReader(filename)
    text = reader.read()
    print(f"{filename} contains: '{text}'")
    print(type(reader))

def _main():
    read('not_exist_file.txt')

    with open('some_file.txt', 'w') as file:
        file.write('some text')

    read('some_file.txt')

if __name__ == '__main__':
    _main()
