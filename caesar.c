#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <wctype.h>

int main(int argc, string argv[])
{
    long int key;
    int n;
    int i;
    string plaintext;
    
    if (argc == 2)
    {
        key = atoi(argv[1]);
        plaintext = get_string("plaintext: ");
        
        printf("ciphertext: ");
        
        n = strlen(plaintext);
        for (i = 0; i < n; i++)
        {
            if (iswlower(plaintext[i]) != 0)
            {
                printf("%c", (char)('a' + (plaintext[i] + key - 'a') % 26));
            } 
            else if (iswupper(plaintext[i]) != 0)
            {
                printf("%c", (char)('A' + (plaintext[i] + key - 'A') % 26));
            }
            else
            {
                printf("%c", plaintext[i]);
            }
        }
        
        printf("\n");
        
        return 0;
    }
    else
    {
        printf("Usage: ./caesar key");
        return 1;
    }
}
