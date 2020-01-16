#include "vfs.h"

UFDT UFDTArr[MAXINODE];
SUPERBLOCK SUPERBLOCKObj;
PINODE head = NULL;

//###################################################
//#Name of function         : man
//#Input parameters         : command name
//#Return value of function : void 
//#Description of function  : This function describes the use of the command. and provides info of how to use it.
//# use of function in project : To sheo help to user about how to use the commands.
//####################################################
void man( char *name )
{
	if (name == NULL)
	{
		return;
	}

	if (_stricmp(name, "create") == 0)
	{
		printf("Description: Used to create new regular file.\n");
		printf("create <file_name> <permission>\n");
	}
	else if (_stricmp(name, "read") == 0)
	{
		printf("Description: Used to read data from regular file.\n");
		printf("Usage: read <file_name> <no_of_bytes_to_read>\n");
	}
	else if (_stricmp(name, "write") == 0)
	{
		printf("Description: Used to write into reggular file.\n");
		printf("Usage:write <file_name> \n After this write the data we want to enter into file\n");
	}
	else if (_stricmp(name, "ls") == 0)
	{
		printf("Description: Used to list all information of file.\n");
		printf("Usage:ls\n");
	}
	else if (_stricmp(name, "stat") == 0)
	{
		printf("Description: Used to Display Information of file.\n");
		printf("Usage:stat <file_name>\n");
	}
	else if (_stricmp(name, "fstat") == 0)
	{
		printf("Description: Used to Display Information of file.\n");
		printf("Usage:fstat <file_desciptor>\n");
	}
	else if (_stricmp(name, "truncate") == 0)
	{
		printf("Description: Used to remove data from file.\n");
		printf("Usage:truncate <file_name>\n");
	}
	else if (_stricmp(name, "open") == 0)
	{
		printf("Description: Used to open the existing file.\n");
		printf("Usage: open <file_name>\n");
	}
	else if (_stricmp(name, "close") == 0)
	{
		printf("Description: Used to close a single opened file.\n");
		printf("Usage:close <file_name>\n");
	}
	else if (_stricmp(name, "closeall") == 0)
	{
		printf("Description: Used to close all opened files.\n");
		printf("Usage:closeall\n");
	}
	else if (_stricmp(name, "lseek") == 0)
	{
		printf("Description: Used to change the offset.\n");
		printf("Usage: lseek <file_name> <change_in_offset> <start_point>\n");
	}
	else if (_stricmp(name, "rm") == 0)
	{
		printf("Description: Used to delete the file.\n");
		printf("Usage: rm <file_Name> \n");
	}
	else
	{
		printf("ERROR: No manual entry available \n");
	}
}//end of man fun


 //###################################################
 //#Name of function         : DisplayHelp
 //#Input parameters         : void
 //#Return value of function : void
 //#Description of function  : this function displays help to user, it describes each command and its work.
 //# use of function in project :
 //####################################################
void DisplayHelp()
{
	printf("ls	     : To list out all the files.\n");
	printf("clear    : To clear the console.\n");
	printf("open     : To open the file.\n");
	printf("close    : To close the file.\n");
	printf("closeall : To close all opened files.\n");
	printf("read     : To read the contents of the file.\n");
	printf("write    : To write contents into the file.\n");
	printf("exit     : To terminate the system.\n");
	printf("stat     : To display information of file using name.\n");
	printf("fstat    : To display information of file using file descriptor.\n");
	printf("truncate : To remove all data from file.\n");
	printf("rm       : To delete the file.\n");
}


//###################################################
//#Name of function         : GetFDFromName
//#Input parameters         : Name of the file already created
//#Return value of function : int 
//#Description of function  : this function retrives the file descryptor of the associated file specified by name
//# use of function in project : To get the File Descriptor of a file.
//####################################################
int GetFDFromName( char *name )
{
	int i = 0;
	while (i<50)
	{
		if (UFDTArr[i].PtrFiletable != NULL)
		{
			if (_stricmp((UFDTArr[i].PtrFiletable->PtrInode->FileName), name) == 0)
			{
				break;
			}
		}
		i++;
	}//while
	if (i == 50)
	{
		return-1;
	}
	else
	{
		return i;
	}
}

 
//###################################################
//#Name of function         : GetInode
//#Input parameters         : Name of file
//#Return value of function : PINODE(Pointer to inode).
//#Description of function  : Ths function retrieves the pointer to inode.
//# use of function in project :to get the free inode available.
//####################################################
PINODE GetInode( char *name )
{
	PINODE temp = head;
	int i = 0;
	if (name == NULL)
	{
		return NULL;
	}
	while (temp != NULL)
	{
		if (_stricmp(name, temp->FileName) == 0)
		{
			break;
		}
		temp = temp->next;
	}
	return temp;
}



