#include <stdio.h>
#include <stdlib.h>
int main() {
    char* test = "123.066 is a number";
    char* str;
    double ret;
    ret = strtod(test, &str);
    
    printf("test str is %s\n", test);
    printf("number is %f\nand the str is:%s\n", ret, str);
}
