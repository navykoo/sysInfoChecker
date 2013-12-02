/*******************************************************************
 Program Name: CMOS read/write/verify for mother board
 History of changes:

 Version     Date            Editor          Description
 --------    -----------     ---------       -----------------------
 01.00.00    Sep 27,2011     Shine Lin       The first creation

 *******************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/io.h>
#include <string.h>


#define PORT_ADDR0_W       0x70
#define PORT_ADDR0_R       0x71

#define PORT_ADDR1_W       0x72
#define PORT_ADDR1_R       0x73
#define BLOCK_LENGTH       0x100         
#define CMOS_LENGTH        0x200

#define VERSION            "1.00.00"

#define BYTE               unsigned char


void DisplayByte(const BYTE readBuffer[])
{
	int index;
	
	
	for(index=0;index<CMOS_LENGTH;index++)
	{
	
		if(!(index&0x0f))
			printf("\n%03X-",index);
		printf("%02X ",readBuffer[index]);
	}
	printf("\n");
}
		
int ReadCmosToBuffer(BYTE readBuffer[])
{
	int status;
	int index;
	BYTE *pRead;
	
	//enable right
	status= ioperm(PORT_ADDR0_W,4,1);
	if(status<0)
	{
		perror("ioperm set error");
		return -1;
	}
	
	pRead = readBuffer;
	//read PORT_ADDR0
	for(index=0;index<BLOCK_LENGTH;index++)
	{
		outb(index,PORT_ADDR0_W);
		*pRead = inb(PORT_ADDR0_R) & 0xff;
		pRead++;	
	}	
	
	//read PORT_ADDR1
	for(index=0;index<BLOCK_LENGTH;index++)
	{
		outb(index,PORT_ADDR1_W);
		*pRead = inb(PORT_ADDR1_R) & 0xff;
		pRead++;
	}	
	return 0;
}

int SaveBufferToFile(const BYTE buffer[], const char fileName[])
{
	FILE *pFile;
	
	
	pFile = fopen(fileName,"wb");
	if(pFile==NULL)
	{
			printf("%s can't be opened for write!\n",fileName);
			return -1;			
	}	
	fwrite(buffer,sizeof(BYTE),CMOS_LENGTH,pFile);
	fclose(pFile);
	return 0;	
}

void ShowHelp(void)
{
	
	printf("CMOS Read Tool %s, Copyright (c) 2011 AMD Suzhou GPU SLT\n",VERSION);
	printf("%s\n","*****************************************************");
	printf("%s\n","Parameter        Description");
	printf("%s\n","---------        -----------");
	printf("%s\n","r filename:      read CMOS as a specific .bin file.");
	printf("%s\n","v sourcename:    verify CMOS with a .bin source file.");	
	
}

int VerifyCmosWithFile(const BYTE cmosBuffer[], const char sourceFile[])
{
	FILE *pFile;
	BYTE sourceBuffer[CMOS_LENGTH];
	int index;
	int failCount = 0;
	
	//read source file to buffer
	memset(sourceBuffer,0x00,CMOS_LENGTH);	
	pFile = fopen(sourceFile,"rb");
	if(pFile==NULL)
	{
		printf("%s can't be opened for read!\n",sourceFile);
		return -1;		
	}
	fread(sourceBuffer,sizeof(BYTE),CMOS_LENGTH, pFile);
	
	//compare byte
	for(index=0;index<CMOS_LENGTH;index++)
	{
		if(sourceBuffer[index]!=cmosBuffer[index])
		{
			printf("FAIL Address:0x%02X, Source Data:0x%02X, CMOS Data:0x%02X\n",index,sourceBuffer[index],cmosBuffer[index]);	
			failCount++;
		}
	}
	printf("Total Fail:%d\n",failCount);	
	fclose(pFile);
	return failCount>0? -1:0;			
}


int main(int argc, char *argv[])
{
	int error;
	char *action;
	char ch;
	BYTE readBuffer[CMOS_LENGTH];
	
	printf("CMOS Read Tool %s, Copyright (c) 2011 AMD Suzhou GPU SLT\n",VERSION);	
	if(argc>=3)
	{
		action = argv[1];
		ch = tolower(action[0]);
		switch(ch)
		{
			//-----------------------------------
			//    Read CMOS as a file
			//-----------------------------------
			case 'r':
				memset(readBuffer,0x00,CMOS_LENGTH);
				if(ReadCmosToBuffer(readBuffer)<0)
				{	
					error = 1;
					break;
				}
				DisplayByte(readBuffer);
				if(SaveBufferToFile(readBuffer,argv[2])<0)
					error = 1;
				
				break;
			//---------------------------------
			//     Verify CMOS from a file
			//---------------------------------	
			case 'v':
				memset(readBuffer,0x00,CMOS_LENGTH);
				if(ReadCmosToBuffer(readBuffer)<0)
				{	
					error = 1;
					break;
				}
				if(VerifyCmosWithFile(readBuffer,argv[2])<0)
					error = 1;
				break;
			/*	
			case 'w':
				WriteCmosFromFile(argv[2]);
				break;
			*/
			default:
				error = 1;
				ShowHelp();
				break;				
		}	
	}
	else
	{
		ShowHelp();
		error = 1;
	}
	return error;

}