//###################################################
//#Name of function         : CreateDILB
//#Input parameters         : void
//#Return value of function : void
//#Description of function  : this function creates Disk Inode List of Blocks.
//# use of function in project : This function is used to create the list of the Disk Inode blocks.
//####################################################
void CreateDILB()
{
	int i = 1;
	PINODE newn = NULL;
	PINODE temp = head;
	while (i < MAXINODE)//	while (i < -MAXINODE)
	{
		newn = (PINODE)malloc(sizeof(INODE));

		newn->LinkCount = newn->ReferenceCount;
		newn->FileType = newn->FileSize = 0;
		newn->Buffer = NULL;
		newn->next = NULL;
		newn->InodeNumber = i;
		if (temp == NULL)
		{
			head = newn;
			temp = head;
		}
		else
		{
			temp->next = newn;
			temp = temp->next;
		}
		i++;
	}//while
	//printf("DILB craete success\n");
	i++;
}



//###################################################
//#Name of function         : InitialiseSuperBlock
//#Input parameters         : void
//#Return value of function : void
//#Description of function  : Used to initialise the superblock.
//# use of function in project : its used to initialise super block superblock contains number of free & used Disk Inodes.
//####################################################
void InitialiseSuperBlock()
{
	int i = 0;
	while (i < MAXINODE)
	{
		UFDTArr[i].PtrFiletable = NULL;
		i++;
	}
	SUPERBLOCKObj.TotalInodes = MAXINODE;
	SUPERBLOCKObj.FreeInodes = MAXINODE;
}



//###################################################
//#Name of function         : CreateFile
//#Input parameters         : filename,permission
//#Return value of function : int 
//#Description of function  : This function Creates the file in FileSystem.
//# use of function in project : Its used to create file with name and permissions.
//####################################################
int CreateFile(char *name, int permission)
{
	int i = 0;
	PINODE temp = head;
	if ((name == NULL) || (permission == 0) || (permission>3))
	{
		return -1;
	}
	if (SUPERBLOCKObj.FreeInodes == 0)
	{
		return -2;
	}
	if (GetInode(name) != NULL)
	{
		return -3;
	}
	(SUPERBLOCKObj.FreeInodes)--;

	while (temp != NULL)
	{
		if (temp->FileType == 0)
		{
			break;
		}
		else
		{
			temp = temp->next;
		}
	}
	while (i < 50)
	{
		if (UFDTArr[i].PtrFiletable == NULL)
		{
			break;
		}
		else
		{
			i++;
		}
	}
	UFDTArr[i].PtrFiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].PtrFiletable == NULL)
	{
		return -4;
	}

	UFDTArr[i].PtrFiletable->Count = 1;
	UFDTArr[i].PtrFiletable->Mode = permission;
	UFDTArr[i].PtrFiletable->ReadOffset = 0;
	UFDTArr[i].PtrFiletable->WriteOffset = 0;

	UFDTArr[i].PtrFiletable->PtrInode = temp;
	strcpy_s(UFDTArr[i].PtrFiletable->PtrInode->FileName, 50, name);//may be locha at 50 3rd param
																	//strcpy_s()

	UFDTArr[i].PtrFiletable->PtrInode->FileType = REGULAR;
	UFDTArr[i].PtrFiletable->PtrInode->ReferenceCount = 1;
	UFDTArr[i].PtrFiletable->PtrInode->LinkCount = 1;
	UFDTArr[i].PtrFiletable->PtrInode->FileSize = MAXFILESIZE;
	UFDTArr[i].PtrFiletable->PtrInode->FileActualSize = 0;
	UFDTArr[i].PtrFiletable->PtrInode->Permission = permission;
	UFDTArr[i].PtrFiletable->PtrInode->Buffer = (char*)malloc(MAXFILESIZE);

	memset(UFDTArr[i].PtrFiletable->PtrInode->Buffer, 0, 1024);
	return i;
}



