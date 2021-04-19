import asyncio


storage = {}


class ClientServerProtocol(asyncio.Protocol):
    def connection_made(self, transport):
        print("connection_made:", transport)
        self.transport = transport

    def data_received(self, data):
        print("data_received:", data)
        resp = process_data(data.decode())
        self.transport.write(resp.encode())


def gather_data(key, data):
    result = ""
    for ts, val in data.items():
        result += f"{key} {val} {ts}\n"
    return result


def process_command(command):
    try:
        print(f" cmd: {command}")
        if len(command) == 0:
            raise ValueError("zero length command")
        if command[0] == "put":
            if len(command) != 4:
                raise ValueError("put: wrong length")
            key = command[1]
            val = command[2]
            ts = command[3]
            if key not in storage:
                storage[key] = dict() #[]
            storage[key][ts] = val
            # storage[key].append((val, ts))
            print(f" put: {key} {val} {ts} => storage {storage}")
            return "ok"
        elif command[0] == "get":
            if len(command) != 2:
                raise ValueError("get: wrong length")
            result = "ok\n"
            key = command[1]
            print(f" get: {key}")
            if key == "*":
                for k, data in storage.items():
                    chunk = gather_data(k, data)
                    result += chunk
                print(f" result: {ascii(result)}")
            elif key in storage:
                data = storage[key]
                result += gather_data(key, data)
            return result
        else:
            raise ValueError("unknown command")
    except ValueError as e:
        what = e.args[0]
        print(f" erroneous command: {what}")
        return "error\nwrong command\n"


def process_data(message):
    commands = message.strip().split("\n")
    # print(f"process_data: msg '{message}' commands {commands}")
    print(f"process_data: cmd {commands}")
    results = [process_command(item.split()) for item in commands]
    result = "\n".join(results) + "\n"
    print(f" RESULT: {ascii(result)}")
    return result


def run_server(host, port):
    loop = asyncio.get_event_loop()
    coro = loop.create_server(ClientServerProtocol, host, port)

    server = loop.run_until_complete(coro)

    try:
        loop.run_forever()
    except KeyboardInterrupt:
        pass

    server.close()
    loop.run_until_complete(server.wait_closed())
    loop.close()
