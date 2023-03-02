#include <stdio.h>

int main() {
    int number, i;
    int a = 0, b = 1, c = 1;
    printf("Podaj liczbe: ");
    scanf("%i", &number);
    for(i = 0; i<number; i++){
        printf("%i ",a);
        c = b;
        b = a+b;
        a = c;
    }

    return 0;
}