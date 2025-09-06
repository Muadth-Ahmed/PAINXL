#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_SIZE 300000
#define MAX_COMMANDS 10000000

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename.eml>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    char **commands = malloc(MAX_COMMANDS * sizeof(char *));
    if (!commands) {
        perror("Memory allocation failed");
        fclose(fp);
        return 1;
    }

    int command_count = 0;
    char bit_buffer[9] = {0}; // 8 bits + null terminator
    int bits_read = 0;
    int ch;

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '0' || ch == '1') {
            bit_buffer[bits_read++] = (char)ch;
            if (bits_read == 8) {
                bit_buffer[8] = '\0';
                commands[command_count] = strdup(bit_buffer);
                if (!commands[command_count]) {
                    perror("Memory allocation failed");
                    return 1;
                }
                command_count++;
                if (command_count >= MAX_COMMANDS) {
                    fprintf(stderr, "Too many commands!\n");
                    return 1;
                }
                bits_read = 0;
            }
        }
    }

    fclose(fp);

    unsigned char tape[TAPE_SIZE] = {0};
    unsigned char *ptr = tape;

    for (int pc = 0; pc < command_count; pc++) {
        char *cmd = commands[pc];

        if (strcmp(cmd, "01001001") == 0) {
            ++ptr;
            if (ptr >= tape + TAPE_SIZE) {
                fprintf(stderr, "Pointer out of bounds (right)\n");
                return 1;
            }
        } else if (strcmp(cmd, "10101001") == 0) {
            if (ptr <= tape) {
                fprintf(stderr, "Pointer out of bounds (left)\n");
                return 1;
            }
            --ptr;
        } else if (strcmp(cmd, "00100101") == 0) {
            ++(*ptr);
        } else if (strcmp(cmd, "10010101") == 0) {
            --(*ptr);
        } else if (strcmp(cmd, "11100001") == 0) {
            putchar(*ptr);
        } else if (strcmp(cmd, "00011101") == 0) {
            int input = getchar();
            *ptr = (input == EOF) ? 0 : (unsigned char)input;
        } else if (strcmp(cmd, "00101101") == 0) {
            *ptr = 0;
        } else if (strcmp(cmd, "00000001") == 0) {
            if (*ptr == 0) {
                int depth = 1;
                while (depth > 0) {
                    pc++;
                    if (pc >= command_count) {
                        fprintf(stderr, "Unmatched loop start (00000001)\n");
                        return 1;
                    }
                    if (strcmp(commands[pc], "00000001") == 0) depth++;
                    else if (strcmp(commands[pc], "11111101") == 0) depth--;
                }
            }
        } else if (strcmp(cmd, "11111101") == 0) {
            if (*ptr != 0) {
                int depth = 1;
                while (depth > 0) {
                    pc--;
                    if (pc < 0) {
                        fprintf(stderr, "Unmatched loop end (11111101)\n");
                        return 1;
                    }
                    if (strcmp(commands[pc], "11111101") == 0) depth++;
                    else if (strcmp(commands[pc], "00000001") == 0) depth--;
                }
                pc--; // Go back to matching 00000001
            }
        } else {
            fprintf(stderr, "Unknown command: %s\n", cmd);
            return 1;
        }
    }

    for (int i = 0; i < command_count; i++) {
        free(commands[i]);
    }
    free(commands);

    return 0;
}
