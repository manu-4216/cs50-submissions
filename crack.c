#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <math.h>

int main(int argc, string argv[])
{
    string hash;
    int hash_length;
    char salt[2];
    string password_attempt;
    long int i = 1;
    int max_hash_len = 5;
    int coefs_base_53[5] = {0, 0, 0, 0, 0};
    char word[6];
    int word_len;
    int index;
    int ascii_diff_uppercase = 64; // 'A' = 65
    int ascii_diff_lowercase = 70; // 97 - 26
     int ascii_diff;
    int j;
    bool invalid_try = false;
    long int n;
    long int max_i = pow(53, max_hash_len);
    
    if (argc == 2)
    {
        hash = argv[1];
        
        // Get the salt: first 2 characters of the hash
        strncpy(salt, hash, 2);
        
        // ASCII table: https://en.cppreference.com/w/cpp/language/ascii
        // 65=A 90=Z; 97=a 122=z 
        // => Normalize each letter: 1=A 25=Z; 26=a 51=z ; INTERVAL: [1-53]
        // => Normalize a group of letters in base 53: 
        //    Examples: 54=1x53 + 1 = AA; 109 = 2x53 + 3 = "BC" ;
        
        do
        {
            // Reset the word string
            word[0] = '\0';
            
            invalid_try = false;
            
            // printf("\n");
            
            word_len = 0;
            
            // Transform the number 'i' into base-53 coeficients.
            // For instance: 54=1x53 + 1 = AA; 109 = 2x53 + 3 = "BC" ;
            n = i;
            
            for (j = 0; j < max_hash_len; j++)
            {
                if (j != max_hash_len - 1)
                {
                    coefs_base_53[j] = n / (pow(53, max_hash_len - j - 1));
                    n = n - coefs_base_53[j] * pow(53, max_hash_len - j - 1);
                }
                else
                {
                    coefs_base_53[j] = n % 53;    
                }   
            }
            
            // Find out the length of the word (ignore the 0 characters in the beggining)
            for (j = 0; j < max_hash_len; j++) 
            {
                if (coefs_base_53[j] != 0) 
                {
                    word_len = max_hash_len - j;
                    break;
                }
            }
            
            // Form the word, letter by letter
            for (j = max_hash_len - word_len; j < max_hash_len; j++) 
            {                
                // If any of the coefs is 0, then no need to continue, because
                // 0, 0, 1 = A, but any conbination with 0 isn't valid, and skipped, 
                // for example: 0, 1, 0 = nothing.
                // This is because we can't use 0 to encode a letter, because we wiuln't be able to 
                // differenciate between 0, 00, 000, etc if they would match A, AA, AAA, etc.
                // But we can differenciate between 1, 11, 111 = A, AA, AAA, etc
                if (coefs_base_53[j] == 0) 
                {
                    invalid_try = true;
                    break;
                }
                
                if (coefs_base_53[j] >= 27) // 'A'
                {
                    ascii_diff = ascii_diff_lowercase;
                }
                else 
                {
                    ascii_diff = ascii_diff_uppercase;
                }
                
                index = j;
                char letter[2] = { (char)(coefs_base_53[index] + ascii_diff), '\0' };
                strncat(word, letter, 1);
            }
            
            // Skip this password_attempt, because it's invalid
            if (invalid_try)
            {
                i++;
                continue;
            }
                
            password_attempt = crypt(word, salt);
            
            // Increment i, that will be transformed into a new try 
            i++;
        }
        while (strcmp(password_attempt, hash) != 0 && (i < max_i));
        
        printf("%s\n", word);
        
        return 0;
    }
    else 
    {
        printf("You need to enter only one argument, the hash!");
        return 1;
    }
}
