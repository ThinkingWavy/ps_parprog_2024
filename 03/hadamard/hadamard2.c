#include <stdio.h>
#include <stdlib.h>

int main (void) {
    int a [512][512]={{1}};
    int b [512][512]={{1}};
    int c [512][512]={{0}};
    size_t n = 512;

    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i < n; ++i) {
            c[i][j] = a[i][j] * b[i][j];
        }
    }
}