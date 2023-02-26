#include "syscall.h"
#define MAXSIZE 1000

int
main()
{
  char fileName[MAXSIZE];
  int len;
  char content[MAXSIZE + 1];
  int readSize;
  int id;

  PrintString("Enter length of file's name: ");
  len = ReadNum();
  PrintString("Enter file's name: ");
  ReadString(fileName, len);
  
  id = Open(fileName); // Mở file.

  // Kiểm tra xem file có mở được hay không:
  if (id == -1) {
    PrintString("Can't open file.\n");
    Halt();
    return;
  }
  
  PrintString("\n************************************ BEGIN READ ***************************************\n");

  while (1) {
    // Truyền nội dung đọc được từ file vào content, số ký tự đọc được được gán vào readSize:
    readSize = Read(content, MAXSIZE, id);

    // Đánh dấu ký tự kết thúc:
    content[readSize] = '\0';

    // Nếu readSize > 0 thì in nội dung đọc được ra màn hình:
    if (readSize > 0) PrintString(content);
    else break; // Nếu không thì break.
  }

  PrintString("\n************************************ END READ ***************************************\n\n");
  
  Close(id); // Đóng file.

  Halt();
  return;
}