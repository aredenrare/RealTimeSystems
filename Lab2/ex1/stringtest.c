#include <stdio.h>
#include <stdlib.h>
# include <string.h>

int main(void){
    char a[1000], b[1000];

    printf("Enter the first string\n");
    scanf("%s",a);

    printf("Enter the second string\n");
    scanf("%s",b);

    strcat(a, b);

    printf("String obtained on concatenation: %s\n", a);

    return 0;

}