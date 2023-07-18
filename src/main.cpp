#include "print.h"

int main() {
    short s = 4;
    int i = 5;
    int h = 0x11223344;

    short* pointer = &s;
    char* array = new char[4];
    array[3] = 9;
    array[5] = 8;


    const char* string = "hello";


    return 0;
}