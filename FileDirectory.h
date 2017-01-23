#pragma once

#include <iostream>
using namespace std;

class FileDirectory
{
	
public:
	FileDirectory();
	bool create(char filename[], int numberBytes);
	bool deletefile(char filename[]);
	bool read(char filename[]);
	void write(char filename[], int numbeBytes, char data[], int year, int month, int day, int hour, int minutes, int second);
	void printClusters(char filename[]);
	void printDirectory();
	void printData(char filename[]);

private:
	unsigned char filedirectory[4][32]; // 2-D array, creates 4 files each with 32bytes 
	unsigned short int FAT16[256];// Designates the FAT16 to be 256 clusters, array size of 257
	unsigned char data[1024];// 256clusters*4bytes/cluster=1,024bytes, Array size of 1024
};
