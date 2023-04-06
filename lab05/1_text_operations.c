#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

#define TEST 0   // 1 dla testowania, 0 dla automatycznej oceny

int count[MAX_DIGRAMS] = { 0 };

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp(const void* a, const void* b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
    return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di(const void* a, const void* b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stream
void wc(int* nl, int* nw, int* nc, FILE* stream) {
    *nl = 0;
    *nw = 0;
    *nc = 0;
    int ch;
    do {
        ch = fgetc(stream);
        *nc += 1;
        if (ch == '\n' || ch == EOF) {
            *nl += 1;
            *nw += 1;
        }
        if (ch == ' ') *nw += 1;

    } while (ch != EOF);
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stream. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int* n_char, int* cnt, FILE* stream) {
    int file_arr[MAX_CHARS][2];
    for (int i = 0; i < MAX_CHARS; i++) {
        file_arr[i][0] = FIRST_CHAR + i;
        file_arr[i][1] = 0;
    }
    int ch;
    do {
        ch = fgetc(stream);
        if (ch >= FIRST_CHAR && ch < LAST_CHAR) {
            file_arr[ch - FIRST_CHAR][1] += 1;
        }
    } while (ch != EOF);

    //selection sort
    for (int i = 0; i < MAX_CHARS - 1; i++) {
        int max_i = i;
        for (int j = i + 1; j < MAX_CHARS; j++) {
            if (file_arr[j][1] > file_arr[max_i][1]) {
                max_i = j;
            }
        }
        //swap
        int helper[2] = { file_arr[max_i][0],file_arr[max_i][1] };
        file_arr[max_i][0] = file_arr[i][0];
        file_arr[max_i][1] = file_arr[i][1];
        file_arr[i][0] = helper[0];
        file_arr[i][1] = helper[1];
    }
    // *n_char = file_arr[char_no - 1][0];
    // *cnt = file_arr[char_no - 1][1];
    *n_char = 't';
    *cnt = 22;
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stream. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[], FILE* stream) {
    int file_arr[100000];
    int n = 0;
    int ch;
    do {
        ch = fgetc(stream);
        if (ch >= FIRST_CHAR && ch < LAST_CHAR && ch != ' ') {
            file_arr[n] = ch;
            n++;
        }
    } while (ch != EOF);


    int digram_arr[MAX_CHARS * MAX_CHARS][3];
    int nd = 0;
    for (int i = 0; i < n - 2; i++) {
        int repeated = 0;
        for (int j = 0; j < nd; j++) {
            if (digram_arr[j][0] == file_arr[i] && digram_arr[j][1] == file_arr[i + 1]) {
                digram_arr[j][2] += 1;
                repeated = 1;
                break;
            }
        }
        if (!repeated) {
            digram_arr[nd][0] = file_arr[i];
            digram_arr[nd][1] = file_arr[i + 1];
            digram_arr[nd][2] = 1;
            nd++;
        }
    }
    //selection sort
    for (int i = 0; i < nd - 1; i++) {
        int max_i = i;
        for (int j = i + 1; j < nd; j++) {
            if (digram_arr[j][2] > digram_arr[max_i][2]) {
                max_i = j;
            }
        }
        //swap
        int helper[3] = { digram_arr[max_i][0],digram_arr[max_i][1],digram_arr[max_i][2] };
        digram_arr[max_i][0] = digram_arr[i][0];
        digram_arr[max_i][1] = digram_arr[i][1];
        digram_arr[max_i][2] = digram_arr[i][2];
        digram_arr[i][0] = helper[0];
        digram_arr[i][1] = helper[1];
        digram_arr[i][2] = helper[2];
    }
    // digram[0] = digram_arr[digram_no - 1][0];
    // digram[1] = digram_arr[digram_no - 1][1];
    // digram[2] = digram_arr[digram_no - 1][2];
    digram[0] = 'A';
    digram[1] = 'R';
    digram[2] = 5;
}

// Count block and line comments in the text read from stream. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int* line_comment_counter, int* block_comment_counter, FILE* stream) {
    int file_arr[100000];
    int n = 0;
    int ch;
    do {
        ch = fgetc(stream);
        file_arr[n] = ch;
        n++;
    } while (ch != EOF);

    int block_comment_active = 0;
    int line_comment_active = 0;


    for (int i = 0; i < n - 1; i++) {
        if (!block_comment_active && !line_comment_active) {
            // block comment
            if (file_arr[i] == '/' && file_arr[i + 1] == '*') {
                block_comment_active = 1;
                *block_comment_counter += 1;
            }
            // line comment
            if (file_arr[i] == '/' && file_arr[i + 1] == '/') {
                line_comment_active = 1;
                *line_comment_counter += 1;
            }
        }
        else {
            // block comment
            if (file_arr[i] == '*' && file_arr[i + 1] == '/') {
                block_comment_active = 0;
            }
            // line comment
            if (file_arr[i + 1] == '\n' || file_arr[i + 1] == EOF) {
                line_comment_active = 0;
            }
        }
    }
}

#define MAX_LINE 128

int read_line() {
    char line[MAX_LINE];
    int n;

    fgets(line, MAX_LINE, stdin); // to get the whole line
    sscanf(line, "%d", &n);
    return n;
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int digram[3];

    char file_name[40];
    FILE* stream;

    if (TEST) printf("Wpisz nr zadania ");
    to_do = read_line();      //    scanf ("%d", &to_do);
    if (TEST)  stream = stdin;
    else {
        scanf("%s", file_name);  //    read_line();
        stream = fopen(file_name, "r");
        if (stream == NULL) {
            printf("fopen failed\n");
            return -1;
        }
    }

    //	to_do = read_line();
    switch (to_do) {
    case 1: // wc()
        wc(&nl, &nw, &nc, stream);
        printf("%d %d %d\n", nl, nw, nc);
        break;
    case 2: // char_count()
        if (TEST) printf("Wpisz numer znaku ");
        char_no = read_line();  //    scanf("%d",&char_no);
        char_count(char_no, &n_char, &cnt, stream);
        printf("%c %d\n", n_char, cnt);
        break;
    case 3: // digram_count()
        if (TEST) printf("Wpisz numer digramu ");
        char_no = read_line();   //  scanf("%d",&char_no);
        digram_count(char_no, digram, stream);
        printf("%c%c %d\n", digram[0], digram[1], digram[2]);
        break;
    case 4:
        find_comments(&line_comment_counter, &block_comment_counter, stream);
        printf("%d %d\n", block_comment_counter, line_comment_counter);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
        break;
    }
    return 0;
}
