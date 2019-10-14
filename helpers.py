from nltk.tokenize import sent_tokenize

def lists(listA, listB):
    itemsInBothAB = list()
    for itemA in listA:
        if itemA in listB and itemA not in itemsInBothAB:
            itemsInBothAB.append(itemA)

    return itemsInBothAB

def lines(a, b):
    """Return lines in both a and b"""
    listA = a.split('\n')
    listB = b.split('\n')

    return lists(listA, listB)

def sentences(a, b):
    """Return sentences in both a and b"""
    sentencesA = sent_tokenize(a)
    sentencesB = sent_tokenize(b)


    return lists(sentencesA, sentencesB)

def substrings(a, b, n):

    substringsA = list()
    for i in range(0,  len(a) - n + 1):
        substringsA.append(a[i:i+n])

    substringsB = list()
    for i in range(0,  len(b) - n + 1):
        substringsB.append(b[i:i+n])

    return lists(substringsA, substringsB)

