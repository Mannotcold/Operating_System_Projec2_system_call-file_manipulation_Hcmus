/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "string"
#include <cstdlib>
#include <ctime>
#include "syscall.h"
#include "openfile.h"
#include "directory.h"
#include "filesys.h"


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}


char SysReadChar()
{
  return kernel->synchConsoleIn->GetChar();	// lấy kí tự từ console
}


void SysPrintChar(char c)
{
  kernel->synchConsoleOut->PutChar(c);		// xuất kí tự c ra console
}


int SysReadNum()
{
	char c;		// biến lưu kí tự đầu tiên
	int check = 0;	// biến kiểm tra đã đọc qua '.' hay chua, 0 là chưa đọc qua
	int num;	// biến lưu số
	int count = 1;	// biến đếm kí tự đã đọc được trước dấu '.', ban đầu giả sử số đọc vào là dương, ta sẽ đọc trước 1 kí tự

  	c = kernel->synchConsoleIn->GetChar();		// lệnh đọc từ console

	if (c == '-')	// trường hợp số âm
	{
		count = 0;	// cho số kí tự đếm được là 0
		num = 0;	// gán số ban đầu là 0 để sau đó thực hiện thuận toán
	}
	else if (c >= '0' && c <= '9') num = (int)c - '0';	// nếu là số dương thì gán num là số đầu tiên của dãy để thực hiện thuận toán

	else return 0;	// trường hợp không phải là số

	char ch;	// biến để lưu các kí tự tiếp theo
	ch = kernel->synchConsoleIn->GetChar();	// đọc kí tự tiếp theo

	while (ch != '\n')	// điều kiện kết thúc chuỗi
	{
		if (count == 10) return 0;	// nếu đã đọc được 10 kí tự nhưng chưa kết thúc chuỗi thì dừng đọc thì dừng đọc (vì tràn số)

		if ('0' <= ch && ch <= '9') {	// nếu kí tự đọc vào là số
			if (num > 214748364) return 0;	// xét số đã đọc có hàng chục, trăm,... lớn hơn hàng chục, trăm,... của giới hạn thì dừng đọc (vì tràng số)

			else if (num == 214748364)	// xét số đã đọc có hàng chục, trăm,... bằng với hàng chục, trăm,... của số giới hạn 
				if (ch == '9' || ch == '8') return 0;	// nếu hàng đơn vị là 9 và 8 thì dừng đọc (vì tràng số)

			if (check == 0)		// khi chưa đọc qua dấu '.'
			{
				num = num * 10 + (int)ch - '0';	// thuận toán chuyển từ chuỗi sang số
				count++;	// tăng số kí tự đọc được
			}

			else if (ch != '0')	// ngược lại kí tự kí tự đọc vào khác 0 thì dừng đọc (xét trường hợp số 123.00001)
				return 0;
		}

		else if (ch == '.' && check == 0)	// nếu đọc đến dấu '.' thì tăng check lên để đánh dấu đã đọc xong phần nguyên
			check++;

		else return 0;	// nếu là kí tự khác thì dừng đọc
		ch = kernel->synchConsoleIn->GetChar();		// tiếp tục lấy kí tự tiếp theo
	}

	if (c == '-') num = -num;	// nếu là số âm thì chuyển số dương vừa đọc về số âm

	return num;	// trả số về số đã đọc được
}


void SysPrintNum(int num)
{
	int _numberBuffer[10];		//khởi tạo mảng kí tự với 10 phần tử

	if (num == 0) return kernel->synchConsoleOut->PutChar('0');	// nếu num bằng 0 thì xuất ra console là 0

	if (num >= -2147483647 && num <= 2147483647)	// nếu số đọc vào nằm trong khoảng giới hạn của kiểu int
	{
		if (num < 0) 	// nếu là số âm
		{
			kernel->synchConsoleOut->PutChar('-');	
			num = -num;
		}

		int n = 0;	// đếm số chữ số

		while (num)	// thực hiện đến khi num = 0
		{
			_numberBuffer[n++] = num % 10;		// lấy chữ số cuối lưu vào mảng
			num /= 10;				// bỏ chữ số cuối
		}
		
		// xuất số ra console bằng cách chuyển nó về kí tự
		for (int i = n - 1; i >= 0; --i)
			kernel->synchConsoleOut->PutChar(_numberBuffer[i] + '0');	
	}
}


int SysRandomNum()
{
	srand(time(NULL));
	return random() + 1; // random mot so nguyen lon hon 0, nen ta cong them 1.
}


void SysReadString (char* b, int length)
{
	int i;
	char c;

	for (i = 0; i < length; i++)
	{
		c = kernel->synchConsoleIn->GetChar();

		if (c == '\n')
			i--;
		else
			b[i] = c;	// lấy kí tự từ console vào mảng b

	}

	 b[i] = '\0';	// đánh dấu kí tự kết thúc
}


void SysPrintString (char* b)
{
	int i;

	for (i = 0; b[i] != '\0'; i++)
	{
		kernel->synchConsoleOut->PutChar(b[i]);		// xuất kí tự ra console
	}
}

//**************************************************************************************************************


int SysCreate(char* name)
{
	return kernel->fileSystem->Create(name);
}


OpenFileId SysOpen(char *name)
{
	return kernel->fileSystem->OpenFileID(name);
}


int SysClose(OpenFileId id)
{
	return kernel->fileSystem->Close(id);
}


int SysRead(char *buffer, int size, OpenFileId id)
{
	return kernel->fileSystem->Read(buffer, size, id);
}


int SysWrite(char *buffer, int size, OpenFileId id)
{
	return kernel->fileSystem->Write(buffer, size, id);
}


int SysSeek(int position, OpenFileId id)
{
	return kernel->fileSystem->Seek(position, id);
}


int SysRemove(char *name)
{
	return kernel->fileSystem->Remove(name);
}

#endif /* ! __USERPROG_KSYSCALL_H__ */