//###################################################
//#Name of function         : rmFile
//#Input parameters         : name of file.
//#Return value of function : int 
//#Description of function  : this function is usedd to delete the file.
//# use of function in project : The file is completely deleted froom the file system.
//####################################################
int rmFile(char *name)
{
	int fd = 0;
	fd = GetFDFromName(name);
	if (fd == -1)
	{
		return -1;
	}

	(UFDTArr[fd].PtrFiletable->PtrInode->LinkCount)--;

	if (UFDTArr[fd].PtrFiletable->PtrInode->LinkCount == 0)
	{
		UFDTArr[fd].PtrFiletable->PtrInode->FileType = 0;
		free(UFDTArr[fd].PtrFiletable);
	}
	UFDTArr[fd].PtrFiletable = NULL;
	(SUPERBLOCKObj.FreeInodes)++;
}



//###################################################
//#Name of function         : Read
//#Input parameters         : Filedescriptor,Name of File,Size in bytes
//#Return value of function : int 
//#Description of function  : This function is used to Read the specified number of bytes from file.
//# use of function in project : This function reads the content of the existing file.
//####################################################
int Read(int fd, char* arr, int isize)
{
	int ReadSize = 0;
	if (UFDTArr[fd].PtrFiletable == NULL)
	{
		return -1;
	}
	if (UFDTArr[fd].PtrFiletable->Mode != READ && UFDTArr[fd].PtrFiletable->Mode != READ + WRITE)
	{
		return -2;
	}
	if (UFDTArr[fd].PtrFiletable->PtrInode->Permission != READ && UFDTArr[fd].PtrFiletable->PtrInode->Permission != READ + WRITE)
	{
		return -2;
	}
	if (UFDTArr[fd].PtrFiletable->ReadOffset == UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize)
	{
		return -3;
	}
	if (UFDTArr[fd].PtrFiletable->PtrInode->FileType != REGULAR)
	{
		return -4;
	}
	ReadSize = (UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) - (UFDTArr[fd].PtrFiletable->ReadOffset);
	if (ReadSize < isize)
	{
		strncpy(arr, (UFDTArr[fd].PtrFiletable->PtrInode->Buffer) + (UFDTArr[fd].PtrFiletable->ReadOffset), ReadSize);
		UFDTArr[fd].PtrFiletable->ReadOffset = UFDTArr[fd].PtrFiletable->ReadOffset + ReadSize;
	}
	else
	{
		strncpy(arr, (UFDTArr[fd].PtrFiletable->PtrInode->Buffer) + (UFDTArr[fd].PtrFiletable->ReadOffset), isize);
		(UFDTArr[fd].PtrFiletable->ReadOffset) = (UFDTArr[fd].PtrFiletable->ReadOffset) + isize;
	}
	return isize;
}

//###################################################
//#Name of function         : WriteFile
//#Input parameters         : FileDescriptor,file name,size in bytes to write.
//#Return value of function : int 
//#Description of function  : this function writes the content in the file.
//# use of function in project : This function writes entered data in a particular file.
//####################################################
int WriteFile(int fd, char* arr, int isize)
{
	if (((UFDTArr[fd].PtrFiletable->Mode) != WRITE) && ((UFDTArr[fd].PtrFiletable->Mode) != READ + WRITE))
	{
		return -1;
	}
	if (((UFDTArr[fd].PtrFiletable->PtrInode->Permission) != WRITE) && ((UFDTArr[fd].PtrFiletable->PtrInode->Permission) != READ + WRITE))
	{
		return -1;
	}
	if ((UFDTArr[fd].PtrFiletable->WriteOffset) == MAXFILESIZE)
	{
		return -2;
	}
	if ((UFDTArr[fd].PtrFiletable->PtrInode->FileType) != REGULAR)
	{
		return -3;
	}
	strncpy((UFDTArr[fd].PtrFiletable->PtrInode->Buffer) + (UFDTArr[fd].PtrFiletable->WriteOffset), arr, isize);
	(UFDTArr[fd].PtrFiletable->WriteOffset) = (UFDTArr[fd].PtrFiletable->WriteOffset) + isize;
	(UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) = (UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) + isize;

	return isize;
}

