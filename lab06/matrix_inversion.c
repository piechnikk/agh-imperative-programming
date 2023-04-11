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
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
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
