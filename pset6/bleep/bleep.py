from cs50 import get_string
from sys import argv


def main():
    if len(argv) != 2:
        print('Usage: python bleep.py dictionary')
        exit(1)
    else:
        fileName = argv[1]

        with open(fileName, 'r') as file:
            badWords = file.read().split('\n')[:-1]

        string = get_string('What message would you like to censor?\n')
        words = string.split(' ')

        cleanWords = list()

        for word in words:
            lword = word.lower()
            if lword not in badWords:
                cleanWords.append(word)
            else:
                cleanWords.append('*'*len(word))

        print(' '.join(cleanWords))


if __name__ == "__main__":
    main()
