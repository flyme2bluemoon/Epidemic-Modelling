#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define BUFFERSIZE 100

typedef struct {
    int x_position;
    int y_position;
} Position;

typedef struct {
    Position coordinates;
    int status;
    /*
        Status -1 | susceptable
        Status -2 | died
        Status -3 | recovered
        Status 0 - 14 | days since infection
    */
} Person;

void greet(void) {
    printf("Hello, world!\n");
    printf("Made with love by Matthew Shen (flyme2bluemoon)\n");
    printf("Wear a mask and stay save during the COVID-19 Pandemic\n");
    printf("\n================================================\n\n");
}

int get_int(char *prompt) {
    bool is_done = false;
    char input[BUFFERSIZE];
    int output;

    do {
        printf("%s ", prompt);
        is_done = true;
        strcpy(input, "");
        fgets(input, BUFFERSIZE, stdin);
        int length_of_input = strlen(input);
        for (int i = 0; i < length_of_input; i++) {
            if (!isdigit(input[i]) && input[i] != '\n' && input[i] != ' ' && input[i] != '\t') {
                is_done = false;
                break;
            }
        }
    } while (!is_done);

    output = atoi(input);

    return output;
}

int main(void) {
    // print greeting message
    greet();

    // seed random number generator
    time_t t;
    srand((unsigned) time(&t));

    // getting values from user
    int area = get_int("Area:");
    int population = get_int("Population:");

    // creating arrays
    int map[area][area];
    Person people[population];

    // initializing arrays
    for (int i = 0; i < area; i++) {
        for (int j = 0; j < area; j++) {
            map[i][j] = 0;
        }
    }

    for (int i = 0; i < population; i++) {
        people[i].coordinates.x_position = rand() % area;
        people[i].coordinates.y_position = rand() % area;
        people[i].status = -1;
    }

    // actual code
    for (int i = 0; i < area; i++) {
        for (int j = 0; j < area; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < population; i++) {
        printf("Person %d is at (%d, %d) and has the status code %d.\n", i, people[i].coordinates.x_position, people[i].coordinates.y_position, people[i].status);
    }

    return 0;
}