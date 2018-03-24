#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int main()
{
	int c; // output variable of byte read from file
	char string[24]; // char array to store sequence of bytes so i can see if it matches what im looking for
	string[23]='\0'; // so string ends
	FILE *file, *new_file; // file is the file i read from, new_file is the output file where i store what i found
	file = fopen("/dev/sdb1", "r"); // open USB PEN. Attention: it may be different on your machine. Check with lsblk linux command
	new_file = fopen("output.txt", "w"); // open output file
	bool flag=false; // flag used to know when i can store in my output file
	int counter=0; // for this example i dont want to write everything that is on USB PEN so i need a counter.

	// if file exists
	if (file) {
		// while there is still something to read from the file, i store the byte at variable c 
	    	while ((c = getc(file)) != EOF){

			/* 
			if size is equal or bigger than 23 i use strncpy so i can remove the position [0] and move the rest of the backwards.
			for example i have char string[10] = "abcdefghi\0". in order to receive the next letter, i need to remove the 1st letter so this happens when i use strncpy: string[10]="bcdefghii\0". Now i can replace the second i (position [8]) with my next letter so the sequence can be continued. 		
			*/
			if(strlen(string)>=23){
				strncpy(string, string +1,22);			
				string[22]=c;
			}
			//if not we just need to place the byte read in the sequence of bytes already read 
			else string[strlen(string)]=c;
			// Compare if my sequence i read from the file equals the sequence im looking for. If so activate the flag
			if(strcmp(string,"mysensitive informatoin")==0){
				printf("Found an occurrence\n");
				flag=true;
			}
			// if flag is true i write the byte to the file. As i said above i dont want to write the rest of the file so i start my counting
			if(counter==100){
				flag=false;
				counter=0;
			}	
			// for this example, when my counter reaches 100, i stop writing to the file beacuse i consider i have all the relevant information
			if(flag==true){
				fputc(c, new_file);
				counter++;			
			}
		}
		
	    	fclose(file);
		fclose(new_file);
	}
  return 0;
}
