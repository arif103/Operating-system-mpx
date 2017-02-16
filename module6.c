//Dirty Mike and the Boys
//CS 450
//Module 6

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "module6.h"

bootSector *boot;
FAT *FAT12;
directory *dir;

//Note: Remember when changing a filename on the disk image that
//file and directory names and extensions ALWAYS occupy 8bytes
//also applies to 3-character extensions.
//File and directory names and extensions are ALWAYS uppercase,
//so ALWAYS convert given file names/directories to uppercase

int main(int argc, char *argv[]) { 
    if (argc < 2 || argc > 3) {
	printf("Incorrect number of inputs\n");
	exit(0);
    }
    else if (!fopen(argv[1], "rw")){
	printf("Incorrect Disk Image File path\n");
	exit(0);
    }
    
    else if (argc == 3 && !fopen(argv[2], "rw")){
	printf("Incorrect File Path\n");
	exit(0);
    }

    boot = malloc(sizeof(bootSector));
    FAT12 = malloc(sizeof(FAT));
    dir = malloc(sizeof(directory));
    char *diskImg = argv[1], *fileName = argv[2];
    FILE *fp;
    int n = 0, choice;
    
    fp = fopen(diskImg, "rw");
    getBootSector(fp);

    printf("\n-----------------------------\n");
    printf("Please Select a menu option\n");
    printf("1)Print Boot Sector Information\n2)Print Root Directory\n3)Change Directory\n4List Directory\n5)Type\n6)Rename\n7)Display Options\n8)Exit\n");
    
    while (n == 0) {
	
	scanf("%d", &choice);

	switch(choice) {

	case 1 :
	    printBootSector();
            break;

        case 2  :
            printRootDirectory(fp);
            break;
        case 3  :
            //changeDirectory();
            break;
        case 4  :
            //listDirectory();
            break;
        case 5  :
            //type();
            break;
        case 6 :
           // rename();
            break;
	case 7:
	     printf("1)Print Boot Sector Information\n2)Print Root Directory			\n3)Change Directory\n4List Directory\n5)Type\n6)Rename\n			7)Display Options\n8)Exit\n");
	     break;
	case 8 :
	    exit(0);
	default	:
	    printf("Incorrect Command\n");
	    break;
	}
    } 
    getFAT(fp);
    
    
   
    
    //getDirectory(fp, 25);
    //printDirectory();

    //printRootDirectory(fp);

    fclose(fp);

    //printf("%s\n", getDate);
    return 0;
}

void printRootDirectory(FILE *fp) {

    int i;
    
    printf("--------------------------------------------------\n");
    printf("      		ROOT DIRECTORY                \n");
    printf("--------------------------------------------------\n");
    printf("PERMISSIONS     FILENAME   EXT   FILESIZE\n");
    printf("--------------------------------------------------\n");
    
    for (i = 0; i < 224; i++) {
	getDirectory(fp, i);
	//printf("Directory Entry %d First Byte: %d\n", i, dir->firstByte);
	       
	if (dir->firstByte != 0x05 || dir->firstByte != 0x00) {
	     
	    getAttributes(dir->attributes);
	    printf("\t\t%8s .%3s\t %d  BYTES\n", dir->fileName, dir->extension, dir->fileSize);
	}
	
    }
}

//Prints the attributes of the current file where R is ready only
//H is Hidden, S is system, D is a subdirectory
void getAttributes(uint8_t attr){
    uint8_t readOnly = attr & 0x01, hidden = attr & 0x02;
    uint8_t system = attr & 0x04, subDirectory = attr & 0x10;
    if (readOnly == 1) 
	printf("R");
    if (hidden == 2) 
	printf("H");
    if (system == 4)
	printf("S");
    if (subDirectory == 16)
	printf("D"); 
}

void getDate(uint16_t date) {
    uint16_t month, day, year;
    day = date & 0x001F;
    month = date & 0x01E0;
    month = month >> 5;
    year = date & 0xFE00;
    year = year >> 9;
    year = year + 1980;
    
    printf("%d\\%d\\%d\n", month, day, year);
}

void getTime(uint16_t time){
    uint16_t second, minute, hour;
    second = time & 0x001F;
    minute = time & 0x07E0;
    minute = minute >> 5;
    hour = time & 0xF800;
    hour = hour >> 11;

    printf("%d:%d:%d\n", hour, minute, second);
}

