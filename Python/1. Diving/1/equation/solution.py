import sys 

"""
Корни квадратного уравнения
(все коэффициенты на входе должны давать 2 корня)
"""

a = int(sys.argv[1]) 
b = int(sys.argv[2]) 
c = int(sys.argv[3])

d = b * b - 4 * a * c

x1 = (-b + d**0.5) / (2 * a)
x2 = (-b - d**0.5) / (2 * a)

print(int(x1))
print(int(x2))
