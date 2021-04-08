import json      # https://docs.python.org/3/library/json.html
import functools # https://docs.python.org/3/library/functools.html

def to_json(func):
	@functools.wraps(func)
	def inner():
		data = func()
		return json.dumps(data)
	return inner

"""
@to_json
def get_data():
  return {
    'data': 42
  }

print(get_data())  # вернёт '{"data": 42}'
"""
