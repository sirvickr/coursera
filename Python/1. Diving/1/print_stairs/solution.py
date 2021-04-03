import sys

lines_count = int(sys.argv[1])
for n in range(1, lines_count + 1):
	print(' ' * (lines_count - n) + '#' * n)