//###################################################
//#Name of function         : OpenFile
//#Input parameters         : name of file,mode
//#Return value of function : int 
//#Description of function  : This functin opens the specified file with the specified mode.
//# use of function in project : This function opens a file.
//####################################################
int OpenFile(char* name, int mode)
{
	int i = 0;

	PINODE temp = NULL;
	if (name == NULL || mode <= 0)
	{
		return -1;
	}
	temp = GetInode(name);
	if (temp == NULL)
	{
		return -2;
	}
	if (temp->Permission < mode)
	{
		return -3;
	}

	while (i < 50)
	{
		if (UFDTArr[i].PtrFiletable == NULL)
		{
			break;
		}
		i++;
	}
	UFDTArr[i].PtrFiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].PtrFiletable == NULL)
	{
		return -1;
	}
	UFDTArr[i].PtrFiletable->Count = 1;
	UFDTArr[i].PtrFiletable->Mode = mode;

	if (mode == READ + WRITE)
	{
		UFDTArr[i].PtrFiletable->ReadOffset = 0;
		UFDTArr[i].PtrFiletable->WriteOffset = 0;
	}
	else if (mode == READ)
	{
		UFDTArr[i].PtrFiletable->ReadOffset = 0;
	}
	else if (mode == WRITE)
	{
		UFDTArr[i].PtrFiletable->WriteOffset = 0;
	}
	UFDTArr[i].PtrFiletable->PtrInode = temp;
	(UFDTArr[i].PtrFiletable->PtrInode->ReferenceCount)++;

	return i;
}



//###################################################
//#Name of function         : CloseFileByFD
//#Input parameters         : FileDescriptor
//#Return value of function : void
//#Description of function  : This function is used to close the file using its file desccriptor.
//# use of function in project : This function closes the file specified by the file descriptor.
//####################################################
void CloseFileByFd(int fd)//closefilebyname
{
	UFDTArr[fd].PtrFiletable->ReadOffset = 0;
	UFDTArr[fd].PtrFiletable->WriteOffset = 0;
	(UFDTArr[fd].PtrFiletable->PtrInode->ReferenceCount)--;
	printf("close file by fd success");
}



//###################################################
//#Name of function         : CloseFileByName
//#Input parameters         : Name of File
//#Return value of function : int 
//#Description of function  : This function closes the file by using its File Name.
//# use of function in project : The file Specified by Name is closed.
//####################################################
int CloseFileByName(char* name)
{
	int i = 0;
	i = GetFDFromName(name);
	if (i == -1)
	{
		return -1;
	}

	UFDTArr[i].PtrFiletable->ReadOffset = 0;
	UFDTArr[i].PtrFiletable->WriteOffset = 0;
	(UFDTArr[i].PtrFiletable->PtrInode->ReferenceCount)--;
	printf("file closed successfully");
	return 0;
}


//###################################################
//#Name of function         : CloseAllFile
//#Input parameters         : void
//#Return value of function : void
//#Description of function  : This function is used to close all the opened files.
//# use of function in project : Its used to close all the currently opened file.
//####################################################
void CloseAllFile()
{
	int i = 0;
	while (i < 50)
	{
		if (UFDTArr[i].PtrFiletable != NULL)
		{
			UFDTArr[i].PtrFiletable->ReadOffset = 0;
			UFDTArr[i].PtrFiletable->WriteOffset = 0;
			(UFDTArr[i].PtrFiletable->PtrInode->ReferenceCount)--;
			break;
		}
		i++;
	}
	//printf("all files closed successully");
}


