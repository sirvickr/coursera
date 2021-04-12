import os
import csv

class CarBase:
    def __init__(self, car_type, brand, photo_file_name, carrying):
        self.car_type = car_type
        self.brand = brand
        self.photo_file_name = photo_file_name
        self.carrying = float(carrying)

    def __repr__(self):
        return f"{self.car_type} '{self.brand}' {self.photo_file_name}, {self.carrying}т: "

    def get_photo_file_ext(self):
        return os.path.splitext(self.photo_file_name)[1]


class Car(CarBase):
    def __init__(self, brand, photo_file_name, carrying, passenger_seats_count):
        super().__init__("car", brand, photo_file_name, carrying)
        self.passenger_seats_count = int(passenger_seats_count)

    def __repr__(self):
        return super().__repr__() + f"{self.passenger_seats_count} seats"


class Truck(CarBase):
    def __init__(self, brand, photo_file_name, carrying, body_whl):
        super().__init__("truck", brand, photo_file_name, carrying)
        whl_list = body_whl.split('x')
        if len(whl_list) == 3:
            try:
                self.body_length = float(whl_list[0])
            except Exception:
                self.body_length = 0.0
            try:
                self.body_width = float(whl_list[1])
            except Exception:
                self.body_width = 0.0
            try:
                self.body_height = float(whl_list[2])
            except Exception:
                self.body_height = 0.0
        else:
            self.body_length = 0.0
            self.body_width = 0.0
            self.body_height = 0.0

    def __repr__(self):
        return super().__repr__() + f"vol={self.get_body_volume()}"

    def get_body_volume(self):
        return self.body_length * self.body_width * self.body_height


class SpecMachine(CarBase):
    def __init__(self, brand, photo_file_name, carrying, extra):
        super().__init__("spec_machine", brand, photo_file_name, carrying)
        self.extra = extra

    def __repr__(self):
        return super().__repr__() + f"{self.extra}"


def get_car_list(csv_filename: str) -> list:
    car_list = []
    with open(csv_filename) as csv_fd:
        reader = csv.reader(csv_fd, delimiter=';')
        next(reader)  # пропускаем заголовок
        for row in reader:
            car = None
            if len(row) > 5:
                #0         1      2                      3                4         5         6
                #car_type, brand, passenger_seats_count; photo_file_name; body_whl; carrying; extra
                try:
                    extensions = [".jpg", ".jpeg", ".png", ".gif"]
                    car_type = row[0]
                    brand = row[1]
                    if len(brand) == 0:
                        continue
                    photo_file_name = row[3]
                    file_name_ext = os.path.splitext(photo_file_name)
                    file_name = file_name_ext[0]
                    extension = file_name_ext[1]
                    if len(file_name) == 0 or len(extension) == 0 or extension not in extensions:
                        continue
                    carrying = float(row[5])
                    if car_type == "car":
                        passenger_seats_count = int(row[2])
                        car = Car(brand, photo_file_name, carrying, passenger_seats_count)
                    elif car_type == "truck":
                        body_whl = row[4]
                        car = Truck(brand, photo_file_name, carrying, body_whl)
                    elif car_type == "spec_machine":
                        if len(row) > 6:
                            extra = row[6]
                            if len(extra) == 0:
                                continue
                            car = SpecMachine(brand, photo_file_name, carrying, extra)
                except ValueError:
                    continue
            if car:
                car_list.append(car)
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
