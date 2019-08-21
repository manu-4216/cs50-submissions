/**
 * Finds the greedy solution for a given change value.
 */
 
#include <stdio.h>
#include <cs50.h>
#include <math.h>

int getChange(void);
void searchSolution(int z);

int main(void)
{
    
    int change_owed_cents = getChange();
    searchSolution(change_owed_cents);
}


int getChange(void)
{
    float change_owed_dollars;
   
    do
    {
        change_owed_dollars = get_float("Change owed: ");   
    }
    while (change_owed_dollars <= 0);
    
    // Return the value transformed in cents
    return round(change_owed_dollars * 100);
}

void searchSolution(int z)
{
    int new_change = z; 
    bool solution_found = false;
    int coin_index = -1; 
    int coin_value;
    // Counts the nr of coins of the same value
    int max_coin_number;
    // Counts the total nr of coins used for the greedy solution (all values combined)
    int total_coins_number = 0;
    int coin_values[4] = {25, 10, 5, 1}; 
    
    do
    {
        coin_index += 1; 
        coin_value = coin_values[coin_index];
        max_coin_number = new_change / coin_value;
        total_coins_number += max_coin_number;

        // Avoid division by zero error */
        if (max_coin_number != 0)
        {
            new_change = new_change % (max_coin_number * coin_value);
            if (new_change == 0)
            {
                solution_found = true; 
            }
        }      
    } 
    while (solution_found == false); 
    
    if (solution_found == false)
    {
        printf("Solution could not be found.\n");
    }
    else 
    {
        printf("%d\n", total_coins_number);
    }
}
