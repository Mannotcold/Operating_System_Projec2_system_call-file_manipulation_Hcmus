// openfile.h
//	Data structures for opening, closing, reading and writing to
//	individual files.  The operations supported are similar to
//	the UNIX ones -- type 'man open' to the UNIX prompt.
//
//	There are two implementations.  One is a "STUB" that directly
//	turns the file operations into the underlying UNIX operations.
//	(cf. comment in filesys.h).
//
//	The other is the "real" implementation, that turns these
//	operations into read and write disk sector requests.
//	In this baseline implementation of the file system, we don't
//	worry about concurrent accesses to the file system
//	by different threads.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef OPENFILE_H
#define OPENFILE_H

#include "copyright.h"
#include "utility.h"
#include "sysdep.h"

class OpenFile
{
public:
	char* fileName;

	OpenFile(int f) // open the file
	{
		file = f;
		currentOffset = 0;
		fileName = NULL;
	}
	OpenFile(int f, char* _fileName) // same, but with steroids
	{
		file = f;
		currentOffset = 0;
		int len = strlen(_fileName);
		fileName = new char[len + 1];
		strncpy(fileName, _fileName, len + 1);
	}
	~OpenFile() // close the file
	{
		Close(file);

		if (fileName) {
			delete[] fileName;
			fileName = NULL;
		}
	}

	int ReadAt(char *into, int numBytes, int position)
	{
		Lseek(file, position, 0);
		return ReadPartial(file, into, numBytes);
	}
	int WriteAt(char *from, int numBytes, int position)
	{
		Lseek(file, position, 0);
		WriteFile(file, from, numBytes);
		return numBytes;
	}
	int Read(char *into, int numBytes)
	{
		int numRead = ReadAt(into, numBytes, currentOffset);
		currentOffset += numRead;
		return numRead;
	}
	int Write(char *from, int numBytes)
	{
		int numWritten = WriteAt(from, numBytes, currentOffset);
		currentOffset += numWritten;
		return numWritten;
	}

	int Length() { Lseek(file, 0, 2); return Tell(file); }
	
	
	// Hàm dịch chuyển con trỏ trong file:
	int Seek(int position){
		// Kiểm tra tính hợp lệ của position:
		if (position < -1 || position > Length()) return -1;
		
		// Nếu vị trí bằng -1 thì di chuyển con trỏ về cuối file:
		if (position == -1) {
			currentOffset = Length();
			return currentOffset;
		}

		// Di chuyển con trỏ đến vị trí position:
		currentOffset = position;
		return currentOffset;
	}

private:
	int file;
	int currentOffset;
};

#endif // OPENFILE_H