#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int bit;
int a;
int bitArray[24];
int In=33;
int  Out=44;
float speedIn=55.2;
float speedOut=66.7;


int main (void)
	{	
		wiringPiSetup();
		pinMode (2, OUTPUT);
		pinMode (3, OUTPUT);
		pinMode (0, INPUT);


		digitalWrite (3,LOW);
		delay(1);
		digitalWrite (3,HIGH);
		
		bit = digitalRead(0);

		for (a = 0; a<24; a=a+1)

			{

				printf("%d",bit);
				bitArray[a] = bit;
				printf("Printing\n");
				printf("%d", bitArray[0]);


				digitalWrite (2, HIGH);
				delay(1);
				digitalWrite (2, LOW);
				delay(1);
				bit = digitalRead (0);
			
			}

/* Gettint Unux time*/
 time_t seconds;

   seconds = time(NULL);
   printf("Seconds since January 1, 1970 = %ld\n", seconds);


/* Putting data into the file */

		FILE *fptr;
		fptr = fopen("file.txt","a");
		if(fptr == NULL)
   			{		
     		 		printf("Error!, can not access the file, does it exist?\n");   
    		  		exit(1);             
   			} 
		fprintf(fptr,"Time=%ld,",seconds);
		fprintf(fptr,"In=%i,",In);
		fprintf(fptr,"Out=%i,",Out);
		fprintf(fptr,"SpeedIn=%.1f,",speedIn);	
		fprintf(fptr,"SpeedOut=%.1f\n,",speedOut);

		fclose(fptr);

		return 0;
	}
