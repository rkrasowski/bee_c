#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Configuration variable*/
int numOfChannels = 12;

/* Proram variable */



int bit;
int loopA;
int sensorNr;
int in;
int out;
int inOutArray[24];
int bitArray[24];
int timeArray[24];
float timeBeeIn;
float timeBeeOut;



int In=33;
int  Out=44;
float speedIn=55.2;
float speedOut=66.7;

int counter();


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

		for (sensorNr =0; sensorNr<24; sensorNr=sensorNr+1)

			{

			/*	printf("%d",bit);*/

				bitArray[sensorNr] = bit;
			/*	printf("%d", bitArray[0]) ;*/

				digitalWrite (2, HIGH);
				delay(1);
				digitalWrite (2, LOW);
				delay(1);
				bit = digitalRead (0);
			}

/* Gettint Unux time*/
 time_t seconds;

   seconds = time(NULL);
   printf("Seconds:  %ld\n", seconds);


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

		bitArray[23] = 5;


/* Data analysis  */

		counter(sensorNr,bitArray);


		return 0;
	}

/* ####################  Functions ####################*/



int counter(int sensorNrIns, int bitArray[]) 
	{
		 for ( loopA=0; loopA<= numOfChannels*2; loopA++)
                {
                        printf("Sensor number: %d\n",loopA);

			
                        /* Check for even number */
                        if(loopA % 2 == 0)    
                                {
                                        printf(" It is even number\n");







                                }

			else 
				{
					printf(" It is odd number\n");
				}	

                }

	/*	printf("Inside function %d", bitArray[1]);*/
		return 0;
	}


