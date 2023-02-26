#include "syscall.h"
#define MAXSIZE 1000

int main() {
    char name[MAXSIZE];
    int len;

    PrintString("Enter length file's name you want delete: ");
    len = ReadNum();
    PrintString("Enter file's name delete: ");
    ReadString(name, len);

    if (Remove(name) == 1)
        PrintString("Delete file successfully!\n\n");
    else
        PrintString("Delete file failed.\n\n");

    Halt();
}