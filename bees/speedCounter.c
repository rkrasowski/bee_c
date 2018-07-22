#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/* Configuration variable*/
int numOfChannels = 2;;
int hive = 21;

/* Proram variable */



int bit;	/* bits from shift register */
int loopA;
int sensorNr;
int in = 0;	/* number of bees moving IN Odd/Even sensor */
int out = 0;	/* number of bees moving OUT Even/odd sensor */
int inOutArray[24];
int bitArray[24];	/* Array of bits from shift register*/
long long int timeArray[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};	/* put start time  for speed calculations */
long long int timeDiff;
long long int timeInArray[10000];	/* Times IN to average for later speed calculation */
long long int  timeOutArray[10000];	/* Times OUT to average for later speed calculation */
int timeInNumberArray = 0;		/* Number of the element in timeInArray, will be reset at snding to file */
int timeOutNumberArray = 0;		/* Number of the ellement in timeOutArray, will be reset at sending to file */




/* ######################### Functions declarations ############################# */

int counter();
long long int getMicrotime();

/* ######################### Main program ####################################### */

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

				bitArray[sensorNr] = bit;
				digitalWrite (2, HIGH);
				delay(1);
				digitalWrite (2, LOW);
				delay(1);
				bit = digitalRead (0);
			}


/* Data analysis  */
		printf("Getting IN #####################################\n");
		bitArray[0] = 1;
                counter();

		printf("Getting deeper IN #########################################\n");
		bitArray[1] = 1;
		counter();

		printf("Reset IN  #########################################\n");
		bitArray[0] = 0;
		bitArray[1] = 0;
		counter();

		printf("Getting OUT ######################################\n");
		bitArray[1] = 1;
		counter();

		printf("Getting deeper OUT #################################\n");
		bitArray[0] = 1;
		counter();

		printf("Reset OUT #################################################\n");
		bitArray[1] = 0;
		bitArray[0] = 0;
		counter();


printf("Getting IN #####################################\n");
                bitArray[0] = 1;
                counter();

                printf("Getting deeper IN #########################################\n");
                bitArray[1] = 1;
                counter();

                printf("Reset IN  #########################################\n");
                bitArray[0] = 0;
                bitArray[1] = 0;
                counter();




/* Gettint Unux time*/
 		time_t seconds;

   		seconds = time(NULL);
   		printf("Seconds:  %ld\n", seconds);

/* calculate the speed */

		float speedIn = 0;
		float speedOut = 0;
	  	long long int sum;
		int speedLoop;
		int numOfElements = 0;
   		float avg;

	/* Speed IN */
   		sum = avg = numOfElements = 0 ;

   		for(speedLoop = 0; speedLoop <= 300; speedLoop++) 
        		{
                		if (timeInArray[speedLoop] != 0)
					{
						sum = sum + timeInArray[speedLoop];
						numOfElements = numOfElements +1;
					}
        		}

   		speedIn = (float)sum / numOfElements;

	/* Speed OUT */
		sum = avg = numOfElements = 0;

                for(speedLoop = 0; speedLoop <= 300; speedLoop++) 
                        {
                                if (timeOutArray[speedLoop] != 0)
                                        {
                                                sum = sum + timeOutArray[speedLoop];
                                                numOfElements = numOfElements +1;
                                        }
                        }

                speedOut = (float)sum / numOfElements;



   		printf("Speed IN is %.1f\n", speedIn);
		printf("Speed OUT is %.1f\n",speedOut);   






/* Putting data into the file */

                FILE *fptr;
                fptr = fopen("file.txt","a");
                if(fptr == NULL)
                        {
                                printf("Error!, can not access the file, does it exist?\n");
                                exit(1);
                        }

		fprintf(fptr,"Counts,");
		fprintf(fptr,"Hive=%d ",hive);
                fprintf(fptr,"Time=%ld,",seconds);
                fprintf(fptr,"In=%i,",in);
                fprintf(fptr,"Out=%i,",out);
                fprintf(fptr,"SpeedIn=%.1f,",speedIn);
                fprintf(fptr,"SpeedOut=%.1f\n",speedOut);

                fclose(fptr);


		return 0;
	}

/* ####################  Functions definition ####################*/



