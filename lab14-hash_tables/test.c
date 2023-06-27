#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

int main(){
    int scan = 1;

    char *string;
    while (scan != -1) {
        scan = fscanf(stdin, "%s", string);
        printf("%s\n", string);
    }

    return 0;
}