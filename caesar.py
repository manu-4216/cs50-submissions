from cs50 import get_string
from sys import argv

if len(argv) == 2:
    key = argv[1]
    if not key.isdigit():
        print('1')
else:
    print('Usage: python caesar.py k')
    exit(1)

key = int(key)
plaintext = get_string('plaintext: ')
print('ciphertext: ', end='')

ai = ord('a')
Ai = ord('A')

for c in plaintext:
    ci = ord(c)

    if c.isalpha():
        if c.islower():
            print(chr(ai + (ci + key - ai) % 26), end='')
        else:
            print(chr(Ai + (ci + key - Ai) % 26), end='')
    else:
        print(c, end='')



