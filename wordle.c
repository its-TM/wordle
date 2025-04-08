#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

// Definitions for word length and list size
#define WORDSIZE 5
#define MAX_WORDS_PER_LINE 100
#define MAX_LINES 26          // One line for each letter of the alphabet

// Scoring values
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes
#define GREEN "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET "\e[0;39m"

// Function prototypes
char* get_guess(void);
int check_word(const char* guess, int status[], const char* choice);
void print_word(const char* guess, const int status[]);
void to_lower_case(char* str);

int main(void)
{
    // Open the file
    FILE* wordlist = fopen("wordle.txt", "r");
    if (!wordlist)
    {
        printf("Error: Could not open wordle.txt.\n");
        return 1;
    }

    // Declare storage for words
    char words[MAX_LINES][MAX_WORDS_PER_LINE][WORDSIZE + 1];
    int word_count[MAX_LINES] = {0};

    // Read the file line by line
    char line[1024];
    int line_index = 0;

    while (fgets(line, sizeof(line), wordlist) && line_index < MAX_LINES)
    {
        char* token = strtok(line, " \n");
        while (token && word_count[line_index] < MAX_WORDS_PER_LINE)
        {
            if (strlen(token) == WORDSIZE)
            {
                to_lower_case(token);
                strcpy(words[line_index][word_count[line_index]], token);
                word_count[line_index]++;
            }
            token = strtok(NULL, " \n");
        }
        line_index++;
    }
    fclose(wordlist);

    // Check if any words were loaded
    if (line_index == 0)
    {
        printf("Error: No valid words found in wordle.txt.\n");
        return 1;
    }

    // Randomly select a word from a non-empty line
    srand(time(NULL));
    int random_line;
    do {
        random_line = rand() % MAX_LINES;
    } while (word_count[random_line] == 0);

    int random_word_index = rand() % word_count[random_line];
    char* choice = malloc(WORDSIZE + 1);
    if (!choice)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }
    strcpy(choice, words[random_line][random_word_index]);

    // Print greeting
    printf(GREEN "Welcome to Wordle!\n" RESET);
    printf("Guess the %d-letter word. You have %d attempts.\n\n", WORDSIZE, WORDSIZE + 1);

    // Game loop
    int guesses = WORDSIZE + 1;
    bool won = false;

    for (int i = 0; i < guesses; i++)
    {
        char* guess = get_guess();
        if (!guess)
        {
            printf("Error getting guess.\n");
            free(choice);
            return 1;
        }

        to_lower_case(guess);

        int status[WORDSIZE] = {0};
        int score = check_word(guess, status, choice);

        printf("Guess %d: ", i + 1);
        print_word(guess, status);

        if (score == WORDSIZE * EXACT)
        {
            printf("Congratulations! You guessed the word!\n");
            won = true;
            free(guess);
            break;
        }

        free(guess);
    }

    if (!won)
    {
        printf("Sorry, you're out of guesses. The word was: %s\n", choice);
    }

    free(choice);
    return 0;
}

char* get_guess(void)
{
    char* guess = malloc(WORDSIZE + 1);
    if (!guess)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    while (true)
    {
        printf("Enter a %d-letter word: ", WORDSIZE);
        
        if (fgets(guess, WORDSIZE + 2, stdin) == NULL)
        {
            printf("Error reading input. Try again.\n");
            continue;
        }

        // Remove newline if present
        guess[strcspn(guess, "\n")] = '\0';
        
        // Clear input buffer if needed
        if (strlen(guess) > WORDSIZE)
        {
            while (getchar() != '\n');
            printf("Invalid input. Please enter exactly %d letters.\n\n", WORDSIZE);
            continue;
        }

        // Validate input is alphabetic
        bool valid = true;
        for (int i = 0; i < strlen(guess); i++)
        {
            if (!isalpha(guess[i]))
            {
                valid = false;
                break;
            }
        }

        if (strlen(guess) == WORDSIZE && valid)
        {
            break;
        }
        else
        {
            printf("Invalid input. Please enter exactly %d letters (a-z).\n\n", WORDSIZE);
        }
    }
    return guess;
}

int check_word(const char* guess, int status[], const char* choice)
{
    int score = 0;
    bool matched[WORDSIZE] = {false};

    // First pass: exact matches
    for (int i = 0; i < WORDSIZE; i++)
    {
        if (guess[i] == choice[i])
        {
            status[i] = EXACT;
            matched[i] = true;
            score += EXACT;
        }
    }

    // Second pass: close matches
    for (int i = 0; i < WORDSIZE; i++)
    {
        if (status[i] == EXACT) continue;
        
        for (int j = 0; j < WORDSIZE; j++)
        {
            if (!matched[j] && guess[i] == choice[j])
            {
                status[i] = CLOSE;
                matched[j] = true;
                score += CLOSE;
                break;
            }
        }
    }
    return score;
}

void print_word(const char* guess, const int status[])
{
    for (int i = 0; i < WORDSIZE; i++)
    {
        if (status[i] == EXACT)
        {
            printf(GREEN " %c " RESET, toupper(guess[i]));
        }
        else if (status[i] == CLOSE)
        {
            printf(YELLOW " %c " RESET, toupper(guess[i]));
        }
        else
        {
            printf(RED " %c " RESET, toupper(guess[i]));
        }
    }
    printf("\n\n");
}

void to_lower_case(char* str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}