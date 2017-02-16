#ifndef MODULE6_H

typedef struct bootSector {
  uint8_t sectorsPerCluster, FATCopies;
  uint16_t bytesPerSector, reservedClusters, maxRd, totalSectors;
  uint16_t sectorsPerFAT, sectorsPerTrack, heads;
  uint32_t  sectorCount, volumeID;
  unsigned char bootSig;
  char volumeLabel[12], fileSystemType[9];
} bootSector;

typedef struct FAT {
  uint16_t FAT1[3072];
  uint16_t FAT2[3072];
} FAT;

typedef struct directory {
  char fileName[9], extension[4];
  uint8_t attributes, firstByte;
  uint16_t reserved, creationTime, creationDate, lastAccessDate, lastWriteTime;
  uint16_t lastWriteDate, firstLogicalCluster, fileSize; 
} directory;
;

void getBootSector(FILE *fp);
void printBootSector();

void printRootDirectory(FILE *fp);

void getDirectory(FILE *fp, int dirEntry);
void getDate(uint16_t date);
void getTime(uint16_t time);

void printDirectory();
void changeDirectory();
void listDirectory();
void printType();
void renameDirectory();
void initBoot();
void printRootDirectory();
void changeDirectory();
void listDirectory();
void type();
void renameFileOrDirectory();
void getFAT(FILE *fp);
void getFATEntry(int n);

void printByteData(FILE *fp);




#endif
