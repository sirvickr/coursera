import os
import csv

class CarBase():
    """базовый класс для транспортных средств"""

    # атрибут для хранения обязательных параметров класса, описывающего транспортное средство
    required = []

    def __init__(self, brand, photo_file_name, carrying):
        self.brand = self.validate_input(brand)
        self.photo_file_name = self.validate_photo_filename(photo_file_name)
        self.carrying = float(self.validate_input(carrying))

    def __repr__(self):
        return f"{self.car_type} '{self.brand}' {self.photo_file_name}, {self.carrying}т: "

    def validate_input(self, value):
        """метод валидации данных, возвращает значение, если оно валидно,
        иначе выбрасывает исключение ValueError"""
        if value == '':
            raise ValueError
        return value

    def validate_photo_filename(self, filename):
        for ext in ('.jpg', '.jpeg', '.png', '.gif'):
            if filename.endswith(ext) and len(filename) > len(ext):
                return filename
        raise ValueError

    def get_photo_file_ext(self):
        _, ext = os.path.splitext(self.photo_file_name)
        return ext

    @classmethod
    def create_from_dict(cls, data):
        """создает экземпляр класса из словаря с параметрами"""
        parameters = [data[parameter] for parameter in cls.required]
        return cls(*parameters)


class Car(CarBase):
    """класс описывающий автомобили для перевозок людей"""
    car_type = "car"
    required = ['brand', 'photo_file_name', 'carrying', 'passenger_seats_count']

    def __init__(self, brand, photo_file_name, carrying, passenger_seats_count):
        super().__init__(brand, photo_file_name, carrying)
        self.passenger_seats_count = int(self.validate_input(passenger_seats_count))

    def __repr__(self):
        return super().__repr__() + f"{self.passenger_seats_count} seats"


class Truck(CarBase):
    """класс описывающий автомобили для перевозок грузов"""
    car_type = "truck"
    required = ['brand', 'photo_file_name', 'carrying', 'body_whl']

    def __init__(self, brand, photo_file_name, carrying, body_whl):
        super().__init__(brand, photo_file_name, carrying)
        self.body_length, self.body_width, self.body_height = self.parse_whl(body_whl)

    def __repr__(self):
        return super().__repr__() + f"vol={self.get_body_volume()}"

    def get_body_volume(self):
        """возвращает объем кузова"""
        return self.body_length * self.body_width * self.body_height

    def parse_whl(self, body_whl):
        """возвращает реальные размеры кузова length, width, height"""
        try:
            length, width, height = (float(c) for c in body_whl.split("x", 2))
        except Exception:
            length, width, height = 0.0, 0.0, 0.0
        return length, width, height


class SpecMachine(CarBase):
    """класс описывающий спецтехнику"""

    car_type = "spec_machine"
    required = ['brand', 'photo_file_name', 'carrying', 'extra']

    def __init__(self, brand, photo_file_name, carrying, extra):
        super().__init__(brand, photo_file_name, carrying)
        self.extra = self.validate_input(extra)

    def __repr__(self):
        return super().__repr__() + f"{self.extra}"


def get_car_list(csv_filename):
    """возвращает список объектов, сохраненных в файле csv_filename"""

    car_types = {'car': Car, 'spec_machine': SpecMachine, 'truck': Truck}
    csv.register_dialect('cars', delimiter=';')
    car_list = []

    with open(csv_filename, 'r') as file:
        reader = csv.DictReader(file, dialect='cars')
        for row in reader:
            try:
                car_class = car_types[row['car_type']]
                car_list.append(car_class.create_from_dict(row))
            except Exception:
                pass

    return car_list


if __name__ == "__main__":
    pass
    car_list = get_car_list("valid.csv")
    print(f"car_list ({len(car_list)} items):", *car_list, sep='\n')
    # car = Car('Bugatti Veyron', 'bugatti.png', '0.312', '2')
    # print(car.car_type, car.brand, car.photo_file_name, car.carrying, car.passenger_seats_count, sep = '\n')
    #
    # truck = Truck('Nissan', 'nissan.jpeg', '1.5', '3.92x2.09x1.87')
    # print(truck.car_type, truck.brand, truck.photo_file_name, truck.body_length, truck.body_width, truck.body_height, sep='\n')
    #
    # spec_machine = SpecMachine('Komatsu-D355', 'd355.jpg', '93', 'pipelayer specs')
    # print(spec_machine.car_type, spec_machine.brand, spec_machine.carrying, spec_machine.photo_file_name, spec_machine.extra, sep='\n')
    # print(spec_machine.get_photo_file_ext())
    #
    # cars = get_car_list('cars.csv')
    # print(len(cars))
    # for car in cars:
    #     print(type(car))
    # print(cars[0].passenger_seats_count)
    # print(cars[1].get_body_volume())
