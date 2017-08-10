#include <stdio.h>

int main() {
    int x=0;
    while(x<1000000) {
        printf("%d\n", x);
        ++x;
    }
    return 0;
}