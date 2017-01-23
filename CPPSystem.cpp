#include <iostream>
#include "FileDirectory.h"


using namespace std; 

int main()
{
	
	FileDirectory file;// object named "file" created

	//1. create and write a file:file 1 of 40 bytes 
	//file.create("file_1", 40);
	//2.create and write a file:file 2 of 200 bytes 
	//file.create("file_2", 200);
	//3.create and write a file:file 3 of 300 bytes 
	//file.create("file_3", 300);
	//4.create and write a file:file 4 of 500 bytes 
	file.create("file_4", 500);
	//5.delete file 2 
	//file.deletefile("file_2");
	//6.create and write a file:file 4 of 500 bytes 

	if (file.create("file_4", 500))
	{
		file.write("file_4", 500, "Data", 2017, 1, 17, 13, 8, 55);// write file directory attributes into file
	}

	//file.printDirectory;

	return 0;
	system("pause");
}