//Gets current Directory from directory entry number starts at 0
void getDirectory(FILE *fp, int dirEntry) {
    int i, offSet = 9728 + dirEntry * 32;
    uint8_t buff8[1];
    uint16_t buff16[2];
    uint32_t buff32[4];
    unsigned char buffU[3];
    char buff[8];

    fseek(fp, offSet, SEEK_SET);
    fread(&buff8[0], 1, 1, fp);
    dir->firstByte = buff8[0];

    fseek(fp, offSet, SEEK_SET);
    fread(&buff[0], 8, 1, fp);
   

    if (buff == 0){
        for (i=0; i < 7; i++){
	    buff[i] = '0';
	}
	buff[7] = '\n';
    }
    else {
	for (i = 0; i < 8; i++){
	    if (isspace(buff[i])){
		buff[i] = '\0';
	    }
	}
	strcpy(dir->fileName, &buff[0]);
    }
    //strcpy(dir->fileName, &buff[0]);
    
    fseek(fp, offSet + 8, SEEK_SET);
    fread(&buffU[0], 3, 1, fp);
    for (i = 0; i < 3; i++){
	if (isspace(buffU[i])){
	    buffU[i] = '\0';
	}
    }
    strcpy(dir->extension, &buffU[0]);
    
    fseek(fp, offSet + 11, SEEK_SET);
    fread(&buff8[0], 1, 1, fp);
    dir->attributes = buff8[0];
    
    fseek(fp, offSet + 12, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    dir->reserved = buff16[0];
    
    fseek(fp, offSet + 14, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);

    
    //printf("Buffer: %d\n", buff16[0]);
    
    dir->creationTime = buff16[0];

    // printf("Creation Time: %d\n", dir->creationTime);
    
    fseek(fp, offSet + 16, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    dir->creationDate = buff16[0];

    fseek(fp, offSet + 18, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    dir->lastAccessDate = buff16[0];

    fseek(fp, offSet + 22, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    dir->lastWriteTime = buff16[0];

    fseek(fp, offSet + 24, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    dir->lastWriteDate = buff16[0];
    
    fseek(fp, offSet + 26, SEEK_SET);
    fread(&buffU[0], 2, 1, fp);

    //printf("First Logical Cluster: %d\n", buff16[0]);
    dir->firstLogicalCluster = buffU[0];

    //printf("First Logica Cluster: %d\n", dir->firstLogicalCluster);
    
    fseek(fp, offSet + 28, SEEK_SET);
    fread(&buff32[0], 2, 1, fp);
    dir->fileSize = buff32[0];
}

//Prints current directory
void printDirectory() {
    printf("First Byte: 0x%2x\n", dir->firstByte); 
    printf("FileName: %s\n", dir->fileName);
    printf("Extension: %s\n", dir->extension);
    //printf("Attributes: %d\n", dir->attributes);

    printf("Attributes: ");
    getAttributes(dir->attributes);
    printf("\n");
    
    printf("Reserved: %d\n", dir->reserved);
    //printf("Creation Time: %d\n", dir->creationTime);

    printf("Creation Time: ");
    getTime(dir->creationTime);
    
    //printf("Creation Date: %d\n", dir->creationDate);

    printf("Creation Date: ");
    getDate(dir->creationDate);
    
    //printf("Last Access Date: %d\n", dir->lastAccessDate);

    printf("Last Access Date: ");
    getDate(dir->lastAccessDate);
    
    //printf("Last Write Time: %d\n", dir->lastWriteTime);

    printf("Last Write Time: ");
    getTime(dir->lastWriteTime);
    
    //printf("Last Write Date: %d\n", dir->lastWriteDate);

    printf("Last Write Date: ");
    getDate(dir->lastWriteDate);
    
    printf("First Logical Cluster: 0x%2x\n", dir->firstLogicalCluster);
    printf("File Size in Bytes: %d\n", dir->fileSize);
}

//Populates FAT array with FAT entries
void getFAT(FILE *fp) {
    int currLoc = 512, entryNum, lowOffset, highOffset, byteOffset;
    uint8_t buff[1];
    uint16_t entry, low, high, byte;
    
    for (entryNum = 0; entryNum < 3072; entryNum++) {	
	if (entryNum % 2 == 0) {
	    lowOffset =  512 + 1 + (3 * entryNum) / 2;
	    fseek(fp, lowOffset, SEEK_SET);
	    fread(&buff[0], 1, 1, fp);	    
	    low = buff[0];
	    low = low & 0x000F;
	    low = low << 8;
	    byteOffset = 512 + (3 * entryNum) / 2;
	    fseek(fp, byteOffset, SEEK_SET);
	    fread(&buff[0], 1, 1, fp);
	    byte = buff[0];
	    byte = byte & 0x00FF;
	    entry = low | byte;
	}	
	else if (entryNum % 2 != 0) {
	    highOffset = 512 + (3 * entryNum) / 2;
	    fseek(fp, highOffset, SEEK_SET);
	    fread(&buff[0], 1, 1, fp);
	    high = buff[0];
	    high = high & 0x00F0;
	    high = high >> 4;
	    byteOffset = 512 + 1 + (3 * entryNum) / 2;
	    fseek(fp, byteOffset, SEEK_SET);
	    fread(&buff[0], 1, 1, fp);
	    byte = buff[0];
	    byte = byte & 0x00FF;
	    byte = byte << 4;
	    entry = high | byte;	    
	}
	FAT12->FAT1[entryNum] = entry;
	FAT12->FAT2[entryNum] = entry;
	currLoc++;
	
	//printf("FAT Entry %d: 0x%2x\n", entryNum, FAT12->FAT1[entryNum]);
    }   
}


void getBootSector(FILE *fp) {
    initBoot();
    int i;
    uint8_t buff8[10];
    uint16_t buff16[10];
    uint32_t buff32[20];
    unsigned char buffU[5];
    char buff[11], *b;

    fseek(fp, 11, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    boot->bytesPerSector = buff16[0];

    fseek(fp, 13, SEEK_SET);
    fread(&buff8[0], 1, 1, fp);
    boot->sectorsPerCluster = buff8[0];

    fseek(fp, 14, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    boot->reservedClusters = buff16[0];

    fseek(fp, 16, SEEK_SET);
    fread(&buff8[0], 2, 1, fp);
    boot->FATCopies = buff8[0];

    fseek(fp, 17, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    boot->maxRd = buff16[0];

    fseek(fp, 19, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    boot->totalSectors = buff16[0];

    fseek(fp, 22, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    boot->sectorsPerFAT = buff16[0];

    fseek(fp, 24, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    boot->sectorsPerTrack = buff16[0];

    fseek(fp, 26, SEEK_SET);
    fread(&buff16[0], 2, 1, fp);
    boot->heads = buff16[0];

    fseek(fp, 32, SEEK_SET);
    fread(&buff32[0], 4, 1, fp);
    boot->sectorCount = buff32[0];

    fseek(fp, 38, SEEK_SET);
    fread(&buffU[0], 1, 1, fp);
    boot->bootSig = buffU[0];

    fseek(fp, 39, SEEK_SET);
    fread(&buff32[0], 2, 1, fp);
    boot->volumeID = buff32[0];
 
    fseek(fp, 43, SEEK_SET);
    fread(&buff[0], 11, 1, fp);
    for (i = 0; i < 11; i++){
	if (isspace(buff[i])){
	    buff[i] = '\0';
	}
    }
    strcpy(boot->volumeLabel, &buff[0]);

    fseek(fp, 54, SEEK_SET);
    fread(&buff[0], 8, 1, fp);
    for (i = 0; i < 8; i++) {
	if (isspace(buff[i])) {
	    buff[i] = '\0';
	}
    }
    strcpy(boot->fileSystemType, &buff[0]);
}

void printBootSector() {
    printf("BOOT SECTOR\n");
    printf("----------\n");
    printf("Bytes per sector: %d\n", boot->bytesPerSector);
    printf("Sectors per cluster: %d\n", boot->sectorsPerCluster);
    printf("Reserved Clusters: %d\n", boot->reservedClusters);
    printf("FAT Copies: %d\n", boot->FATCopies);
    printf("Max number of Root Directory Entries: %d\n", boot->maxRd);
    printf("Total number of sectors in File System: %d\n", boot->totalSectors);
    printf("Sectors per FAT: %d\n", boot->sectorsPerFAT);
    printf("Sectors per Track: %d\n", boot->sectorsPerTrack);
    printf("Number of Heads: %d\n", boot->heads);
    printf("Total Sector Count: %d\n", boot->sectorCount);
    printf("Boot Signature: 0x%2x\n", boot->bootSig);
    printf("Volume ID: %d\n", boot->volumeID);
    printf("Volume Label: %s\n", boot->volumeLabel);
    printf("File System Type: %s\n", boot->fileSystemType);
}

//Gets the contents of the Root Directory
void getRootDirectory(FILE *fp) {

}






//Changes to a subdirectory 
void changeDirectory() {

}

//Prints entire contents of current directory
void listDirectory() {

}

//Prints the contents of any file with the extension .TXT .BAT or C
void printType() {

}

//Renames a file or directory
void renameDirectory() {

}

void initBoot() {
    boot->sectorsPerCluster = 0;
    boot->FATCopies = 0;
    boot->bytesPerSector = 0;
    boot->reservedClusters = 0;
    boot->maxRd = 0;
    boot->totalSectors = 0;
    boot->sectorsPerFAT = 0;
    boot->sectorsPerTrack = 0;
    boot->heads = 0;
    boot->sectorCount = 0;
    boot->volumeID = 0;
    boot->bootSig = NULL;
}

//Prints the entire stream of Bytes found on the image file
void printByteData(FILE *fp) {
    

}



