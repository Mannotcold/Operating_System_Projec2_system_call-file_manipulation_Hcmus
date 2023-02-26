#include "syscall.h"
#define MAXSIZE 256

int main()
{
    char writeName[MAXSIZE], copyName[MAXSIZE];
    int writeLen, copyLen;
    int writeId, copyId;
    char content[MAXSIZE + 1];
    int readSize;


    PrintString("Enter length of write file's name: ");
    writeLen = ReadNum();
    PrintString("Enter length of copy file's name: ");
    copyLen = ReadNum();
    PrintString("Enter name of write file: ");
    ReadString(writeName, writeLen);

    writeId = Open(writeName);

    if (writeId == -1){
        PrintString("\nCan't open write file.\n\n");
        Halt();
        return;
    }


    PrintString("Enter name of copy file: ");
    ReadString(copyName, copyLen);

    copyId = Open(copyName);

    if (copyId == -1){
        PrintString("\nCan't open copy file.\n\n");
        Halt();
        return;
    }

    Seek(-1, writeId); // Dịch chuyển con trỏ đến cuối file.

    while (1)
    {
        readSize = Read(content, MAXSIZE, copyId);
        content[readSize] = '\0';

        if (readSize > 0) Write(content, readSize, writeId);
        else break;
    }

    PrintString("\nConcatenate sucessfully!\n\n");

    Close(writeId);
    Close(copyId);

    Halt();
    /* not reached */
}
