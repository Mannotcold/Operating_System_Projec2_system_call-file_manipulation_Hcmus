// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "synchconsole.h"
#include "syscall.h"
#include "ksyscall.h"
#include "openfile.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------

// Hàm tăng thanh ghi PC:
void PC_increase()
/* Modify return point */
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}


// Hàm chuyển đổi kernel sang users:
void kernel2users(int regAddr, int len, char* buffer)
{
	if (len <= 0) return; // Trường hợp len không thỏa mãn.

	int i = 0;
	int oneChar = 0;

	// Chuyển dữ liệu từ kernel sang users:
	do {
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(regAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
}


// Hàm chuyển đổi users sang kernel:
char* users2kernel(int regAddr)
{
	int length = 0;
    char* s;
	int oneChar;

	// Tính độ dài của string:
    while (1) {
        kernel->machine->ReadMem(regAddr + length, 1, &oneChar);
        length++;

        if (oneChar == '\0') break; // Điều kiện dừng.
    }

	// Cấp phát bộ nhớ cho string:
    s = new char[length];

	// Chuyển dữ liệu từ users sang kernel:
    for (int i = 0; i < length; i++) {
        kernel->machine->ReadMem(regAddr + i, 1, &oneChar);
        s[i] = (unsigned char)oneChar;
    }

    return s;
}


void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
		case NoException:
		{
			kernel->interrupt->setStatus(SystemMode); // Trả về system mode.
			return;
		}


		case PageFaultException:
		{
			DEBUG(dbgSys, "No valid translation found.\n");
			printf("No valid translation found.\n");
			SysHalt(); // Ngắt.
			return;
		}


		case ReadOnlyException:
		{
			DEBUG(dbgSys, "Write attempted to page marked read-only.\n");
			printf("Write attempted to page marked read-only.\n");
			SysHalt();
			return;
		}


		case BusErrorException:
		{
			DEBUG(dbgSys, "Translation resulted in an invalid physical address.\n");
			printf("Translation resulted in an invalid physical address.\n");
			SysHalt();
			return;
		}


		case AddressErrorException:
		{
			DEBUG(dbgSys, "Unaligned reference or one that was beyond the end of the address space.\n");
			printf("Unaligned reference or one that was beyond the end of the address space.\n");
			SysHalt();
			return;
		}


		case OverflowException:
		{
			DEBUG(dbgSys, "Integer overflow in add or sub.\n");
			printf("Integer overflow in add or sub.\n");
			SysHalt();
			return;
		}


		case IllegalInstrException:
		{
			DEBUG(dbgSys, "Unimplemented or reserved instr.\n");
			printf("Unimplemented or reserved instr.\n");
			SysHalt();
			return;
		}


		case NumExceptionTypes:
		{
			DEBUG(dbgSys, "NumExceptionTypes.\n");
			printf("NumExceptionTypes.\n");
			SysHalt();
			return;
		}

//*********************************************************************************
    case SyscallException:
      switch(type) {
		  case SC_ReadNum:
		  {
			  kernel->machine->WriteRegister(2, SysReadNum()); // Ghi xuống thanh ghi số 2 giá trị SysReadNum() trả về.
			  PC_increase(); // Tăng thanh ghi PC.
			  return;
		  }


		  case SC_PrintNum:
		  {
			  int n;
			  n = kernel->machine->ReadRegister(4); // Đọc từ thanh ghi số 4 rồi gán vào n.
			  SysPrintNum(n);
			  PC_increase();
			  return;
		  }


		  case SC_ReadChar:
		  {
			  kernel->machine->WriteRegister(2, SysReadChar()); // Ghi xuống thanh ghi số 2 giá trị SysReadChar() trả về.
			  PC_increase();
			  return;
		  }


		  case SC_PrintChar:
		  {
			  char c = kernel->machine->ReadRegister(4); // Đọc từ thanh ghi số 4 rồi gán vào c.
			  SysPrintChar(c);
			  PC_increase();
			  return;
		  }


		  case SC_RandomNum:
		  {
			  /* Process SysAdd Systemcall*/
			  kernel->machine->WriteRegister(2, int(SysRandomNum()));
			  PC_increase();
			  return;
		  }


		  case SC_ReadString:
		  {
			  char *buffer;
			  int len = kernel->machine->ReadRegister(5); // Đọc giá trị từ thanh ghi số 5 rồi gán vào len.
			 
			  if(len > 0) {
			  buffer = new char[len + 1]; // Cấp phát bộ nhớ.

			  SysReadString(buffer, len);
			  kernel2users(kernel->machine->ReadRegister(4), len, buffer);
			  
			  delete[] buffer; // Xóa buffer.
			  }

			  PC_increase();
			  return;
		  }


		  case SC_PrintString:
		  {
			  char* buffer;

			  buffer = users2kernel(kernel->machine->ReadRegister(4));
			  SysPrintString(buffer);

			  PC_increase();
			  delete[] buffer; // Xóa buffer.
			  return;
		  }

//************************************************************************************************************************
		case SC_Create:
		{
			char *buffer; // Mảng lưu tên file.

			buffer = users2kernel(kernel->machine->ReadRegister(4));
			kernel->machine->WriteRegister(2, SysCreate(buffer)); // Ghi kết quả trả về từ hàm SysCreate xuống thanh ghi số 2.

			PC_increase();
			delete[] buffer; // Xóa buffer.
			return;
		}

		case SC_Open:
		{
			char *buffer;

			buffer = users2kernel(kernel->machine->ReadRegister(4));
			kernel->machine->WriteRegister(2, SysOpen(buffer));

			PC_increase();
			delete[] buffer; // Xóa buffer.
			return;
		}


		case SC_Close:
		{
			int id;

			id = kernel->machine->ReadRegister(4);
			kernel->machine->WriteRegister(2, SysClose(id));

			PC_increase();
			return;
		}


		case SC_Read:
		{
			int strPtr = kernel->machine->ReadRegister(4);
			int size = kernel->machine->ReadRegister(5);
			int id = kernel->machine->ReadRegister(6);

			char *buffer = new char[size];
			kernel->machine->WriteRegister(2, SysRead(buffer, size, id));
			kernel2users(strPtr, size, buffer); // Chuyển buffer từ users sang kernel.
			
			PC_increase();
			delete[] buffer; // Xóa buffer.
			return;
		}


		case SC_Write:
		{
			char* buffer = users2kernel(kernel->machine->ReadRegister(4)); // Đọc buffer từ thanh ghi số 4 (users sang kernel).
			int size = kernel->machine->ReadRegister(5);
			OpenFileId id = kernel->machine->ReadRegister(6);

			kernel->machine->WriteRegister(2, SysWrite(buffer, size, id));

			PC_increase();
			delete[] buffer; // Xóa buffer.
			return;
		}


		case SC_Seek:
		{
			int position = kernel->machine->ReadRegister(4);
			OpenFileId id = kernel->machine->ReadRegister(5);

			kernel->machine->WriteRegister(2, SysSeek(position, id));

			PC_increase();
			return;
		}


		case SC_Remove:
		{
			char *buffer;

			buffer = users2kernel(kernel->machine->ReadRegister(4));
			kernel->machine->WriteRegister(2, SysRemove(buffer));

			PC_increase();
			delete[] buffer; // Xóa buffer.
			return;
		}
//************************************************************************************************************************
		  case SC_Halt:
			  DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			  printf("Shutdown, initiated by user program.\n");

			  SysHalt();

			  ASSERTNOTREACHED();
			  break;

		  case SC_Add:
			  DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

			  /* Process SysAdd Systemcall*/
			  int result;
			  result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
							  /* int op2 */ (int)kernel->machine->ReadRegister(5));

			  DEBUG(dbgSys, "Add returning with " << result << "\n");
			  /* Prepare Result */
			  kernel->machine->WriteRegister(2, (int)result);

			  PC_increase();

			  return;

			  ASSERTNOTREACHED();

			  break;

		  default:
			  cerr << "Unexpected system call " << type << "\n";
			  break;
      }
      break;
    default:
      cerr << "Unexpected user mode exception" << (int)which << "\n";
      break;
    }
    ASSERTNOTREACHED();
}
