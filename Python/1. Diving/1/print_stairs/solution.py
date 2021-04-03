import sys

lines_count = int(sys.argv[1])
for n in range(lines_count):
	print(' ' * (lines_count - i - 1), '#' * (i + 1), sep="")
