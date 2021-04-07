import argparse # https://docs.python.org/3/howto/argparse.html
import tempfile # https://docs.python.org/3/library/tempfile.html
import json     # https://docs.python.org/3/library/json.html
import os       # https://docs.python.org/3.3/library/os.html

def read_data(storage_path):
	if os.path.exists(storage_path):
		with open(storage_path, "r") as file:
			raw_data = file.read()
			data = None
			if raw_data:
				data = json.loads(raw_data)
			return data

def write_data(storage_path, data):
	with open(storage_path, "w") as file:
		file.write(json.dumps(data))

dir = tempfile.gettempdir();
storage_path = os.path.join(dir, 'storage.data')
db = read_data(storage_path) or dict()

parser = argparse.ArgumentParser()
parser.add_argument("--key")
parser.add_argument("--val")

args = parser.parse_args()

if args.val: # add val to [key]
	if args.key in db:
		db[args.key].append(args.val)
	else:
		db[args.key] = [args.val]
	write_data(storage_path, db)

elif args.key: # show [key] values
	if args.key in db:
		print(", ".join(db[args.key]))
	else:
		print("")

else:
	pass
