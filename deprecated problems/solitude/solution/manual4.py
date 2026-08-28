print(1)
print(5000)

a = []
a += [1920]
a += [1922] * 3

for x in range(1924, 3083, 2):
    a += [x] * 2

for x in range(3084, 5001, 2):
    a += [x] * 4

print(*a)