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
        Status 1 - 14 | days since infection
        Status 0 | Invalid (no person)
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

void print_map(int width, int map[width][width]) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    return;
}

void move_people(int range, int width, int population, int map[width][width], Person people[population]) {
    for (int i = 0; i < population; i++) {
        int dx = (rand() % (range * 2 + 1)) - range;
        int dy = (rand() % (range * 2 + 1)) - range;
        if (people[i].coordinates.x_position + dx >= width || people[i].coordinates.x_position + dx < 0 || people[i].coordinates.y_position >= width || people[i].coordinates.y_position + dx < 0) {
            i--;
        } else {
            map[people[i].coordinates.x_position][people[i].coordinates.y_position] = 0;
            people[i].coordinates.x_position += dx;
            people[i].coordinates.y_position += dy;
            map[people[i].coordinates.x_position][people[i].coordinates.y_position] = people[i].status;
        }
    }

    return;
}

int main(void) {
    // print greeting message
    greet();

    srand(time(NULL));

    // getting values from user
    int width = get_int("Width (recommended: 10-100):");
    int population = get_int("Population (requirement: < width * width):");
    int days = get_int("Numbers of days (recommended: 30):");
    int range = get_int("Range of movements (recommended: 2):");

    if (population > width * width) {
        printf("Population must be less than the area.\n");
        return 1;
    } else if (range > width) {
        printf("Range must be less than the width.\n");
    }

    printf("\nRunning Simulation!\n\n");

    // creating arrays
    int map[width][width];
    Person people[population];

    // initializing arrays
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            map[i][j] = 0;
        }
    }

    for (int i = 0; i < population; i++) {
        people[i].coordinates.x_position = rand() % width;
        people[i].coordinates.y_position = rand() % width;
        people[i].status = -1;
        if (map[people[i].coordinates.x_position][people[i].coordinates.y_position] != 0) {
            i--;
        }
        map[people[i].coordinates.x_position][people[i].coordinates.y_position] = people[i].status;
    }

    // patient zero
    int patient_zero = rand() % population;
    people[patient_zero].status = 1;

    // actual code
    print_map(width, map);

    for (int i = 0; i < days; i++) {
        move_people(range, width, population, map, people);

        print_map(width, map);

        for (int j = 0; j < population; j++) {
            printf("Person %d is at (%d, %d) and has the status code %d.\n", j, people[j].coordinates.x_position, people[j].coordinates.y_position, people[j].status);
        }
    }

    printf("\n====SUMMARY====\n");
    printf("Patient Zero: %d\n", patient_zero);

    return 0;
}