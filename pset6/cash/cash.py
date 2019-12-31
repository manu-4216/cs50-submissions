from cs50 import get_float

def main():
    change_owed_cents = getChange()
    searchSolution(change_owed_cents)

def getChange():
    while True:
        change_owed_dollars = get_float("Change owed: ")
        if  change_owed_dollars > 0:
            break

    # Return the value transformed in cents
    return round(change_owed_dollars * 100)


def searchSolution(z):
    new_change = z
    solution_found = False
    coin_index = -1
    # Counts the total nr of coins used for the greedy solution (all values combined)
    total_coins_number = 0
    coin_values = [25, 10, 5, 1]

    while True:
        coin_index += 1
        coin_value = coin_values[coin_index]
        max_coin_number = new_change // coin_value
        total_coins_number += max_coin_number

        # Avoid division by zero error
        if max_coin_number != 0:
            new_change = new_change % (max_coin_number * coin_value)
            if new_change == 0:
                solution_found = True

        if solution_found:
            break

    if solution_found == False:
        print("Solution could not be found.")
    else:
        print(total_coins_number)


if __name__ == '__main__':
    main()