//###################################################
//#Name of function         : LseekFile
//#Input parameters         : FileDescriptor,size to move,start point to move
//#Return value of function : int 
//#Description of function  : This function is used to seek at some position in file.
//# use of function in project : This function is used to set pointer at some location in file.
//####################################################
int LseekFile(int fd, int size, int from)
{
	if (fd < 0 || from >2)
	{
		return -1;
	}
	if (UFDTArr[fd].PtrFiletable == NULL)
	{
		return -1;
	}

	if ((UFDTArr[fd].PtrFiletable->Mode == READ) || (UFDTArr[fd].PtrFiletable->Mode == WRITE))
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].PtrFiletable->ReadOffset) + size) > UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize)
			{
				return -1;
			}
			if (((UFDTArr[fd].PtrFiletable->ReadOffset) + size) < 0)
			{
				return -1;
			}
			(UFDTArr[fd].PtrFiletable->ReadOffset) = (UFDTArr[fd].PtrFiletable->ReadOffset) + size;
		}
		else if (from == START)
		{
			if (size > (UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize))
			{
				return -1;
			}
			if (size < 0)
			{
				return -1;
			}
			(UFDTArr[fd].PtrFiletable->ReadOffset) = size;
		}
		else if (from == END)
		{
			if ((UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) + size>MAXFILESIZE)
			{
				return -1;
			}
			if (((UFDTArr[fd].PtrFiletable->ReadOffset) + size) < 0)
			{
				return -1;
			}
			(UFDTArr[fd].PtrFiletable->ReadOffset) = (UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) + size;
		}
	}
	else if (UFDTArr[fd].PtrFiletable->Mode == WRITE)
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].PtrFiletable->WriteOffset) + size) > MAXFILESIZE)
			{
				return -1;
			}
			if (((UFDTArr[fd].PtrFiletable->WriteOffset) + size) < 0)
			{
				return -1;
			}
			if (((UFDTArr[fd].PtrFiletable->WriteOffset) + size) > (UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize))
			{
				(UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) = (UFDTArr[fd].PtrFiletable->WriteOffset) + size;
			}
			(UFDTArr[fd].PtrFiletable->WriteOffset) = (UFDTArr[fd].PtrFiletable->WriteOffset) + size;
		}
		else if (from == START)
		{
			if (size > MAXFILESIZE)
			{
				return -1;
			}
			if (size < 0)
			{
				return -1;
			}
			if (size >(UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize))
			{
				(UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) = size;
			}
			(UFDTArr[fd].PtrFiletable->WriteOffset) = size;
		}
		else if (from == END)
		{
			if ((UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) + size > MAXFILESIZE)
			{
				return -1;
			}
			if (((UFDTArr[fd].PtrFiletable->WriteOffset) + size) < 0)
			{
				return -1;
			}
			(UFDTArr[fd].PtrFiletable->WriteOffset) = (UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize) + size;
		}
	}
}//lseek


 //###################################################
 //#Name of function         : LsFile
 //#Input parameters         : void
 //#Return value of function : void
 //#Description of function  : This function is used to list all the available files in the directory.
 //# use of function in project : Its used to list out all the files in current working directory.
 //####################################################
