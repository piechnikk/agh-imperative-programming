// square root by newton's method

#include <stdio.h>

int main()
{
    float a, e;
    printf("Podaj: ");
    scanf("%f %f", &a, &e);
    float b = a + 1, c = a;
    while (b - c > e)
    {
        b = c;
        c = (b + (a / b)) / 2;
    }
    printf("%f", c);
}