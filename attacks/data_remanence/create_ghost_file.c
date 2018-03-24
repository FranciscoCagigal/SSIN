#include <stdio.h>

//text example to write into file
char text[] = "This is mysensitive informatoin: My password is x and my bank account is y. please dont steal money.";

int main()
{
	// open file in USB PEN
	FILE* file_ptr = fopen("/media/name_of_pen/file_created.txt", "w");
	//write text to file	
	fwrite(text , 1 , sizeof(text), file_ptr );	
	//close file	
	fclose(file_ptr);
	//remove file
	if (remove("/media/MARIA JOSE/file_created.txt") == 0)
		printf("Deleted successfully\n");
	else
		printf("Unable to delete the file\n");

  return 0;
}
