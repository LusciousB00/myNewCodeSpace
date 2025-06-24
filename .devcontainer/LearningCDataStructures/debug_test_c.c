#include <stdio.h>

int main() {
    int a = 10;
    int b = 20;
    int sum = a + b;
    printf("The sum is: %d\n", sum);
    // Let's add a condition to step through
    if (sum > 25) {
        printf("Sum is greater than 25.\n");
    } else {
        printf("Sum is 25 or less.\n");
    }
    return 0;
}