#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

/* Configuration variable*/
int numOfChannels = 12;
int hive = 21;
int delay2send = 5;			/* Time delay between message to server */

/* Program variable */



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
int timeInNumber = 0;		/* Number of the element in timeInArray, will be reset at snding to file */
int timeOutNumber = 0;		/* Number of the ellement in timeOutArray, will be reset at sending to file */


long long int startTestTime;
float elapsTime;



/* ######################### Functions declarations ############################# */

int counter();
long long int getMicrotime();

/* ######################### Main program ####################################### */

int main (void)
	{

		int currTime = (unsigned)time(NULL);
		int time2be = currTime + delay2send;


		for (;;)


			{
				if (time2be > (unsigned)time(NULL))
					{
						startTestTime = getMicrotime();
						delay(1000);

						wiringPiSetup();
						pinMode (2, OUTPUT);
						pinMode (3, OUTPUT);
						pinMode (0, INPUT);
						digitalWrite (3,LOW);
						delay(1);
						digitalWrite (3,HIGH);
						bit = digitalRead(0);

						for (sensorNr =0; sensorNr<numOfChannels * 2-1; sensorNr=sensorNr+1)

							{
								bitArray[sensorNr] = bit;
								digitalWrite (2, HIGH);
								delay(1);
								digitalWrite (2, LOW);
								delay(1);
								bit = digitalRead (0);
							}
   	/*	printf("Getting IN #####################################\n");*/
                bitArray[0] = 1;
                bitArray[1] = 0;
                counter();

          /*      printf("Getting deeper IN #########################################\n");*/
                bitArray[1] = 1;
                counter();

            /*    printf("Reset IN  #########################################\n");*/
                bitArray[0] = 0;
                bitArray[1] = 0;
                counter();

/*printf("Getting OUT #####################################\n");	*/	
		bitArray[1] =1;
		counter();


/*printf("Getting deeper OUT #########################################\n");*/
		bitArray[0] = 1;
		bitArray[1] = 1;
		counter();

/*printf("Reset OUT  #########################################\n");*/
		bitArray[0] = 0;
		bitArray[1] = 0;
		counter();


						counter();


					}

				else
					{
						elapsTime = (getMicrotime() - startTestTime) * 0.000001;
						printf("Elaps time is: %.6f\n",elapsTime);


						delay(1000);
					/* Gettint Unux time*/
 						time_t seconds;

   						seconds = time(NULL);

					/* calculate the speed */

						float speedIn = 0;
						float speedOut = 0;
	  					long long int sum;
						int speedLoop;
						int numOfElements = 0;
   						float avg;

					/* Speed IN */
   						sum = avg = numOfElements = 0 ;


					 /* int loopTest;

                                                        for(loopTest = 0; loopTest < 10; loopTest++)
                                                        printf("TimeInArray BEFORE speed loop:%lld\n ", timeInArray[loopTest]);

						*/


   						for(speedLoop = 0; speedLoop <= timeInNumber; speedLoop++) 
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
                				for(speedLoop = 0; speedLoop <= timeOutNumber; speedLoop++) 
                        				{
                                				if (timeOutArray[speedLoop] != 0)
                                        				{
                                                				sum = sum + timeOutArray[speedLoop];
                                                				numOfElements = numOfElements +1;
                                        				}
                        				}

                				speedOut = (float)sum / numOfElements;

						printf("\nSendind data to file\n");
						printf("In is:%d\n",in);
						printf("Out is:%d\n",out);
   						printf("Speed IN is %.1f\n", speedIn);
						printf("Speed OUT is %.1f\n\n",speedOut);   


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

					/* Reseting all variables */
						in = 0;
						out = 0;
						speedIn = 0;
						speedOut = 0;

					int loopErase;

					for(loopErase =0; loopErase <= timeInNumber; loopErase++)
						{
							timeInArray[loopErase] = 0;
						}


					 for(loopErase =0; loopErase <= timeOutNumber; loopErase++)
                                                {
                                                        timeOutArray[loopErase] = 0;
                                                }

						timeInNumber = 0;
                                                timeOutNumber = 0;




				/*
						  int loopTest;

   							for(loopTest = 0; loopTest <= timeInNumber; loopTest++)
								{
      									printf("Time in number array: %d\n ", timeInNumber);

								}

				*/
						time2be = (unsigned)time(NULL) + delay2send;
					}



				}

		return 0;

	}

/* ####################  Functions definition ####################*/



int counter ()
	{
		 for ( loopA=0; loopA<= numOfChannels*2-1; loopA++)
                {
                       /* printf("\n\nSensor number: %d\n",loopA);

			printf("BEFORE: bitArray: %d, ",bitArray[loopA]);
                        printf("bitArray+1:%d, ",bitArray[loopA+1]);
			printf("bitArray-1:%d, ",bitArray[loopA-1]);
                        printf("inOutArray:%d, ",inOutArray[loopA]);
                        printf("inOutArray+1:%d, ",inOutArray[loopA+1]);
			printf("inOutArray-1:%d ",inOutArray[loopA-1]);
                        printf("in: %d ",in);
			printf("out: %d\n",out);
		*/

                        if(loopA % 2 == 0)
                                {

					/* Case of bee walking from OUT to IN, first sensor triggered is EVEN */
					if (bitArray[loopA] == 1 && bitArray[loopA+1] == 0 && inOutArray[loopA] == 0 && inOutArray[loopA+1] == 0)
						{
							inOutArray[loopA] = 1;
							inOutArray[loopA+1] = 1;
							timeArray[loopA] = getMicrotime();
							printf("EVEN pos 1\n");
						}
					/* Case of bee walking in and second sensor is triggeredm, time calculated */
					else if (bitArray[loopA] == 1 && bitArray[loopA+1] == 1 && inOutArray[loopA] ==1 && timeArray[loopA+1] == 0)
						{
							timeDiff = getMicrotime() - timeArray[loopA];
							timeInArray[timeInNumber] = timeDiff;
							timeInNumber = timeInNumber+1;
							in = in +1;
							printf("EVEN pos 2\n");
						}

					else if (bitArray[loopA] == 0 && bitArray[loopA+1] == 0 && inOutArray[loopA] == 1 && inOutArray[loopA+1] == 1)
						{
							inOutArray[loopA] = 0;
							inOutArray[loopA+1] = 0;
							timeArray[loopA] = 0;
							timeArray[loopA+1] = 0;
							printf("EVEN pos 3\n");
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
							printf("ODD pos 1\n");

                                                }
					/* Case of bee walking in and second sensor is triggeredm, time calculated */

                                        else if (bitArray[loopA] == 1 && bitArray[loopA-1] == 1 && inOutArray[loopA] ==1 && timeArray[loopA-1] == 0)
                                                {
                                                        timeDiff = getMicrotime() - timeArray[loopA];
                                                        timeOutArray[timeOutNumber] = timeDiff;
                                                        timeOutNumber = timeOutNumber+1;
                                                        out = out +1;
							printf("ODD pos 2\n");
                                                }

                                        else if (bitArray[loopA] == 0 && bitArray[loopA-1] == 0 && inOutArray[loopA] == 1 && inOutArray[loopA-1] == 1)
                                                {
                                                        inOutArray[loopA] = 0;
                                                        inOutArray[loopA-1] = 0;
							timeArray[loopA] = 0;
							timeArray[loopA-1] = 0;
							printf("ODD pos 3\n");
                                                }

				}
		/*
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
		*/

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

