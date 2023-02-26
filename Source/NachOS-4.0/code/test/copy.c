#include "syscall.h"
#define MAXSIZE 256

int main()
{
    char readName[MAXSIZE], writeName[MAXSIZE];
    int readLen, writeLen;
    int readId, writeId;
    char content[MAXSIZE + 1];
    int readSize;


    PrintString("Enter length of read file's name: ");
    readLen = ReadNum();
    PrintString("Enter length of write file's name: ");
    writeLen = ReadNum();
    PrintString("Enter name of read file: ");
    ReadString(readName, readLen);

    readId = Open(readName);

    if (readId == -1){
        PrintString("\nCan't open read file.\n\n");
        Halt();
        return;
    }


    PrintString("Enter name of write file: ");
    ReadString(writeName, writeLen);

    writeId = Open(writeName);

    if (writeId == -1){
        PrintString("\nCan't open write file.\n\n");
        Halt();
        return;
    }

    while (1)
    {
        readSize = Read(content, MAXSIZE, readId);
        content[readSize] = '\0';

        if (readSize > 0) Write(content, readSize, writeId);
        else break;
    }

    PrintString("\nCopy sucessfully!\n\n");

    Close(readId);
    Close(writeId);

    Halt();
    /* not reached */
}
