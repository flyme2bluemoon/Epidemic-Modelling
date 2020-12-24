#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUFFERSIZE 100

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
    greet();

    int area = get_int("Area:");

    int map[area][area];

    for (int i = 0; i < area; i++) {
        for (int j = 0; j < area; j++) {
            map[i][j] = 0;
        }
    }

    for (int i = 0; i < area; i++) {
        for (int j = 0; j < area; j++) {
            printf("%d ", map[i][j]);
        }
        printf("\n");
    }

    return 0;
}