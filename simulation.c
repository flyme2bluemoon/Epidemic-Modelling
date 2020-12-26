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
        // TODO (reflect this change in code as 0 is currently invalid)
    */
} Person;

void greet(void) {
    printf("Hello, world!\n");
    printf("Made with love by Matthew Shen (flyme2bluemoon)\n");
    printf("Wear a mask and stay save during the COVID-19 Pandemic\n");
    printf("\n================================================\n\n");

    return;
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
        if (length_of_input <= 1) {
            is_done = false;
        }
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

void print_map(int width, int population, int map[width][width], Person people[population]) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            if (map[i][j] == -1) {
                printf("X ");
            } else {
                printf("%d ", people[map[i][j]].status);
            }
        }
        printf("\n");
    }

    return;
}

void print_status(int population, Person people[population]) {
    for (int i = 0; i < population; i++) {
        printf("Person %d is at (%d, %d) and has the status code %d.\n", i, people[i].coordinates.x_position, people[i].coordinates.y_position, people[i].status);
    }
    printf("\n");

    return;
}

void increment_day(int population, Person people[population], int recovery_time) {
    for (int i = 0; i < population; i++) {
        if (people[i].status > 0) {
            people[i].status++;
            if (people[i].status > recovery_time) {
                people[i].status = -3;
            }
        }
    }

    return;
}

void move_people(int range, int width, int population, int map[width][width], Person people[population]) {
    for (int i = 0; i < population; i++) {
        int dx = (rand() % (range * 2 + 1)) - range;
        int dy = (rand() % (range * 2 + 1)) - range;
        if ((people[i].coordinates.x_position + dx) >= width || (people[i].coordinates.x_position + dx) < 0 || (people[i].coordinates.y_position + dy) >= width || (people[i].coordinates.y_position + dy) < 0) {
            i--;
        } else {
            map[people[i].coordinates.x_position][people[i].coordinates.y_position] = -1;
            people[i].coordinates.x_position += dx;
            people[i].coordinates.y_position += dy;
            map[people[i].coordinates.x_position][people[i].coordinates.y_position] = i;
        }
    }

    return;
}

void virus_transmission(int transmission_distance, int width, int population, int map[width][width], Person people[population]) {
    for (int i = 0; i < population; i++) {
        if (people[i].status == -1) {
            int positive_dx = transmission_distance;
            int negative_dx = transmission_distance * -1;
            int positive_dy = transmission_distance;
            int negative_dy = transmission_distance * -1;
            if (people[i].coordinates.x_position + positive_dx >= width) {
                positive_dx = width - people[i].coordinates.x_position - 1;
            }
            if (people[i].coordinates.x_position + negative_dx < 0) {
                negative_dx = 0 - people[i].coordinates.x_position;
            }
            if (people[i].coordinates.y_position + positive_dy >= width) {
                positive_dy = width - people[i].coordinates.y_position - 1;
            }
            if (people[i].coordinates.y_position + negative_dy < 0) {
                negative_dy = 0 - people[i].coordinates.y_position;
            }
            for (int dx = negative_dx; dx <= positive_dx; dx++) {
                for (int dy = negative_dy; dy <= positive_dy; dy++) {
                    if (map[people[i].coordinates.x_position + dx][people[i].coordinates.y_position +dy] != -1) {
                        if (people[map[people[i].coordinates.x_position + dx][people[i].coordinates.y_position +dy]].status >= 1) {
                            // TODO: update percent change for transmission later
                            if (rand() % 2) {
                                people[i].status = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    return;
}

double calculate_number_of_infections(int population, Person people[population]) {
    double infected = 0;
    for (int i = 0; i < population; i++) {
        if (people[i].status != -1) {
            infected++;
        }
    }

    return infected;
}

double calculate_infected_percentage(int population, Person people[population]) {
    double infected = calculate_number_of_infections(population, people);

    double percentage = (infected / population) * 100;

    return percentage;
}

int add_new_cases(int population, Person people[population], int current_day, int days, int case_count, int daily_cases[days]) {
    double infected = calculate_number_of_infections(population, people);
    int new_cases = (int)infected - case_count;

    daily_cases[current_day] = new_cases;

    return new_cases;
}

void print_daily_case_counts(int days, int daily_cases[days]) {
    for (int i = 0; i < days; i++) {
        printf("Day %d: %d cases\n", i + 1, daily_cases[i]);
    }
}

int main(void) {
    // print greeting message
    greet();

    // seed the random function
    srand(time(NULL));

    // getting values from user
    int width = get_int("Width (recommended: 10-100):");
    int population = get_int("Population (requirement: < width * width):");
    int days = get_int("Numbers of days (recommended: 30):");
    int range = get_int("Range of movements (recommended: 2):");
    int transmission_distance = get_int("Range of virus transmission (recommended: 1):");
    int recovery_time = get_int("Recovery time (recommended: 14):");

    // init case count to zero
    int case_count = 0;
    int daily_cases[days];

    /*
        Test input:
        width=25
        population=5
        days=5
        range=2
        transmission_distance=1
        recovery_time=14
    */

    if (population > width * width) {
        printf("Population must be less than the area.\n");
        return 1;
    } else if (range > width) {
        printf("Range must be less than the width.\n");
        return 1;
    }

    printf("\nRunning Simulation!\n\n");

    // creating arrays
    int map[width][width];
    Person people[population];

    // initializing arrays
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            map[i][j] = -1;
        }
    }

    for (int i = 0; i < population; i++) {
        people[i].coordinates.x_position = rand() % width;
        people[i].coordinates.y_position = rand() % width;
        people[i].status = -1;
        if (map[people[i].coordinates.x_position][people[i].coordinates.y_position] != -1) {
            i--;
        }
        map[people[i].coordinates.x_position][people[i].coordinates.y_position] = i;
    }

    // patient zero
    int patient_zero = rand() % population;
    people[patient_zero].status = 1;
    case_count++;

    // actual code
    for (int i = 1; i <= days; i++) {
        printf("\n[*] Day %d\n==========\n", i);
        printf("Incrementing...\n");
        increment_day(population, people, recovery_time);
        printf("Moving...\n");
        move_people(range, width, population, map, people);
        printf("Simulating virus transmission...\n");
        virus_transmission(transmission_distance, width, population, map, people);
        printf("Tallying case counts...\n");
        int new_cases = add_new_cases(population, people, i - 1, days, case_count, daily_cases);
        case_count += new_cases;
        printf("Daily case count: %d\n", new_cases);
        printf("Printing map...\n");
        print_map(width, population, map, people);
        // printf("Printing Status...\n");
        // print_status(population, people);
    }

    // Prints summary
    printf("\n====SUMMARY====\n");
    printf("Patient Zero: %d\n", patient_zero);
    double percentage_infected = calculate_infected_percentage(population, people);
    printf("Percentage infected: %.2f%%\n", percentage_infected);
    printf("Total number of cases: %d\n", case_count);
    print_daily_case_counts(days, daily_cases);

    return 0;
}