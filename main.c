

#include "constant.h"
#include "utils.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_valid_template(const char *template) {
    int i;
    for (i = 0; template[i] != '\0'; i++) {
        if (strchr("ASan", template[i]) == NULL) {
            return false;
        }
    }
    return true;
}

bool is_valid_number(const char *number) {
    for (int i = 0; number[i] != '\0'; i++) {
        if (!isdigit(number[i])) {
            return false;
        }
    }
    return true;
}

int main() {
    char username[MAX];
    char website[MAX];
    char master_password[MAX];
    int rounds = 1000;           // Default value for rounds
    char template[MAX] = "ASan"; // Default template
    int password_length = 16;    // Default password length

    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter website name: ");
    fgets(website, sizeof(website), stdin);
    website[strcspn(website, "\n")] = '\0';

    printf("Enter master password: ");
    hidden_input(master_password);
    printf("\n");

    char rounds_input[MAX];
    do {
        printf("Enter number of rounds (default: 1000): ");
        fgets(rounds_input, sizeof(rounds_input), stdin);
        rounds_input[strcspn(rounds_input, "\n")] = '\0';

        if (strlen(rounds_input) == 0) {
            break; // Use default value
        }

        if (!is_valid_number(rounds_input)) {
            printf("Invalid input. Please enter a valid number.\n");
        } else {
            rounds = atoi(rounds_input);
            break;
        }
    } while (true);

    char template_input[MAX];
    do {
        printf("Enter password template (default: ASan): ");
        fgets(template_input, sizeof(template_input), stdin);
        template_input[strcspn(template_input, "\n")] = '\0';

        if (strlen(template_input) == 0) {
            break; // Use default value
        }

        if (!is_valid_template(template_input)) {
            printf("Invalid password template. Valid characters are 'A', 'S', "
                   "'a', 'n'. Please try again.\n");
        } else {
            strcpy(template, template_input);
            break;
        }
    } while (true);

    char length_input[MAX];
    do {
        printf("Enter length of password (default: 16): ");
        fgets(length_input, sizeof(length_input), stdin);
        length_input[strcspn(length_input, "\n")] = '\0';

        if (strlen(length_input) == 0) {
            break; // Use default value
        }

        if (!is_valid_number(length_input)) {
            printf("Invalid input. Please enter a valid number.\n");
        } else {
            password_length = atoi(length_input);
            break;
        }
    } while (true);

    char *masterkey = get_masterkey(username, website, master_password, rounds);
    char *password = get_password(template, masterkey, password_length);
    printf("Generated password: %s\n", password);

    free(masterkey);
    free(password);

    return EXIT_SUCCESS;
}
