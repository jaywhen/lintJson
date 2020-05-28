#include <stdio.h>
#include <stdlib.h>

int main() {
    char* test = "123.066 is a number";
    char* str;
    double ret;
    ret = strtod(test, &str);
    
    printf("test str is : %s\n", test);
    printf("number is %f\nand the str is:%s\n", ret, str);

    size_t length = sizeof("hellods"); //sizeof字符串字面量计算的是字符串的长度＋１，多出来的１是＇\0＇
    printf("the length of test is %d\n", length);
}   
