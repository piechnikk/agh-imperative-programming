#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define SIZE 40

#define TEST 0    // 1 - dla testow,  0 - dla oceny automatycznej

void read_vec(double x[], size_t n) {
    for (size_t i = 0; i < n; ++i)  scanf("%lf", x++);
}

void print_vec(const double x[], size_t n) {
    for (size_t i = 0; i < n; ++i) printf("%.4f ", x[i]);
    printf("\n");
}

void read_mat(double A[][SIZE], size_t m, size_t n) {
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j)  scanf("%lf", &A[i][j]);
    }
}

void print_mat(const double A[][SIZE], size_t m, size_t n) {
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j)  printf("%.4f ", A[i][j]);
        printf("\n");
    }
}

// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    double determinant = 1;
    int rows_permutation_vector[n];
    for (size_t i = 0; i < n; i++) {
        rows_permutation_vector[i] = i;
        x[i] = b[i];
    }

    for (size_t i = 0; i < n; i++) {
        int indexof_actual_max = i;
        for (size_t j = i + 1; j < n; j++) {
            if (fabs(A[rows_permutation_vector[j]][i]) > fabs(A[rows_permutation_vector[indexof_actual_max]][i])) {
                indexof_actual_max = j;
            }
        }
        if (fabs(A[rows_permutation_vector[indexof_actual_max]][i]) < eps) return 0;
        if (indexof_actual_max != i) {
            int helper = rows_permutation_vector[i];
            rows_permutation_vector[i] = rows_permutation_vector[indexof_actual_max];
            rows_permutation_vector[indexof_actual_max] = helper;

            double helper2 = x[rows_permutation_vector[i]];
            x[rows_permutation_vector[i]] = x[i];
            x[i] = helper2;

            determinant *= -1;
        }

        for (size_t j = i + 1; j < n; j++) {
            double row_multiplier = A[rows_permutation_vector[j]][i] / A[rows_permutation_vector[i]][i];
            for (size_t k = i; k < n; k++) {
                A[rows_permutation_vector[j]][k] -= A[rows_permutation_vector[i]][k] * row_multiplier;
            }
            x[j] -= x[i] * row_multiplier;
        }
        determinant *= A[rows_permutation_vector[i]][i];
    }
    for (int i = n - 1; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            x[i] -= A[rows_permutation_vector[i]][j] * x[j];
        }
        x[i] /= A[rows_permutation_vector[i]][i];
    }
    return determinant;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
    double temp_B[n][n];
    double determinant = 1;
    int rows_permutation_vector[n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp_B[i][j] = 0;
        }
        temp_B[i][i] = 1;
    }

    for (size_t i = 0; i < n; i++) {
        rows_permutation_vector[i] = i;
    }
    for (size_t i = 0; i < n; i++) {
        int indexof_actual_max = i;
        for (size_t j = i + 1; j < n; j++) {
            if (fabs(A[rows_permutation_vector[j]][i]) > fabs(A[rows_permutation_vector[indexof_actual_max]][i])) {
                indexof_actual_max = j;
            }
        }
        if (fabs(A[rows_permutation_vector[indexof_actual_max]][i]) < eps) return 0;
        if (indexof_actual_max != i) {
            int helper = rows_permutation_vector[i];
            rows_permutation_vector[i] = rows_permutation_vector[indexof_actual_max];
            rows_permutation_vector[indexof_actual_max] = helper;
            determinant *= -1;
        }

        double temp_multiplier = A[rows_permutation_vector[i]][i];
        determinant *= temp_multiplier;
        for (size_t j = 0; j < n; j++) {
            A[rows_permutation_vector[i]][j] /= temp_multiplier;
            temp_B[rows_permutation_vector[i]][j] /= temp_multiplier;
        }
        for (size_t j = i + 1; j < n; j++) {
            double row_multiplier = A[rows_permutation_vector[j]][i] / A[rows_permutation_vector[i]][i];
            for (size_t k = 0; k < n; k++) {
                A[rows_permutation_vector[j]][k] -= A[rows_permutation_vector[i]][k] * row_multiplier;
                temp_B[rows_permutation_vector[j]][k] -= temp_B[rows_permutation_vector[i]][k] * row_multiplier;
            }
        }

    }
    for (int i = n - 1; i >= 0; i--) {
        for (int j = n - 1; j > i; j--) {
            for (int k = 0; k < n; k++) {
                temp_B[rows_permutation_vector[i]][k] -= temp_B[rows_permutation_vector[j]][k] * A[rows_permutation_vector[i]][j];
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i][j] = temp_B[rows_permutation_vector[i]][j];
        }
    }
    return determinant;
}

int main(void) {

    double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    double b[SIZE], x[SIZE], det, eps = 1.e-13;
    int to_do;
    size_t m, n, p, q;
    if (TEST) printf("Wpisz nr zadania ");
    scanf("%d", &to_do);
    switch (to_do) {
    case 1:
        if (TEST) printf("Wpisz liczbe wierszy i kolumn mac. kwadratowej: ");
        scanf("%d", &n);
        if (TEST) printf("Wpisz macierz A (wierszami): ");
        read_mat(A, n, n);
        if (TEST) printf("Wpisz wektor b: ");
        read_vec(b, n);
        det = gauss(A, b, x, n, eps);
        printf("%.4f\n", det);
        if (det) print_vec(x, n);
        break;
    case 2:
        if (TEST) printf("Wpisz rozmiar macierzy n = ");
        scanf("%d", &n);
        if (TEST) printf("Wpisz elementy macierzy (wierszami): ");
        read_mat(A, n, n);
        det = matrix_inv(A, B, n, eps);
        printf("%.4f\n", det);
        if (det) print_mat(B, n, n);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}
