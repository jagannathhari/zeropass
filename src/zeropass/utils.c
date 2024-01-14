#include "constant.h"
#include "sha512.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int compare(const void *a, const void *b) {
    const char *char_a = (const char *)a;
    const char *char_b = (const char *)b;
    return *char_a - *char_b;
}

/*
 * Function: sha512_base64_encode
 * --------------------
 * convert the given sha512 hash in base64
 *
 * Parameters:
 *   input: input to be convert in base64
 *   input_len: length of the input
 *   output: where to store the base64 value
 *
 */

static void sha512_base64_encode(const uint8_t *input, size_t input_len,
                                 char *output) {
    size_t i, j;
    uint32_t value;
    for (i = 0, j = 0; i < input_len; i += 3, j += 4) {
        value = (input[i] << 16) |
                ((i + 1 < input_len) ? (input[i + 1] << 8) : 0) |
                ((i + 2 < input_len) ? input[i + 2] : 0);
        output[j] = BASE64_TABLE[(value >> 18) & 0x3F];
        output[j + 1] = BASE64_TABLE[(value >> 12) & 0x3F];
        output[j + 2] =
            (i + 1 < input_len) ? BASE64_TABLE[(value >> 6) & 0x3F] : '=';
        output[j + 3] = (i + 2 < input_len) ? BASE64_TABLE[value & 0x3F] : '=';
    }

    output[j] = '\0';
}

/*
 * Function: _sha512_rounds
 * --------------------
 * Hash the input n times
 *
 * Parameters:
 *   input: input to be hashed
 *   rounds: Number of time it should be hashed
 *
 */

static void _sha512_rounds(uint8_t *input, int rounds) {
    struct sha512_state state;
    sha512_init(&state);
    for (int i = 1; i < rounds; ++i) {
        sha512_add(&state, input, SHA512_HASH_SIZE);
        sha512_final(&state, input);
    }
}

/*
 * Function: shuffle_array
 * --------------------
 * Shuffle the array in random order(based on seed value)
 *
 * Parameters:
 *   seed: seed to be used for generating random number
 *   len: The length of the array
 *   string: The array to be shuffled
 *
 * Returns:
 *   The Shuffled array;
 */

static void shuffle_array(unsigned int seed, unsigned int len, char *string) {
    srand(seed);
    unsigned int i;
    int j;
    char temp;
    for (i = 0; i < len; i++) {
        j = rand() % len;
        temp = string[i];
        string[i] = string[j];
        string[j] = temp;
    }
}

/*
 * Function: _multiply_string
 * --------------------
 * Multiply the string n times.
 *
 * Parameters:
 *   to_include: string to be mulitplied.
 *   n: number of times string should be mulitplied.
 *
 * Returns:
 *   The string mulitplied n times.
 */

static char *_multiply_string(const char *to_include, unsigned int n) {
    char *template = (char *)malloc(sizeof(char) * n + 1);
    if (template == NULL) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    size_t included_len = strlen(to_include);
    unsigned int i;
    for (i = 0; i < n; i += included_len) {
        memcpy(template + i, to_include, included_len);
    }
    memcpy(template + n - (n % included_len), to_include, n % included_len);
    template[n] = '\0';
    return template;
}

static char *get_template(unsigned int len, const char *to_include, char seed) {
    char *template;

    if (strlen(to_include) == len) {
        template = (char *)malloc(len + 1);
        strcpy(template, to_include);
        return template;
    }
    template = _multiply_string(to_include, len);
    shuffle_array((unsigned int)seed, len, template);
    return template;
}

char *get_masterkey(const char *username, const char *site,
                    const char *master_password, int rounds) {

    char *base64_hash = (char *)malloc(sizeof(char) * 89);
    if (base64_hash == NULL) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }

    struct sha512_state state;
    uint8_t hash[SHA512_HASH_SIZE];
    sha512_init(&state);
    sha512_add(&state, username, strlen(username));
    sha512_add(&state, site, strlen(site));
    sha512_add(&state, master_password, strlen(master_password));
    sha512_final(&state, hash);
    _sha512_rounds(hash, rounds);
    sha512_base64_encode(hash, sizeof(hash), base64_hash);
    return base64_hash;
}

static char get_password_char(const char template_char, const char key_char) {
    switch (template_char) {
    case 'A':
        return (char)((int)key_char % 26) + 65;
    case 'a':
        return (char)((int)key_char % 26) + 97;
    case 'n':
        return (char)((int)key_char % 10) + 48;
    case 'S':
        return SPECIAL_CHAR[((int)key_char % 24)];
    default:
        return '\0';
    }
}

char *get_password(const char *to_include, const char *master_key,
                   long unsigned int password_len) {
    size_t to_include_len = strlen(to_include);
    char *temp = (char *)malloc(sizeof(char) * to_include_len + 1);
    char *template;
    if (temp == NULL) {
        fprintf(stderr, "Unable to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(temp, to_include);
    template = get_template(password_len, temp, master_key[0]);
    for (unsigned int i = 0; template[i] != '\0'; i++) {
        template[i] = get_password_char(template[i], master_key[i]);
    }
    free(temp);
    return template;
}
