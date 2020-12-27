// 
// CS50x 2020 Final Project
// Project title: Epidemic Modelling
// Name: Matthew Shen
// GitHub: github.com/flyme2bluemoon
// City: Toronto, ON, Canada
// 

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
   int basic_reproduction_number;
} Person;

void greet(void) {
    printf("Hello, world!\n");
    printf("Made with love by Matthew Shen (flyme2bluemoon)\n");
    printf("Wear a mask and stay save during the COVID-19 Pandemic\n");
    printf("\n================================================\n\n");

    return;
}

void print_help(char *program_name) {
    printf("Usage: %s [--help] [--display-map] [--display-status] [--verbose]\n\n", program_name);

    printf("optional arguments:\n");
    printf("  -h, --help \t\t show this help message and exit.\n");
    printf("  --display-map \t Display the map after every simulated day.\n");
    printf("  --display-status \t Display the status after every simulated day..\n");
    printf("  --display-case-counts  Displays the daily and cumulative case counts in stdin.\n");
    printf("  --verbose \t\t Behaves as if --display-map and --display-status were specified.\n");

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

double get_average(int length, double array[length]) {
    double total = 0;

    for (int i = 0; i < length; i++) {
        total += array[i];
    }

    double output = total / (double) length;

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
        if ((people[i].coordinates.x_position + dx) >= width || (people[i].coordinates.x_position + dx) < 0 || (people[i].coordinates.y_position + dy) >= width || (people[i].coordinates.y_position + dy) < 0 || map[people[i].coordinates.x_position + dx][people[i].coordinates.y_position + dy] != -1) {
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

void virus_transmission(int transmission_distance, int chance_of_transmission, int width, int population, int map[width][width], Person people[population]) {
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
                            if (rand() % 100 < chance_of_transmission && people[i].status == -1) {
                                people[map[people[i].coordinates.x_position + dx][people[i].coordinates.y_position +dy]].basic_reproduction_number++;
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

int add_new_cases(int population, Person people[population], int current_day, int days, int case_count, int daily_cases[days], int cumulative_cases[days]) {
    double infected = calculate_number_of_infections(population, people);
    int new_cases = (int)infected - case_count;

    daily_cases[current_day] = new_cases;

    if (current_day == 0) {
        cumulative_cases[current_day] = new_cases;
    } else {
        cumulative_cases[current_day] = cumulative_cases[current_day - 1] + new_cases;
    }

    return new_cases;
}

void print_daily_case_counts(int days, int daily_cases[days], bool display_case_count) {
    FILE *daily_case_counts_txt = fopen("daily_case_counts.txt", "w");
    FILE *daily_case_counts_csv = fopen("daily_case_counts.csv", "w");

    for (int i = 0; i < days; i++) {
        fprintf(daily_case_counts_txt, "Day %d: %d new cases\n", i + 1, daily_cases[i]);
        fprintf(daily_case_counts_csv, "day_%d,%d\n", i + 1, daily_cases[i]);
        if (display_case_count) {
            printf("Day %d: %d new cases\n", i + 1, daily_cases[i]);
        }
    }

    fclose(daily_case_counts_txt);
    fclose(daily_case_counts_csv);
}

void print_cumulative_case_counts(int days, int cumulative_cases[days], bool display_case_count) {
    FILE *cumulative_case_counts_txt = fopen("cumulative_case_counts.txt", "w");
    FILE *cumulative_case_counts_csv = fopen("cumulative_case_counts.csv", "w");

    for (int i = 0; i < days; i++) {
        fprintf(cumulative_case_counts_txt, "Day %d: %d cumulative cases\n", i + 1, cumulative_cases[i]);
        fprintf(cumulative_case_counts_csv, "day_%d,%d\n", i + 1, cumulative_cases[i]);
        if (display_case_count) {
            printf("Day %d: %d new cases\n", i + 1, cumulative_cases[i]);
        }
    }

    fclose(cumulative_case_counts_txt);
    fclose(cumulative_case_counts_csv);
}

double calculate_basic_reproduction_number(int population, Person people[population]) {
    double total = 0;
    double spreaders = 0;
    for (int i = 0; i < population; i++) {
        total += people[i].basic_reproduction_number;
        if (people[i].status == -3 || people[i].status > 1) {
            spreaders++;
        }
    }
    double basic_reproduction_number = total / spreaders;

    return basic_reproduction_number;
}

double vaccinized_percentage_required(double basic_reproduction_number) {
    double endemic_equilibrium = 1.0 / basic_reproduction_number;
    double percentage_req = (1 - endemic_equilibrium) * 100;

    return percentage_req;
}

int main(int argc, char *argv[]) {
    // parse command line arguments
    bool help = false;
    bool display_map = false;
    bool display_status = false;
    bool display_case_count = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            help = true;
        } else if (strcmp(argv[i], "--display-map") == 0) {
            display_map = true;
        } else if (strcmp(argv[i], "--display-status") == 0) {
            display_status = true;
        } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            display_map = true;
            display_status = true;
        } else if (strcmp(argv[i], "--display-case-counts") == 0) {
            display_case_count = true;
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            help = true;
        }
    }

    // print help message
    if (help) {
        print_help(argv[0]);
        return 0;
    }

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
    int chance_of_transmission = get_int("Chance of transmission (recommended: 50, range: 0-100):");
    int recovery_time = get_int("Recovery time (recommended: 14):");

    // init case count to zero
    int case_count = 0;
    int daily_cases[days];
    int cumulative_cases[days];

    // init daily basic reproduction number
    double daily_basic_reproduction_number[days];

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
    } else if (chance_of_transmission > 100 || chance_of_transmission < 0) {
        printf("Chance of traqnsmission must be between 0 and 100.\n");
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
        people[i].basic_reproduction_number = 0;
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
        virus_transmission(transmission_distance, chance_of_transmission, width, population, map, people);
        printf("Tallying case counts...\n");
        int new_cases = add_new_cases(population, people, i - 1, days, case_count, daily_cases, cumulative_cases);
        case_count += new_cases;
        printf("Daily case count: %d\n", new_cases);
        double current_basic_reproduction_number = calculate_basic_reproduction_number(population, people);
        daily_basic_reproduction_number[i - 1] = current_basic_reproduction_number;
        printf("Basic Reproduction Number: %f\n", current_basic_reproduction_number);
        if (display_map) {
            printf("Printing map...\n");
            print_map(width, population, map, people);
        }
        if (display_status) {
            printf("Printing Status...\n");
            print_status(population, people);
        }
    }

    // Prints summary
    printf("\n====SUMMARY====\n");
    printf("Patient Zero: %d\n", patient_zero);
    double percentage_infected = calculate_infected_percentage(population, people);
    printf("Percentage infected: %.2f%%\n", percentage_infected);
    printf("Total number of cases: %d\n", case_count);
    print_daily_case_counts(days, daily_cases, display_case_count);
    print_cumulative_case_counts(days, cumulative_cases, display_case_count);
    double basic_reproduction_number = get_average(days, daily_basic_reproduction_number);
    printf("Basic Reproduction Number: %f\n", basic_reproduction_number);
    double vaccine_percent_requirement = vaccinized_percentage_required(basic_reproduction_number);
    printf("Percentage of population immunized to stop spread required: %.2f%%\n", vaccine_percent_requirement);

    return 0;
}