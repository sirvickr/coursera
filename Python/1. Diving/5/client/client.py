import time
import socket
import operator


class ClientError(BaseException):
    """класс исключений клиента"""
    pass


class Client:

    def __init__(self, host, port, timeout=None):
        self.host = host
        self.port = port
        self.timeout = timeout
        self.connection = socket.create_connection((host, port), timeout)

    def put(self, key, value, timestamp = None):
        timestamp = timestamp or int(time.time())
        message = f"put {key} {value} {timestamp}\n"
        self.connection.sendall(message.encode())
        data = self.connection.recv(1024).decode()
        reply = data.split()
        # ошибка сервера
        if len(reply) == 0 or reply[0] != "ok":
            raise ClientError

    def get(self, key):
        message = f"get {key}\n"
        self.connection.sendall(message.encode())
        data = self.connection.recv(1024).decode()
        reply = data.split()
        # ошибка сервера
        if len(reply) == 0 or reply[0] != "ok":
            raise ClientError
        # нет данных
        if len(reply) == 1:
            return {}
        result = {}
        try:
            for index in range(1, len(reply), 3):
                metric = reply[index]
                value = float(reply[index + 1])
                ts = int(reply[index + 2])
                if metric not in result:
                    result[metric] = []
                result[metric].append((ts, value))
            for metric, pairs in result.items():
                pairs.sort(key=operator.itemgetter(0))
        except (IndexError, ValueError):
            raise ClientError
        return result
