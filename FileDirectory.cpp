#include <iostream>
#include "FileDirectory.h"
using namespace std;

/*constructor*/
FileDirectory::FileDirectory()
{//initialize enteries in filedirectory and FAT16 to be 0

	for (int i = 0; i <= 256; i++)
		FAT16[i] = 0;

	for (int i = 0; i <= 4; i++)
	{
		for (int j = 0; j <= 32; i++)
			filedirectory[i][j] = 0;
	}

}

/*Verify if there is an unused entery and enough unused clusters*/
bool FileDirectory::create(char filename[], int numberBytes)
{
	int i;
	int unusedCluster;// store the amount of empty clusters 
	int clusterSize = (numberBytes / 4);// deterine cluster size based on bytes
	if (clusterSize % 4 != 0)// check for remaining of cluster size 
	{
		clusterSize = clusterSize + 1;// add one more to the cluster size to account for remainder
	}

	//1.check if there is an unused entry in the file directory(first character of file name in file directory is zero)
	for (i = 0; i <= 4; i++)// loop through each file 
	{
		if (filedirectory[i][0] = 0)// Check first character of file directory 
		{
			//2. Check if there are enough unused clusters to store the file with the umber of b 
				for (int k = 0; k < 256; k++)// loop through thr data bytes (private:unisgned short int FAT16[256])
				{
					if (FAT16[k] == 0 || FAT16[k] == 1)// Checks for empty or unused clusters
					{
						unusedCluster++;//if space is empty count and store total empty spaces into a variable
					}
				}// end loop to check for empty and unused file size 

				if (unusedCluster < numberBytes)//compare the empty clusters (unusedCluster) to numberBytes to store
				{
					return false;//Return false if there are NOT enough unused clusters to store file with numberBytes
				}
				else return true;// Return true if file directory is not empty
		}// end loop to check for first character of file name  

	}// end loop to check though each file 
	if (i = 4)// if you hit the end of file with no empty character return false 
	return false; 
}

/*Check if file exist and change the clusters to 1*/
bool FileDirectory::deletefile(char filename[])
{
	int i,j,k;
	unsigned short int firstClusterAddress, ClusterAddress;
	unsigned short int storeClusterLocation[256];

	//0.check if the file to be deleted(i.e file name) is in file directory 
	for (i = 0; i < 4; i++)// loop through each file 
	{
		for (j = 0; j < 8; j++)// loop though first 8 bytes (i.e. filename[7:0])
			if (filedirectory[i][j] = filename[j])// Checking characters in file name i.e [f][i][l][e][_][n][a][m][e]
				break;

		if (j = 8)// if characters match,continue this function
		{
			filedirectory[i][0] == 0;// make fist charcater of file name in the file directory to be zero 

			firstClusterAddress = filedirectory[i][26] + filedirectory[i][27] << 8;// defines first sector address
			ClusterAddress = firstClusterAddress;// sets the defined cluster address defined above 
			for (k = 0; k < 256 && ClusterAddress < 0xFFF8; k++)// Loops with it paramaters: less than 256 and CLuster address is within Last cluster file 
			{
				storeClusterLocation[k] = ClusterAddress;// Store cluster address into a new array to be used for accessing specific address in FAT16
				ClusterAddress = FAT16[ClusterAddress];// LINKED LIST; FAT16 links to next address and stores it to CLusterAddress 
				//ClusterAddress is then a new value and goes back into the loop
			}
			for (int m = 0; m <= k; m++)
			{
				FAT16[storeClusterLocation[m]] = 1;
			}
			return true;// file was found and entery clusters were changed 
		}
	}
	if (i == 4)// no file was found with that name 
	return false;// file does not exist 
}

/*Read of data from data[] array from file with the specified name*/
bool FileDirectory::read(char filename[])
{
	int i,j,k,p;
	unsigned short int firstClusterAddress,ClusterAddress;
	unsigned short int nextClusteradreess;
	unsigned short int time, date, numberofBytes;
	unsigned short int storeClusterLocation[256];
	unsigned char fileData[1024];

	//0.check if file to be deleted filename[]is in directory 
	for (i = 0; i < 4; i++)// loop through each file 
	{
		for (j = 0; j < 8; j++)// loop though first 8 bytes (i.e. filename[7:0])
			if (filedirectory[i][j] = filename[j])// Checking characters in file name i.e [f][i][l][e][_][n][a][m][e]
				break;// break if it does match

		if (j = 8)// if characters match,continue this function
		{
			//found file name in directory matching the input file 
		
		//1.use file name to get file info from the file directory including date, time, number of bytes and first cluster address
			date = filedirectory[i][24] + filedirectory[i][25]<<8;
			time = filedirectory[i][22] + filedirectory[i][23]<<8;
			//numberofBytes = filedirectory[i][];
		
		//2.use first cluster address to get all the cluster address of this file FAT-16
			firstClusterAddress = filedirectory[i][26] + filedirectory[i][27] << 8;// defines first sector address
			ClusterAddress = firstClusterAddress;
			for (k = 0; k < 256 && ClusterAddress < 0xFFF8; k++)
			{
				storeClusterLocation[k] = ClusterAddress;
				ClusterAddress = FAT16[ClusterAddress];
			}
				//3.Use all cluster address to read the data from disk/flash 
				p = 0;
				for (int m = 0; m <= k; m++)
				{
					for (int L = 0; L < 4; L++)
					{
						fileData[p] = data[storeClusterLocation[m] * 4 + L];
						p = p + 1;
					}//end L loop
				}//end m loop 
		}//end j loop 
	}// end i loop
	if (i = 4)
		return false;
	else
		return true;
}

