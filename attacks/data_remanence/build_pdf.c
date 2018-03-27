#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// state machine to determine if it found the EOF of pdf
int stateMachine(int c, int state){
	switch(state){
		case 0:
			if(c==37)
				return 1;
			break;
		case 1:
			if(c==37)
				return 2;
			break;
		case 2:
			if(c==69)
				return 3;
			break;
		case 3:
			if(c==79)
				return 4;
			break;
		case 4:
			if(c==70)
				return 5;	
			break;
	}
	return 0;
}

int main()
{
	int c; // output variable of byte read from file
	char string[5]; // char array to store sequence of bytes so i can see if it matches what im looking for
	string[4]='\0'; // so string ends
	FILE *file, *new_file= NULL; // file is the file i read from, new_file is the output file where i store what i found
	file = fopen("/dev/sdb1", "r"); // open USB PEN. Attention: it may be different on your machine. Check with lsblk linux command
	bool flag=false; // flag used to know when i can store in my output file
	int counter=0; // for this example i dont want to write everything that is on USB PEN so i need a counter.
	int state = 0; // state in which the pdf read/write is
	int fileCounter=0; // counter of ppdfs found
 	char buf[15]; // array to store name of pdf

	// if file exists
	if (file) {
		// while there is still something to read from the file, i store the byte at variable c 
	    	while ((c = getc(file)) != EOF){
			/* 
			if size is equal or bigger than 4 i use strncpy so i can remove the position [0] and move the rest of the backwards.
			for example i have char string[10] = "abcdefghi\0". in order to receive the next letter, i need to remove the 1st letter so this happens when i use strncpy: string[10]="bcdefghii\0". Now i can replace the second i (position [8]) with my next letter so the sequence can be continued. 		
			*/
			if(strlen(string)>=4){
				strncpy(string, string +1,3);			
				string[3]=c;
			}
			//if not we just need to place the byte read in the sequence of bytes already read 
			else string[strlen(string)]=c;
			// Compare if my sequence i read from the file equals the sequence im looking for. If so activate the flag
			if(strcmp(string,"%PDF")==0){
				printf("Found an occurrence\n");
				flag=true;
				if(new_file!=NULL) // if it is still writing to a file, assume data is corrupt and close said file
					fclose(new_file);
				sprintf(buf, "output%d.pdf", fileCounter); // put name of next file (determined by the counter) on variable buf
				fileCounter++;
				new_file = fopen(buf, "w"); // open new file
				fwrite ("%PD" , 1, 3, new_file); // write the first bytes to it
			}
			// if flag is true i write the byte to the file
			if(flag==true){
				state=stateMachine(c,state); // get state in which the proccess of writing to the file is
				fputc(c, new_file);
				if(state==5){ // if it has reached state 5 then pdf was retrieved successfully so we can close it
					flag = false;
					fclose(new_file);
					printf("pdf retrieved successfully \n");
					new_file=NULL;
				}			
			}
		}	
	    	fclose(file);
		if(new_file!=NULL)
			fclose(new_file);
	}
  return 0;
}
