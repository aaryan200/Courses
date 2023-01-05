#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
int main() {
    mkdir("1",0777);
    FILE* f=fopen("1/1.txt","a+");
    fprintf(f,"Hi");
    return 0;
}