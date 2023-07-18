#include "print.h"

int main() {
    short s = 4;
    int i = 5;
    int h = 0x11223344;

    short* pointer = &s;
    short* array = new short[4];
    *(array + 0) = 2;
    *(array + 1) = 3;
    *(array + 2) = 4;

    // array[3] = 9;
    // array[5] = 8;


    const char* string = "hello";


    return 0;
}