from cs50 import get_int

print("Provide a height between 1 and 8.")
while True:
    height = get_int("Pyramid height: ")
    if 1 <= height <= 8:
        break

for i in range(1, height + 1):
    print((" " * (height - i)) + ("#" * i) + ("  ") + ("#" * i))
