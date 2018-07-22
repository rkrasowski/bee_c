#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/* Configuration variable*/
int numOfChannels = 12;

/* Proram variable */



int bit;
int loopA;
int sensorNr;
int in = 0;
int out = 0;
int inOutArray[24];
int bitArray[24];
long long int timeArray[24];
long long int timeDiff;
long long int timeInArray[10000];
long long int  timeOutArray[10000];
int timeInNumberArray = 0;
int timeOutNumberArray = 0;



int In=33;
int  Out=44;
float speedIn=55.2;
float speedOut=66.7;

int counter();
long long int getMicrotime();

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

		for (sensorNr =0; sensorNr<23; sensorNr=sensorNr+1)

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




/* Just for testing */

                bitArray[0] = 1;
                inOutArray[2] = 0;
		bitArray[3] = 0;




/* Data analysis  */

                counter(sensorNr,bitArray);



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



		return 0;
	}

/* ####################  Functions ####################*/



int counter (int sensorNrIns, int bitArray[], long long timeInArray[]) 
	{
		 for ( loopA=0; loopA<= numOfChannels*2-1; loopA++)
                {
                        printf("\n\nSensor number: %d, ",loopA);
			printf("BitArray value: %d\n",bitArray[loopA]);


                        /* Check for even number - bee is going from Inside -> Outside*/
                        if(loopA % 2 == 0)
                                {
					printf("In even routiine\n");
					printf("bitArray+1:%d ",bitArray[loopA+1]);
					printf("inOutArray:%d ",inOutArray[loopA]);
					printf("inOutArray+1:%d ",inOutArray[loopA+1]);

					/* Case of bee walking from outside to inside, first sensor triggered */
					if (bitArray[loopA] == 1 && bitArray[loopA+1] == 0 && inOutArray[loopA] == 0 && inOutArray[loopA+1] == 0)
						{
							inOutArray[loopA] = 1;
							inOutArray[loopA+1] = 1;
							timeArray[loopA] = getMicrotime();
						}
					/* Case of bee walking in and second sensor is triggeredm, time calculated */
					else if (bitArray[loopA] == 1 && bitArray[loopA+1] == 1 && inOutArray[loopA] ==1)
						{
							timeDiff = getMicrotime() - timeArray[loopA];
							timeInArray[timeInNumberArray] = timeDiff;
							timeInNumberArray = timeInNumberArray+1;
							in = in +1;
						}

					else if (bitArray[loopA] == 0 && bitArray[loopA+1] == 0 && inOutArray[loopA] == 1 && inOutArray[loopA+1] == 1)
						{
							inOutArray[loopA] = 0;
							inOutArray[loopA+1] = 0;
							printf("Opening channel, sensor nr %d",loopA);
						}

                                }

			else
				{
					printf("In odd routiine\n");
					printf ("Number: %d\n",loopA);
                                        printf("bitArray-1:%d ",bitArray[loopA-1]);
                                        printf("inOutArray:%d ",inOutArray[loopA]);
                                        printf("inOutArray-1:%d ",inOutArray[loopA-1]);

					/* Case of bee walking from outside to inside, first sensor triggered */
                                        if (bitArray[loopA] == 1 && bitArray[loopA-1] == 0 && inOutArray[loopA] == 0 && inOutArray[loopA-1] == 0)
                                                {
                                                        inOutArray[loopA] = 1;
                                                        inOutArray[loopA-1] = 1;
                                                        timeArray[loopA] = getMicrotime();
                                                }
					/* Case of bee walking in and second sensor is triggeredm, time calculated */

                                        else if (bitArray[loopA] == 1 && bitArray[loopA-1] == 1 && inOutArray[loopA] ==1)
                                                {
                                                        timeDiff = getMicrotime() - timeArray[loopA];
                                                        timeInArray[timeInNumberArray] = timeDiff;
                                                        timeInNumberArray = timeInNumberArray-1;
                                                        out = out +1;
                                                }

                                        else if (bitArray[loopA] == 0 && bitArray[loopA+1] == 0 && inOutArray[loopA] == 1 && inOutArray[loopA-1] == 1)
                                                {
                                                        inOutArray[loopA] = 0;
                                                        inOutArray[loopA-1] = 0;
                                                }

				}

                }

	/*	printf("Inside function %d", bitArray[1]);*/
		return 0;
	}



long long int getMicrotime()
        {
                struct timeval timer_usec; 
                long long int timestamp_usec; /* timestamp in microsecond */
                if (!gettimeofday(&timer_usec, NULL)) 
                        {
                                timestamp_usec = ((long long int) timer_usec.tv_sec) * 1000000ll + 
                                (long long int) timer_usec.tv_usec;
                        }
                else
                        {
                                timestamp_usec = -1;
                        }
                return timestamp_usec;
        }


