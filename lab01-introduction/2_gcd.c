// greatest common divisor
#include <stdio.h>

int main()
{
    int a, b, c;
    printf("Podaj: ");
    scanf("%i %i", &a, &b);
    while (b != 0)
    {
        if (b > a)
        {
            c = b;
            b = a;
            a = c;
        }
        c = b;
        b = a - b;
        a = c;
    }
    printf("%i", a);
}
