import time


class ClientError:
    pass


class Client:

    def __init__(self, addr, port, timeout=None):
        self.addr = addr
        self.port = port
        self.timeout = timeout

    def put(self, key, value, timestamp):
        timestamp = timestamp or int(time.time())

    def get(self, key):
        pass
