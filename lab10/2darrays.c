#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

int get(int cols, int row, int col, const int* A) {
    return *(A + (row * cols) + col);
}

void set(int cols, int row, int col, int* A, int value) {
    *(A + (row * cols) + col) = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int* A, int* B, int* AB) {
    for (int rowA = 0; rowA < rowsA; rowA++) {
        for (int colB = 0; colB < colsB; colB++) {
            int product = 0;
            for (int colA = 0; colA < colsA; colA++) {
                product += get(colsA, rowA, colA, A) * get(colsB, colA, colB, B);
            }
            set(colsB, rowA, colB, AB, product);
        }
    }

}

void read_mat(int rows, int cols, int* t) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            int i = (row * cols) + col;
            scanf("%i", (t + i));
        }
    }
}

void print_mat(int rows, int cols, int* t) {
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            printf("%i ", get(cols, row, col, t));
        }
        printf("\n");
    }

}

int read_char_lines(char* array[], char* line_buff) {
    int len, n = 0;
    int scan = 1;

    char ch = ' ';
    while (scan != -1) {
        scan = fscanf(stdin, "%c", &ch);
        if (ch == '\n' || scan == -1) {
            line_buff[n] = '\0';
            n++;
            char* ptr = (char*)malloc(n * sizeof(char));
            memcpy(ptr, line_buff, n * sizeof(char));
            array[len] = ptr;
            n = 0;
            len++;
        }
        else {
            line_buff[n] = ch;
            n++;
        }
    }
    return len;
}

void write_char_line(char* array[], int n) {
    int i = 0;
    while (*(*(array + n) + i) != '\0') {
        printf("%c", *(*(array + n) + i));
        i++;
    }
}

void delete_lines(char* array[]) {
}

int read_int_lines_cont(int* ptr_array[]) {
    int arr[TAB_SIZE];
    int len = 0;
    int n = 0;
    char ch = ' ';
    int new_line = 1;
    int scan = 1;
    while (scan != -1) {
        scan = fscanf(stdin, "%i%c", &arr[n], &ch);
        if (new_line) {
            *(ptr_array + len) = &arr[n];
            len++;
            new_line = 0;
        }
        if (ch == '\n' || scan == -1) new_line = 1;
        n++;
    }
    return len;
}

void write_int_line_cont(int* ptr_array[], int n) {
    printf("\n");
    int i = 0;
    while ((*(ptr_array + n) + i) != *(ptr_array + n + 1)) {
        printf("%i ", *(*(ptr_array + n) + i));
        i++;
    }
}

typedef struct {
    int* values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[], int* line_buff) {
    int len, n = 0;
    int scan = 1;
    double sum = 0;
    char ch = ' ';
    int int_char = 0;
    while (scan != -1) {
        scan = fscanf(stdin, "%i%c", &int_char, &ch);
        line_buff[n] = int_char;
        sum += int_char;
        n++;
        if (scan == -1) break;
        if (ch == '\n') {
            int* ptr = (int*)malloc(n * sizeof(int));
            memcpy(ptr, line_buff, n * sizeof(int));
            lines_array[len].values = ptr;
            lines_array[len].len = n;
            lines_array[len].average = sum / n;
            n = 0;
            sum = 0;
            len++;
        }
    }
    return len;
}

void write_int_line(line_type lines_array[], int n) {
    printf("\n");

    for (int i = 0; i < lines_array[n].len; i++) {
        printf("%i ", lines_array[n].values[i]);
    }
    printf("\n%lf", lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count) {
}

int cmp(const void* a, const void* b) {
    const line_type* line_a = a;
    const line_type* line_b = b;
    return (*line_a).average - (*line_b).average;
}

void sort_by_average(line_type lines_array[], int line_count) {
    qsort(lines_array, line_count, sizeof(line_type), cmp);
}

typedef struct {
    int r, c, v;
} triplet;

int read_sparse(triplet* triplet_array, int n_triplets) {
    for (int i = 0; i < n_triplets; i++) {
        scanf("%i %i %i", &triplet_array[i].r, &triplet_array[i].c, &triplet_array[i].v);
    }
    return n_triplets;
}

int cmp_triplets(const void* t1, const void* t2) {
    const triplet* ct1 = t1;
    const triplet* ct2 = t2;
    int cmp_row = (*ct1).r - (*ct2).r;
    if (cmp_row != 0) return cmp_row;
    int cmp_col = (*ct1).c - (*ct2).c;
    return cmp_col;
}

void make_CSR(triplet* triplet_array, int n_triplets, int rows, int* V, int* C, int* R) {
    qsort(triplet_array, n_triplets, sizeof(triplet), cmp_triplets);
    int i_trip = 0;
    R[0] = 0;
    // int i_row = 1;
    for (int i = 0; i < rows; i++) {
        int empty_row = 1;
        while (triplet_array[i_trip].r == i) {
            empty_row = 0;
            C[i_trip] = triplet_array[i_trip].c;
            V[i_trip] = triplet_array[i_trip].v;
            i_trip++;
        }
        R[i + 1] = i_trip;
    }
    R[rows] = n_triplets;
}

void multiply_by_vector(int rows, const int* V, const int* C, const int* R, const int* x, int* y) {
    for (int i = 0; i < rows; i++) {
        int row_start = R[i];
        int row_end = R[i + 1];
        y[i] = 0;
        for (int j = row_start; j < row_end; j++) {
            y[i] += V[j] * x[C[j]];
        }
    }
}

void read_vector(int* v, int n) {
    for (int i = 0; i < n; i++) {
        scanf("%i", &v[i]);
    }
}

void write_vector(int* v, int n) {
    for (int i = 0; i < n; i++) {
        printf("%i ", v[i]);
    }
    printf("\n");
}

int read_int() {
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void) {
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char* char_lines_array[TAB_SIZE] = { NULL };
    int continuous_array[TAB_SIZE];
    int* ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];
    char line_buff[BUF_SIZE];
    int int_line_buff[BUF_SIZE];

    switch (to_do) {
    case 1:
        scanf("%d %d", &rowsA, &colsA);
        read_mat(rowsA, colsA, A);
        scanf("%d %d", &rowsB, &colsB);
        read_mat(rowsB, colsB, B);
        prod_mat(rowsA, colsA, colsB, A, B, AB);
        print_mat(rowsA, colsB, AB);
        break;
    case 2:
        n = read_int() - 1; // we count from 1 :)
        ptr_array[0] = continuous_array;
        read_int_lines_cont(ptr_array);
        write_int_line_cont(ptr_array, n);
        break;
    case 3:
        n = read_int() - 1;
        read_char_lines(char_lines_array, line_buff);
        write_char_line(char_lines_array, n);
        delete_lines(char_lines_array);
        break;
    case 4:
        n = read_int() - 1;
        lines_counter = read_int_lines(int_lines_array, int_line_buff);
        sort_by_average(int_lines_array, lines_counter);
        write_int_line(int_lines_array, n);
        delete_int_lines(int_lines_array, lines_counter);
        break;
    case 5:
        scanf("%d %d %d", &rows, &cols, &n_triplets);
        n_triplets = read_sparse(triplet_array, n_triplets);
        read_vector(x, cols);
        make_CSR(triplet_array, n_triplets, rows, V, C, R);
        multiply_by_vector(rows, V, C, R, x, y);
        write_vector(V, n_triplets);
        write_vector(C, n_triplets);
        write_vector(R, rows + 1);
        write_vector(y, rows);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
        break;
    }
    return 0;
}


