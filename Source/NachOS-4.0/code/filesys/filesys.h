// filesys.h
//	Data structures to represent the Nachos file system.
//
//	A file system is a set of files stored on disk, organized
//	into directories.  Operations on the file system have to
//	do with "naming" -- creating, opening, and deleting files,
//	given a textual file name.  Operations on an individual
//	"open" file (read, write, close) are to be found in the OpenFile
//	class (openfile.h).
//
//	We define two separate implementations of the file system.
//	The "STUB" version just re-defines the Nachos file system
//	operations as operations on the native UNIX file system on the machine
//	running the Nachos simulation.
//
//	The other version is a "real" file system, built on top of
//	a disk simulator.  The disk is simulated using the native UNIX
//	file system (in a file named "DISK").
//
//	In the "real" implementation, there are two key data structures used
//	in the file system.  There is a single "root" directory, listing
//	all of the files in the file system; unlike UNIX, the baseline
//	system does not provide a hierarchical directory structure.
//	In addition, there is a bitmap for allocating
//	disk sectors.  Both the root directory and the bitmap are themselves
//	stored as files in the Nachos file system -- this causes an interesting
//	bootstrap problem when the simulated disk is initialized.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#ifndef FS_H
#define FS_H

#include "copyright.h"
#include "sysdep.h"
#include "openfile.h"

#ifdef FILESYS_STUB 		// Temporarily implement file system calls as 
				// calls to UNIX, until the real file system
				// implementation is available
const int MAX_SIZE = 256;
class FileSystem
{
public:
	FileSystem() {
		fileTable = new OpenFile*[MAX_SIZE];
	}

	~FileSystem() {
		delete[] fileTable;
	}

	bool Create(char *name)
	{
		int fileDescriptor = OpenForWrite(name);

		if (fileDescriptor == -1)
			return FALSE;
		Close(fileDescriptor);
		return TRUE;
	}

	// H??m m??? file:
	int OpenFileID(char *name)
	{
		if (!name) return -1;

		int fileDescriptor = OpenForReadWrite(name, false);
		
		if (fileDescriptor == -1) return -1;

		// Ki???m tra xem file c?? ??ang ???????c m??? hay ch??a:
		for (int i = 0; i < MAX_SIZE; i++)	
			if (fileTable[i] && strcmp(name, fileTable[i]->fileName) == 0)
				return -1;

		// T??m v??? tr?? tr???ng trong fileTable:
		for (int i = 0; i < MAX_SIZE; i++)
			if (!fileTable[i]) {
			 	fileTable[i] = new OpenFile(fileDescriptor, name);
				return i; // Tr??? v??? v??? tr?? n???u t??m th???y.
			}

		return -1; // N???u kh??ng t??m th???y v??? tr?? tr???ng th?? tr??? v??? -1.
	}
	
	OpenFile *Open(char *name)
	{
		int fileDescriptor = OpenForReadWrite(name, FALSE);

		if (fileDescriptor == -1)
			return NULL;
		return new OpenFile(fileDescriptor);
	}

	// H??m ????ng file:
	bool Close(int id){
		if (fileTable[id]) {
			delete fileTable[id];
			fileTable[id] = NULL;
			return true; // Close th??nh c??ng.
		}
		return false; // Close th???t b???i.
	}

	// H??m ?????c file:
	int Read(char *buffer, int size, int id) {
		// Tr?????ng h???p kh??ng th???a m??n:
		if (id < 0 || id >= MAX_SIZE || !fileTable[id] || size < 0) return -1;

		return fileTable[id]->Read(buffer, size);
	}

	int Write(char *buffer, int size, int id) {
		// Tr?????ng h???p kh??ng th???a m??n:
		if (id < 0 || id >= MAX_SIZE || !fileTable[id] || size < 0) return -1;

		return fileTable[id]->Write(buffer, size);
	}

	int Seek(int pos, int id) {
		// Tr?????ng h???p kh??ng th???a m??n:
		if (id < 0 || id >= MAX_SIZE || !fileTable[id]) return -1;

		return fileTable[id]->Seek(pos);
	}

	int Remove(char *name) {
		if (!name) return false;

		// Ki???m tra xem file c?? ??ang ???????c m??? hay kh??ng:
		for (int i = 0; i < MAX_SIZE; i++)
			if (fileTable[i] && strcmp(name, fileTable[i]->fileName) == 0)
				return false;
		
		// X??a file:
		return !Unlink(name);
	}

private:
	OpenFile **fileTable; // B???ng l??u ?????a ch??? file ??ang ???????c m???.
};

#else // FILESYS
class FileSystem {
  public:
    FileSystem(bool format);		// Initialize the file system.
					// Must be called *after* "synchDisk" 
					// has been initialized.
    					// If "format", there is nothing on
					// the disk, so initialize the directory
    					// and the bitmap of free blocks.

    bool Create(char *name, int initialSize);  	
					// Create a file (UNIX creat)

    OpenFile* Open(char *name); 	// Open a file (UNIX open)

    bool Remove(char *name);  		// Delete a file (UNIX unlink)

    void List();			// List all the files in the file system

    void Print();			// List all the files and their contents

  private:
   OpenFile* freeMapFile;		// Bit map of free disk blocks,
					// represented as a file
   OpenFile* directoryFile;		// "Root" directory -- list of 
					// file names, represented as a file
};

#endif // FILESYS

#endif // FS_H