/*Write numberBytes bytes of data from data[]array into the file with specified file name*/
void FileDirectory::write(char filename[], int numbeBytes, char data[], int year, int month, int day, int hour, int minutes, int second)
{
	unsigned short int firstClusterAddress;
	unsigned short int nextClusterAddress;
	unsigned int allocateBytes;

	//0.look for first unused entry (o or 1) in Fat-16, store result in First cluster
	for (int i = 0; i <= 256; i++)
	{
		if (FAT16[i] == 0 || FAT16[i] == 1)
		{
			firstClusterAddress = i;
			break;
		}
	}

	//look for the nest unused entry (0 or 1) in the Fat-16, store results in Next cluster, write value to Fat-16
	allocateBytes = 512;
	while (allocateBytes < numbeBytes)
	{
		for (int j = firstClusterAddress; j <= 256; j++)
		{
			if (FAT16[j] == 0 || FAT16[j] == 1)
			{
				nextClusterAddress = j;
				break;
			}
		}
	}

	//write/update the file name, extension date time and file length and first cluster into first unused entry in filedirectory 
	unsigned int empty;
	for (int i = 0; i <4; i++)
	{
		if (filedirectory[i][0] == 0)//checks if the first cluster is empty
		{
			empty = i;// Stores number of file where location was empty 
			break;// Once found, leav if statment 
		}
	}
	for (int j = 0; j <= 8; j++)
	{
		//write file name 
		filedirectory[empty][j] = filename[j];
	}

		//write date
		filedirectory[empty][24] = (day + ((0xE0 & month << 5)));
		filedirectory[empty][25] = (((year - 1980)<<1) + month >> 3);

		//write time
		filedirectory[empty][23] = (hour << 3 + (minutes >> 3 & 0x07));
		filedirectory[empty][22] = (minutes << 5 & 0xE0) + second>>1;

		//write first sector address into directory[27:26]
		filedirectory[empty][27] = firstClusterAddress >> 8;
		filedirectory[empty][26] = firstClusterAddress;

		//write file length
		filedirectory[empty][31] = numbeBytes >> 24;
		filedirectory[empty][30]=  numbeBytes >> 16;
		filedirectory[empty][29]=  numbeBytes >> 8;
		filedirectory[empty][28] = numbeBytes;
}

/*Print all clusters of a file*/
void FileDirectory::printClusters(char filename[])
{
	int i, j, k;
	unsigned short int firstClusterAddress, ClusterAddress;
	unsigned short int storeClusterLocation[256];

	//0.check if the file to be printed; filename[]; is in the Directory. If not; return false
	for (i = 0; i < 4; i++)// loop through each file 
	{
		for (j = 0; j < 8; j++)// loop though first 8 bytes (i.e. filename[7:0])
			if (filedirectory[i][j] = filename[j])// Checking characters in file name i.e [f][i][l][e][_][n][a][m][e]
				break;// break if it does match

		if (j = 8)// if characters match,continue this function
		{
			//1.use the file name to get the file information from the File Directory; including the first cluster address;
			firstClusterAddress = filedirectory[i][26] + filedirectory[i][27] << 8;// defines first sector address	

			//2.use first cluster address to get all the cluster address of this file FAT-16
			ClusterAddress = firstClusterAddress;
			for (k = 0; k < 256 && ClusterAddress < 0xFFF8; k++)
			{
				storeClusterLocation[k] = ClusterAddress;
				ClusterAddress = FAT16[ClusterAddress];
			}
			
		}
	}
	// will have a cout funtion here to print clusters
}

/*print all the files in the directory*/
void FileDirectory::printDirectory()
{
	unsigned short int date, time;
	for (int i = 0; i <= 4; i++)
	{
		//If file name valid, print file directory and extension 
		if (filedirectory[i][0] != '0')
		{
			for (int j = 0; j < 8; j++)
			{
				cout << filedirectory[i][j] << endl;
			}
			cout << ".";
			for (int j = 7; j < 11; j++)
			{
				cout << filedirectory[i][j] << endl;
			}
		//Print file date 
			date = (filedirectory[i][25] << 8) + filedirectory[i][24];
			cout << ((1980 + (date & 0xFE00))>>9);// print year
			cout << '/';
			cout << ((date & 0x01E0)>>5);// print month 
			cout << '/';
			cout << (date & 0x001F);// print day 

		//print file time 
			time = (filedirectory[i][23] << 8) + filedirectory[i][22];
			//add the stuff here 
			cout << ((time & 0xF800) >> 11);// print hour
			cout << ':';
			cout << ((time & 0x07E0) >> 5);//print min
			cout << ':';
			cout << (time & 0x001F);//print sec

		}
		cout << endl;
	}
}

void FileDirectory::printData(char filename[])
{
}