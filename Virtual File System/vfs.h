#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<io.h>

/// include guards
#ifndef __HH_VFS_H__
#define __HH_VFS_H__
//////////////////////////////////////////////////////////

#define MAXINODE 50 // maximum count of inodes

#define READ 1
#define WRITE 2

#define MAXFILESIZE 1024 // maximum file size

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

/// structure for storing superblock information
typedef struct superblock
{
	int TotalInodes;
	int FreeInodes;
}SUPERBLOCK, *PSUPERBLOCK;


/// structure for inode
typedef struct node
{
	char FileName[50];
	int InodeNumber;
	int FileSize;
	int FileActualSize;
	int FileType;
	char *Buffer;
	int LinkCount;
	int ReferenceCount;
	int Permission;
	struct node* next;
}INODE, *PINODE, **PPINODE;


/// structure for filetable
typedef struct filetable
{
	int ReadOffset;
	int WriteOffset;
	int Count;
	int Mode;
	PINODE PtrInode;
}FILETABLE, *PFILETABLE;

/// structure for user file descriptor table
typedef struct ufdt
{
	PFILETABLE PtrFiletable;
}UFDT;


/// functions declaration
void man( char* );
void DisplayHelp( );
int GetFDFromName( char* );
PINODE GetInode( char* );
void CreateDILB( );
void InitialiseSuperBlock( );
int CreateFile( char*, int );
int rmFile( char* );
int Read( int, char*, int );
int WriteFile( int, char*, int );
int OpenFile( char*, int );
void CloseFileByFd( int );
int CloseFileByName( char* );
void CloseAllFile( );
int LseekFile( int, int, int );
void LsFile( );
int FstatFile( int );
int StatFile( char* );
int TruncateFile( char* );
//////////////////////////////////////////////////////////
#endif //#ifndef __HH_VFS_H__
