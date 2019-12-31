while True:
    height = input('Height: ')
    if not height.isdigit():
        continue
    height = int(height)
    if height > 0 and height < 9:
        break

# Repetition operator:
# https://www.w3schools.in/python-tutorial/repeat-string-in-python/
for i in range(height):
    print(' ' * (height - i - 1) + '#' * (i + 1))