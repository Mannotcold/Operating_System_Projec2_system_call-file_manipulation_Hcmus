#include "syscall.h"
#define MAXSIZE 1000

int main() {
    char fileName[MAXSIZE];
    int len;

    PrintString("Enter length file's name: ");
    len = ReadNum();
    PrintString("Enter file's name: ");
    ReadString(fileName, len);

    if (Create(fileName) == 1)
        PrintString("Create file sucessfully!\n\n");

    else
        PrintString("Create file failed.\n\n");

    Halt();
}