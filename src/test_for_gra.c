#include <stdio.h>
#include <stdlib.h>

int main() {
    char* test = "number";
    printf("before is : %s\n", test);
    for(size_t i = 0; i < 6; ++i) {
        int se = (int) (test[i] + 1);
        char good = (char) se;
        printf("%c \n", good);
        //printf("%d \n", se );
    }
    printf("now is %s\n", test);
    // char* str;
    // double ret;
    // ret = strtod(test, &str);
    
    // printf("test str is : %s\n", test);
    // printf("number is %f\nand the str is:%s\n", ret, str);

    // size_t length = sizeof("hellods"); //sizeof字符串字面量计算的是字符串的长度＋１，多出来的１是＇\0＇
    // printf("the length of test is %d\n", length);
}   
