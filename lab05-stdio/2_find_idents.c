#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define TEST 0  // 1 - dla testowania,  0 - dla automatycznej oceny

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char* keywords[] = {
    "auto", "break", "case", "char",
    "const", "continue", "default", "do",
    "double", "else", "enum", "extern",
    "float", "for", "goto", "if",
    "int", "long", "register", "return",
    "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

int compare(int my_str1[], int my_str2[]) {
    if (my_str1[0] == my_str2[0]) {
        for (int i = 1; i <= my_str1[0]; i++) {
            if (my_str1[i] != my_str2[i]) return 0;
        }
    }
    else {
        return 0;
    }
    return 1;
}

void printstr(int my_str[]) {
    printf("%i", my_str[0]);
    for (int i = 1; i <= my_str[0]; i++) {
        printf("%c", my_str[i]);
    }
    printf("\n");
}

int is_keyword(int my_str[]) {
    for (int i = 0; i < 32; i++) {
        // printf("%i %i ", my_str[0], strlen(keywords[i]));
        // printstr(keywords[i]);
        if (my_str[0] != strlen(keywords[i])) continue;
        int the_same = 1;
        for (int j = 0; j < my_str[0];j++) {
            if (keywords[i][j] != my_str[j + 1]) {
                the_same = 0;
                break;
            };
        }
        if (the_same) return 1;
    }
    return 0;
}

int find_idents(FILE* stream) {
    int file_arr[100000];
    file_arr[0] = 0;
    int n = 1;

    int block_comment_active = 0;
    int line_comment_active = 0;
    int single_quote_active = 0;
    int double_quote_active = 0;
    int hash_active = 0;

    int words_array[10000][100];
    int words_count = 0;

    int ch;
    do {
        ch = fgetc(stream);
        if (ch == 92) {
            ch = fgetc(stream);
            ch = fgetc(stream);
        }
        if (!block_comment_active && !line_comment_active && !single_quote_active && !double_quote_active && !hash_active) {
            if (ch == '/') {
                ch = fgetc(stream);
                if (ch == '*') {
                    block_comment_active = 1;
                }
                if (ch == '/') {
                    line_comment_active = 1;
                }
            }
            if (ch == 39) {
                single_quote_active = 1;
            }
            if (ch == 34) {
                double_quote_active = 1;
            }
            if (ch == 35) {
                hash_active = 1;
            }

            if ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122) || ch == 95) {
                file_arr[n] = ch;
                n++;
            }
            else {
                file_arr[n] = 0;
                n++;
            }
        }
        else {
            if (ch == '*') {
                ch = fgetc(stream);
                if (ch == '/') {
                    block_comment_active = 0;
                }
            }
            if (ch == '\n') {
                line_comment_active = 0;
                hash_active = 0;
            }
            if (ch == 39) {
                single_quote_active = 0;
            }
            if (ch == 34) {
                double_quote_active = 0;
            }
        }
    } while (ch != EOF);
    for (int i = 0; i < n; i++)
    {
        printf("%c", file_arr[i]);
    }

    int i = 0;
    while (i < n - 1) {
        if (file_arr[i] == 0 && file_arr[i + 1] != 0) {
            int temp_word[1000];
            temp_word[0] = 0;
            while (file_arr[i + 1] != 0) {
                temp_word[0]++;
                temp_word[temp_word[0]] = file_arr[i + 1];
                i++;
            }
            if (!is_keyword(temp_word)) {
                int repeated = 0;
                for (int j = 0; j < words_count; j++) {
                    if (compare(temp_word, words_array[j])) {
                        repeated = 1;
                        break;
                    }

                }
                if (!repeated) {
                    for (int j = 0; j <= temp_word[0]; j++) {
                        words_array[words_count][j] = temp_word[j];
                    }
                    words_count += 1;
                }
            }
        }
        i++;
    }

    return words_count;
}

int cmp(const void* first_arg, const void* second_arg) {
    char* a = *(char**)first_arg;
    char* b = *(char**)second_arg;
    return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
    int a = *(int*)first_arg;
    int b = *(int*)second_arg;
    return strcmp(tab[a], tab[b]);
}

int main(void) {
    char file_name[40];
    FILE* stream;

    if (TEST)  stream = stdin;
    else {
        scanf("%s", file_name);
        stream = fopen(file_name, "r");
        if (stream == NULL) {
            printf("fopen failed\n");
            return -1;
        }
    }
    printf("%d\n", find_idents(stream));
    return 0;
}

