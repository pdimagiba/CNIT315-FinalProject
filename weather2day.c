//  CNIT 315 Final Project
//  Phoenix Dimagiba, pdimagib@purdue.edu
//  Weather2Day
//
//  Weather2Day allows the user to check local or specific location weather in detailed or simple form.
//  The user can also choose whether or not to store the weather data in a queue as well as perform basic queue actions.

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>

struct Node
{
    char *locationName;
    char *weatherCondition;
    char *actualTemp;
    char *feelTemp;
    struct Node *nextPtr;
};

typedef struct Node node_t;

node_t *qHeadPtr = NULL;

//function prototypes
int system(const char *command);
char getAction();
void getLocal();
void getCity();
void queueEnqueue(char *);
void *queueDequeue();
node_t *createNode(char *);
node_t *deleteFront();
int countNodes();
void queueClear();
int clearList();
int queueIsEmpty();
void printNode(node_t *);
void printNodes();

int main()
{
    puts("Welcome to Weather2Day!");
    puts("Powered by wttr.in");
    
    //infinite loop to display menu
    while (1)
    {
        char uInput = getAction();
        //proceeds based on choice selected in getAction
        switch(uInput)
        {
            case '1':
                getLocal();
                break;
            case '2':
                getCity();
                break;
            case '3':
                printNodes();
                break;
            case '4':
                queueDequeue();
                break;
            case '5':
                queueClear();
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
    printf("2. Get specific location/locations' weather\n");
    printf("3. Display stored location weather entries.\n");
    printf("4. Delete a node from the weather entries queue.\n");
    printf("5. Remove all of the nodes from the weather entries queue.\n");
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
        //api call with no specification
        system("curl wttr.in");
        putchar('\n');
        putchar('\n');
    }
    else if(choice == 2)
    {
        puts("The information is provided as follows: [Location]: [Weather Condition] [Actual Temperature] [Feels-Like Temperature]");
        //api call with simplified output
        system("curl wttr.in?format=\"%l:+\%C+%t+\%f\n\"");
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
    char qAdd;
    
    puts("\n[Weather by Location]");
    puts("To enter more than one location, separate with commas (Lafayette,Rensselaer,Singapore).");
    puts("To enter multiple-word locations, separate words with + (Lake+Orion).");
    puts("You may also enter 3-letter airport codes (SAN) or special locations with a preceding ~ (~White+House)");
    printf("Enter location or locations: ");
    scanf("%s", &choice);
    printf("Select 1 for detailed and 2 for simple: ");
    scanf("%d", &detail);

    if(detail == 1)
    {
        //gets first location from entered string
        token = strtok(choice, s);
        /* walk through tokens */
        while( token != NULL )
        {
            //adds location to start of api call
            strcat(command, token);
            system(command);
            putchar('\n');
            //resets base commmand to start of api call
            (void)strncpy(command, "curl wttr.in/", sizeof(command));
            fflush(stdin);
            //prompts user to add data to queue
            printf("Would you like to save this update to the queue? (Y/N): ");
            qAdd = getchar();            
            //ternary op: proceeds to enqueue if yes is selected
            (qAdd == 'Y' || qAdd == 'y') ? queueEnqueue(token) : puts("Proceeding without saving.");
            //proceeds to next token
            token = strtok(NULL, s);
            //printf("%s", token);
        }
    }
    else if(detail == 2)
    {
        //gets first location from entered string
        token = strtok(choice, s);
        puts("The information is formatted as follows: [Location]: [Weather Condition] [Actual Temperature] [Feels-Like Temperature]");
        //walk through tokens
        while( token != NULL )
        {
            //adds location to start of api call
            strcat(command, token);
            //continues building api call by specifying formatting
            strcat(command, "?format=\"%l:+\%C+%t+\%f\n\n");
            system(command);
            putchar('\n');
            //resets base commmand to start of api call
            (void)strncpy(command, "curl wttr.in/", sizeof(command));
            fflush(stdin);
            //prompts user to add data to queue
            printf("Would you like to save this update to the queue? (Y/N): ");
            qAdd = getchar();
            //ternary op: proceeds to enqueue if yes is selected           
            (qAdd == 'Y' || qAdd == 'y') ? queueEnqueue(token) : puts("Proceeding without saving.");
            //proceeds to next token
            token = strtok(NULL, s);
        }
        putchar('\n');
    }
    else
        puts("Invalid option selected.");

    fflush(stdin);
}

void queueEnqueue(char *newCity)
{
    fflush(stdin);
    
    node_t *tempPtr = qHeadPtr;
    //if queue has no nodes
    if (!tempPtr)
    {
        // create a new node, set queue headPtr to point to the node
        qHeadPtr = createNode(newCity);
    }
    else
    {
       //moves to last node, sets last node's next pointer to new node
        while (tempPtr)
        {
            if (!tempPtr->nextPtr)
            {
                node_t *newNode = createNode(newCity);
                tempPtr->nextPtr = newNode;
                break;
            }
            tempPtr = tempPtr->nextPtr;
        }
    }
}

void *queueDequeue()
{
    //topmost node deleted from queue
    node_t *tempPtr = deleteFront();
    printf("Topmost node deleted from queue.\n\n");
    free(tempPtr);
}

node_t *createNode(char *tempName)
{   
    //node memory allocation
    node_t *newNode = malloc(sizeof(node_t));
    char *locationNameArr = malloc(31 * sizeof(char));
    char *weatherConditionArr = malloc(31 * sizeof(char));
    char *actualTempArr = malloc(31 * sizeof(char));
    char *feelTempArr = malloc(31 * sizeof(char));

    //empties junk data from locationNameArr and fills it with token
    memset(locationNameArr, 0, strlen(locationNameArr));
    strcat(locationNameArr, tempName);

    //get values with f gets
    puts("Please enter the following weather data manually!");
    printf("Enter the city's weather condition: ");
    fgets(weatherConditionArr, 31, stdin);
    printf("Enter the city's actual temperature: ");
    fgets(actualTempArr, 31, stdin);
    printf("Enter the city's feels-like temperature: ");
    fgets(feelTempArr, 31, stdin);
    
    //set node values to inputted values
    newNode->locationName = &locationNameArr[0];
    newNode->weatherCondition = &weatherConditionArr[0];
    newNode->actualTemp = &actualTempArr[0];
    newNode->feelTemp = &feelTempArr[0];
    newNode->nextPtr = NULL;
    return newNode;
}

node_t *deleteFront()
{
    node_t *tempPtr = qHeadPtr;

    //if the node not empty, remove the node front of queue and return the pointer
    if (!queueIsEmpty())
    {
        node_t *dequeuePtr = qHeadPtr;
        qHeadPtr = qHeadPtr->nextPtr;
        return dequeuePtr;
    }
    //if queue is empty note this and return NULL
    else
    {
        printf("The queue is empty.\n\n");
        return NULL;
    }
    
}

int queueIsEmpty()
{
    //returns 1 if the queue has nodes
    return countNodes() ? 0 : 1;
}

int countNodes()
{
    node_t *tempPtr = qHeadPtr;

    //increments counter variable for each node
    int i = 0;
    while (tempPtr != NULL)
    {
        tempPtr = tempPtr->nextPtr;
        i++;
    }
    return i;
}

void queueClear()
{
    //calls the clearList() function in queue mode
    if (clearList())
        printf("Queue cleared!\n\n");
}

int clearList()
{
    //int selection = select == 's';
    node_t *tempPtr = qHeadPtr;

    //finds nodeCount of stack or queue depending on selection
    if (countNodes())
    {
        //frees the memory of every node in either list starting with the last node in the list until there is only one node remaining
        int n, i;
        for (n = (countNodes() - 1); n > 0; n--)
        {
            i = 0;
            node_t *travPtr = tempPtr;
            while (i != n - 1)
            {
                travPtr = travPtr->nextPtr;
                i++;
            }
            free(travPtr->nextPtr);
        }

        //free memory of the last node, set the corresponding HeadPtr to NULL
        free(tempPtr);
        qHeadPtr = NULL;
        return 1;
    }
    //queue empty already
    else
    {
        printf("The queue is already empty.\n\n");
        return 0;
    }
}

void printNode(node_t *nodePtr)
{
    //prints node contents
    printf("Memory Address: %p\n", nodePtr);
    printf("City: %s\n", nodePtr->locationName);
    printf("Weather Condition: %s", nodePtr->weatherCondition);
    printf("Actual Temperature: %s", nodePtr->actualTemp);
    printf("Feel-like Temperature: %s", nodePtr->feelTemp);
    printf("Next Node Memory Address: %p\n\n", nodePtr->nextPtr);
}

void printNodes()
{
    node_t *tempPtr = qHeadPtr;

    //finds number of nodes in list depending on the selection
    int nodeCount = countNodes();
    printf("There is/are %d node(s) in the queue.\n\n", nodeCount);

    if (nodeCount)
    {
        //traverse queue, print each node
        printf("[Queue Front]\n\n");
        while (tempPtr)
        {
            printNode(tempPtr);
            tempPtr = tempPtr->nextPtr;
        }
        printf("[Queue Back]\n\n");
    }
    else
        return;
}