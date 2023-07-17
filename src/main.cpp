#include "print.h"

int main() {
    short s = 4;
    int i = 5;
    long l = 6;
    int h = 0x11223344;

    short* pointer = &s;

    char* string = "hello";

    char* array = new char[4];
    array[3] = 9;

    return 0;
}