void LsFile()
{
	int i = 0;
	PINODE temp = head;

	if (SUPERBLOCKObj.FreeInodes == MAXINODE)
	{
		printf("ERROR:there are no files\n");
		return;
	}
	printf("\n File Name \t Inode Number \t File Size \t Link Count\n");
	printf("---------------------------------------------------------\n");
	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			printf("%s \t\t %d \t\t %d \t\t %d \n", temp->FileName, temp->InodeNumber, temp->FileActualSize, temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("---------------------------------------------------------\n");
}//ISFile

 //###################################################
 //#Name of function         : Fstat
 //#Input parameters         : FileDescriptor
 //#Return value of function : int 
 //#Description of function  : Its used to display the statistical informtion of the file.
 //# use of function in project : it Displays the statistical information using the FileDescriptor.
 //####################################################
int Fstat(int fd)
{
	PINODE temp = head;
	int i = 0;
	if (fd < 0)
	{
		return -1;
	}
	if ((UFDTArr[fd].PtrFiletable) == NULL)
	{
		return -2;
	}
	temp = UFDTArr[fd].PtrFiletable->PtrInode;

	printf("\n-------------Statistical Information About File-------------\n");
	printf("File Name	:%s\n", temp->FileName);
	printf("Inode Number:%d\n", temp->InodeNumber);
	printf("File Size	:%d\n", temp->FileSize);
	printf("Actual File Size:%d\n", temp->FileActualSize);
	printf("Link Count	:%d\n", temp->LinkCount);
	printf("Reference Count:%d\n", temp->ReferenceCount);

	if (temp->Permission == 1)
	{
		printf("File permission : Read Only\n");
	}
	else if (temp->Permission == 2)
	{
		printf("File permission : Write\n");
	}
	else if (temp->Permission == 3)
	{
		printf("File permission : Read & Write \n");
	}
	printf("---------------------------------------------------------");
}//Fsytat


 //###################################################
 //#Name of function         : StatFile
 //#Input parameters         : Name of File
 //#Return value of function : int 
 //#Description of function  : Its used to display the statistical informtion of the file.
 //# use of function in project : it Displays the statistical information using the File Name.
 //####################################################
int StatFile(char *name)
{
	PINODE temp = head;
	int i = 0;
	if (name == NULL)
	{
		return -1;
	}

	while (temp != NULL)
	{
		if (strcmp(name, temp->FileName) == 0)
		{
			break;
		}
		temp = temp->next;
	}
	if (temp == NULL)
	{
		return -2;
	}

	printf("\n-------------Statistical Information About File-------------\n");
	printf("File Name:%s\n", temp->FileName);
	printf("Inode Number:%d\n", temp->InodeNumber);
	printf("File Size:%d\n", temp->FileSize);
	printf("Actual File Size:%d\n", temp->FileActualSize);
	printf("Link Count:%d\n", temp->LinkCount);
	printf("Reference Count:%d\n", temp->ReferenceCount);

	if (temp->Permission == 1)
	{
		printf("File permission : Read Only\n");
	}
	else if (temp->Permission == 2)
	{
		printf("File permission : Write\n");
	}
	else if (temp->Permission == 3)
	{
		printf("File permission : Read & Write \n");
	}
	printf("---------------------------------------------------------");

	return 0;
}//stat file

 //###################################################
 //#Name of function         : TruncateFile
 //#Input parameters         : Name of File
 //#Return value of function : int 
 //#Description of function  : This function is used to Truncate the data in file.
 //# use of function in project : This function deletes all the data in the file.
 //####################################################
int TruncateFile(char *name)
{
	int fd = GetFDFromName(name);
	if (fd == -1)
	{
		return -1;
	}
	memset(UFDTArr[fd].PtrFiletable->PtrInode->Buffer, 0, 1024);
	UFDTArr[fd].PtrFiletable->ReadOffset = 0;
	UFDTArr[fd].PtrFiletable->WriteOffset = 0;
	UFDTArr[fd].PtrFiletable->PtrInode->FileActualSize = 0;

	return 0;
}


//###################################################
//#Name of function         : main
//#Input parameters         : void
//#Return value of function : int 
//#Description of function  : This is the Entrypoint function used to drive the program.
//# use of function in project : This function drives the whole program eecution.
//####################################################
int main()
{
	/*
	char username[5][15] = {'\0'};
	char password[5][15]= {'\0'};;

	int iret = 0;
	iret = Check(username, password);
	if (iret == 0 || iret==6)
	{
	printf("enter valid details:EXITING the application");
	getchar();
	return 0;
	}
	if (iret == -1)
	{
	printf("exit called: EXITING THE APPLICATION\n");
	getchar();
	return 0;

	}
	*/
	/*
	char username[20] = "akshay";
	char password[20] = "akki";
	char usr[20];
	char pass[20];
	printf("enter username:\n");
	scanf("%s", usr);
	printf("enter password:\n");
	scanf("%s", pass);*/

	/*if (stricmp(username, usr)!=0 || stricmp(password, pass)!=0)
	{
	printf("not equals");
	return 0;
	}

	*/

	char *ptr = NULL;
	int ret = 0;
	int fd = 0;
	int count = 0;


	char command[4][80], str[80], arr[1024];
	InitialiseSuperBlock();
	CreateDILB();
	while (1)
	{
		fflush(stdin);
		strcpy_s(str, 80, "");

		printf("\nVirtual File System:>");
		fgets(str, 80, stdin);

		count = sscanf(str, "%s %s %s %s", command[0], command[1], command[2], command[3]);
		if (count == 1)
		{
			if (_stricmp(command[0], "ls") == 0)
			{
				LsFile();
			}
			else if (_stricmp(command[0], "closeall") == 0)
			{
				CloseAllFile();
				printf("All files closed successfully\n");
				continue;
			}
			else if (_stricmp(command[0], "clear") == 0)
			{
				system("cls");
				continue;
			}
			else if (_stricmp(command[0], "help") == 0)
			{
				DisplayHelp();
				continue;
			}
			else if (_stricmp(command[0], "exit") == 0)
			{
				printf("Terminating the marvellous virtual file system\n");
				break;
			}
			else
			{
				printf("\nERROR:Command not found!!!\n Enter help for references");
				continue;
			}
		}
		else if (count == 2)
		{
			if (_stricmp(command[0], "stat") == 0)
			{
				ret = StatFile(command[1]);
				if (ret == -1)
				{
					printf("ERROR :Incorect parameter\n");
				}
				if (ret == -2)
				{
					printf("ERROR :There no such file found\n");
					continue;
				}
			}
			else if (_stricmp(command[0], "fstat") == 0)
			{
				ret = Fstat(atoi(command[1]));
				if (ret == -1)
				{
					printf("ERROR :Incorect parameter\n");
				}
				if (ret == -2)
				{
					printf("ERROR :There no such file found\n");
					continue;
				}
			}
			else if (_stricmp(command[0], "close") == 0)
			{
				ret = CloseFileByName(command[1]);
				if (ret == -1)
				{
					printf("ERROR :There no such file found\n");
					continue;
				}
			}
			else if (_stricmp(command[0], "fclose") == 0)
			{
				CloseFileByFd(atoi(command[1]));
			}
			else if (_stricmp(command[0], "rm") == 0)
			{
				ret = rmFile(command[1]);
				if (ret == -1)
				{
					printf("ERROR :There no such file found\n");
					continue;
				}
			}
			else if (_stricmp(command[0], "man") == 0)
			{
				man(command[1]);
			}
			else if (_stricmp(command[0], "write") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR :Incorect parameter\n");
					continue;
				}
				printf("Enter the data:\n");
				scanf("%[^\n]", arr);
				ret = strlen(arr);
				if (ret == 0)
				{
					printf("ERROR :Incorect parameter\n");
					continue;
				}
				ret = WriteFile(fd, arr, ret);
				if (ret == -1)
				{
					printf("ERROR :permission detained\n");
				}
				if (ret == -2)
				{
					printf("ERROR :There is no sufficient memory to write\n");
				}
				if (ret == -3)
				{
					printf("ERROR :It is not regular file\n");
				}
			}
			else if (_stricmp(command[0], "truncate") == 0)
			{
				ret = TruncateFile(command[1]);
				if (ret == -1)
				{
					printf("ERROR :Incorect parameter\n");
				}
				else if (ret == 0)
				{
					printf("truncate success\n");
				}
				else
				{
					printf("ERROR :Command not found\n");
					continue;
				}
			}
		}
		else if (count == 3)
		{
			if (_stricmp(command[0], "create") == 0)
			{
				ret = CreateFile(command[1], atoi(command[2]));
				if (ret >= 0)
				{
					printf("File is successfully created with the decriptor:%d\n", ret);
				}
				if (ret == -1)
				{
					printf("ERROR:Incorrect parameters\n", ret);
				}
				if (ret == -2)
				{
					printf("ERROR:File already exits\n", ret);
				}
				if (ret == -3)
				{
					printf("ERROR:Memory allocation failure\n", ret);
					continue;
				}
			}
			else if (_stricmp(command[0], "open") == 0)
			{
				ret = OpenFile(command[1], atoi(command[2]));
				if (ret >= 0)
				{
					printf("File is successfully created with the decriptor:%d\n", ret);
				}
				if (ret == -1)
				{
					printf("ERROR:Incorrect parameters\n", ret);
				}
				if (ret == -2)
				{
					printf("ERROR:File not present\n", ret);
				}
				if (ret == -3)
				{
					printf("ERROR:Permission denied\n", ret);
					continue;
				}
			}
			else if (_stricmp(command[0], "read") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR:Incorrect parameter\n");
					continue;
				}
				ptr = (char*)malloc(sizeof(atoi(command[2])) + 1);
				if (ptr == NULL)
				{
					printf("ERROR:Memory allocation is failure\n");
					continue;
				}
				ret = Read(fd, ptr, atoi(command[2]));//int ReadFile(int, char*, int);
				if (ret == -1)
				{
					printf("ERROR:File not exiting\n", ret);
				}
				if (ret == -2)
				{
					printf("ERROR:Permission denied\n", ret);
				}
				if (ret == -3)
				{
					printf("ERROR:Reached at the end of file\n", ret);
				}
				if (ret == -4)
				{
					printf("ERROR:It is not regular file\n", ret);
				}
				if (ret == 0)
				{
					printf("ERROR:File empty\n");
				}
				if (ret > 0)
				{
					_write(2, ptr, ret);
				}
				continue;
			}
			else
			{
				printf("ERROR :Command not found!!!\n");
				continue;
			}
		}
		else if (count == 4)
		{
			if (_stricmp(command[0], "lseek") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR :Incorect parameter\n");
					continue;
				}
				ret = LseekFile(fd, atoi(command[2]), atoi(command[3]));
				if (ret == -1)
				{
					printf("ERROR :Unable to perform lseek\n");
				}
			}
			else
			{
				printf("ERROR :Command not found!!!\n");
				continue;
			}
		}
		else
		{
			printf("ERROR :Command not found!!!\n");
			continue;
		}
	}

	getchar();
	return 0;
}
