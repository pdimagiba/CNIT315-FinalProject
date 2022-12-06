//  CNIT 315 Final Project
//  Phoenix Dimagiba, pdimagib@purdue.edu
//  
//
//  Project description
//  

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

//function prototypes
char getAction();
void getLocal();
void getCity();
int system(const char *command);

int main()
{
    puts("Welcome to Weather Wizard!");
    //proceeds based on choice selected in getAction
    while (1)
    {
        char uInput = getAction();
        switch(uInput)
        {
            case '1':
                getLocal();
                break;
            case '2':
                getCity();
                break;
            case 'A':
                printf("Farewell.\n");
                return 0;
            default:
                printf("Error: Option not recognized.\n\n");
                break;
        }
    }
    return 0;
}

char getAction()
{
    char uInput;
    
    //display options
    printf("1. Get local weather only\n");
    printf("2. Get specific city/cities' weather\n");
    printf("A. Exit program\n");
    //get and return user's action
    printf("Enter an option: ");
    uInput = getchar();
    fflush(stdin);
    
    return uInput;
}

void getLocal()
{
    int choice;
    puts("\n[Local Weather]");
    printf("Select 1 for detailed and 2 for simple: ");
    scanf("%i", &choice);

    if(choice == 1)
    {
        system("curl wttr.in");
        putchar('\n');
        putchar('\n');
    }
    else if(choice == 2)
    {
        system("curl wttr.in?format=\"%l:+%C+%t\n\"");
        putchar('\n');
        putchar('\n');
    }
    else
        puts("Invalid option selected.");

    fflush(stdin);
}

void getCity()
{
    char choice[50];
    char command[60] = "curl wttr.in/";
    const char s[2] = ",";
    char *token;
    int detail;
    
    puts("\n[Weather by City]");
    puts("To enter more than one city, separate with commas (London,Singapore,Paris).");
    puts("To enter multiple-word cities, separate words with + (San+Diego).");
    printf("Enter city or cities: ");
    scanf("%s", &choice);
    printf("Select 1 for detailed and 2 for simple: ");
    scanf("%d", &detail);

    if(detail == 1)
    {
        token = strtok(choice, s);
        
        /* walk through other tokens */
        while( token != NULL )
        {
            strcat(command, token);
            system(command);
            putchar('\n');
            //printf("%s", command);
            (void)strncpy(command, "curl wttr.in/", sizeof(command));
            token = strtok(NULL, s);
            //printf("%s", token);
        }
    }
    else if(detail == 2)
    {
        token = strtok(choice, s);

        while( token != NULL )
        {
            strcat(command, token);
            //printf("%s", command);
            strcat(command, "?format=\"%l+\%C+%t\n\n");
            //printf("%s", command);
            system(command);
            putchar('\n');
            (void)strncpy(command, "curl wttr.in/", sizeof(command));
            token = strtok(NULL, s);
        }
        putchar('\n');
    }
    else
        puts("Invalid option selected.");

    fflush(stdin);
}