int counter ()
	{
		 for ( loopA=0; loopA<= numOfChannels*2-1; loopA++)
                {
                        printf("\n\nSensor number: %d\n",loopA);

			printf("BEFORE: bitArray: %d, ",bitArray[loopA]);
                        printf("bitArray+1:%d, ",bitArray[loopA+1]);
			printf("bitArray-1:%d, ",bitArray[loopA-1]);
                        printf("inOutArray:%d, ",inOutArray[loopA]);
                        printf("inOutArray+1:%d, ",inOutArray[loopA+1]);
			printf("inOutArray-1:%d ",inOutArray[loopA-1]);
                        printf("in: %d ",in);
			printf("out: %d\n",out);


                        if(loopA % 2 == 0)
                                {

					/* Case of bee walking from OUT to IN, first sensor triggered is EVEN */
					if (bitArray[loopA] == 1 && bitArray[loopA+1] == 0 && inOutArray[loopA] == 0 && inOutArray[loopA+1] == 0)
						{
							inOutArray[loopA] = 1;
							inOutArray[loopA+1] = 1;
							timeArray[loopA] = getMicrotime();
							printf("Inside EVEN pos 1\n");
						}
					/* Case of bee walking in and second sensor is triggeredm, time calculated */
					else if (bitArray[loopA] == 1 && bitArray[loopA+1] == 1 && inOutArray[loopA] ==1 && timeArray[loopA+1] == 0)
						{
							timeDiff = getMicrotime() - timeArray[loopA];
							timeInArray[timeInNumberArray] = timeDiff;
							timeInNumberArray = timeInNumberArray+1;
							in = in +1;
							printf("Inside EVEN pos 2\n");
						}

					else if (bitArray[loopA] == 0 && bitArray[loopA+1] == 0 && inOutArray[loopA] == 1 && inOutArray[loopA+1] == 1)
						{
							inOutArray[loopA] = 0;
							inOutArray[loopA+1] = 0;
							timeArray[loopA] = 0;
							timeArray[loopA+1] = 0;
							printf("Inside EVEN pos 3\n");
						}


                                }

			else
				{

					/* Case of bee walking from IN to OUT, first sensor triggered ODD */
                                        if (bitArray[loopA] == 1 && bitArray[loopA-1] == 0 && inOutArray[loopA] == 0 && inOutArray[loopA-1] == 0)
                                                {
                                                        inOutArray[loopA] = 1;
                                                        inOutArray[loopA-1] = 1;
                                                        timeArray[loopA] = getMicrotime();
							printf("Inside ODD pos 1\n");

                                                }
					/* Case of bee walking in and second sensor is triggeredm, time calculated */

                                        else if (bitArray[loopA] == 1 && bitArray[loopA-1] == 1 && inOutArray[loopA] ==1 && timeArray[loopA-1] == 0)
                                                {
                                                        timeDiff = getMicrotime() - timeArray[loopA];
                                                        timeOutArray[timeOutNumberArray] = timeDiff;
                                                        timeOutNumberArray = timeOutNumberArray+1;
                                                        out = out +1;
							printf("Inside ODD pos 2\n");
							printf("DiffOut:%lld\n",timeOutArray[timeOutNumberArray-1]);
                                                }

                                        else if (bitArray[loopA] == 0 && bitArray[loopA-1] == 0 && inOutArray[loopA] == 1 && inOutArray[loopA-1] == 1)
                                                {
                                                        inOutArray[loopA] = 0;
                                                        inOutArray[loopA-1] = 0;
							timeArray[loopA] = 0;
							timeArray[loopA-1] = 0;
							printf("Inside ODD pos 3\n");
                                                }

				}

			printf("AFTER: bitArray: %d, ",bitArray[loopA]);
                        printf("bitArray+1:%d, ",bitArray[loopA+1]);
			printf("bitArray-1:%d, ",bitArray[loopA-1]);
                        printf(" inOutArray:%d, ",inOutArray[loopA]);
                        printf(" inOutArray+1:%d, ",inOutArray[loopA+1]);
			printf("inOutArray-1:%d, ",inOutArray[loopA -1]);
                        printf("in:%d, ",in);
			printf("out:%d, ",out);
			printf("speedIN:%lld, ",timeInArray[timeInNumberArray-1]);
			printf("speedOUT:%lld, ",timeOutArray[timeOutNumberArray-1]);
			printf("timeArray: %lld, ",timeArray[loopA]);
			printf("timeInNumberArray: %d\n\n",timeInNumberArray);


                }

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


