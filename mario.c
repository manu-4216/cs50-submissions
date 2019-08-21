#include <cs50.h>
#include <stdio.h>


int main(void)
{
    int height;
    
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0);
    
    // Print out the pyramid
    for (int i = 1; i <= height; i++)
    {
        int nr_of_spaces = height - i;
        
        // Print spaces
        for (int j = 1; j <= nr_of_spaces; j++)
        {
            printf(" ");
        }
               
        // Print hashes
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }
        
        // Print 2 spaces
        printf("  ");
        
        // Print hashes
        for (int j = 1; j <= i; j++)
        {
            printf("#");
        }
        
        // Print a new line 
        printf("\n");  
    }
}
