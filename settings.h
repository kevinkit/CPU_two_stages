/* here you can change settings of the signal and the filter*/
/* for a better understanding the peak is always located at the middle of the signal */




#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

//Signal-Settings
gint32 base = 50000;
gint32 peak = 10000;
const gint32 input_length = 40000;
gint32 peak_length = 512;
gint32 decrement = 1; 			//linear decrement

//Filtersettings
gint32 filter_length = 8;			//possible filterlengths are just even numbers, usual filterlengths are 2,4,8,16,32,64,128,256
//The filter will only find a peak if the peak_length is double as long as the filter_length, otherwise it may find it but will give back a wrong energy! 
//If the filterlength is over the half of the peak_length it will find the peaks, but with the wrong energy, if it is under the half_filter length it will not find anything



//DO NOT USE FILTER-LENGTH = 2    IT WILL FIND ANYTHING !!!! {EVEN NOISES}

//function will set random vectors at random points


gint32* create_signal(gint32* input_vector, gint32 noise)
{


	//signal creation
	gint32 i = 0;
	for(i = 0; i < (input_length/2) ;i++)
	{

		input_vector[i] = base;

	}





	for(i=0;i < input_length/2 ; i++)
	{

		input_vector[i + input_length/2] = base + peak - i*decrement;

	}

	//noise addition
	for(i=0; i < input_length; i++)
	{
		input_vector[i] = input_vector[i] + (rand() %noise) - noise;
	}






	return input_vector;
}


void create_random_signal(gint32* input_signal, gint32 noise, gint32* counts, gint32* positions)
{
	gint32 i = 0;
	gint32 x = 0;
	gint32 pos = 0;
	gint32 j = 0;
	gint32 hit = 0;
	//	counts[2] = max_num;  ->> MUSS NOCH RICHTIG GERUNDET WERDEN ! 

	for(i = 0; i < input_length;i++)
	{
		input_signal[i] = base;
	}

	//third try	
	for(j=0; j < 2000; j++)
	{

		hit = 0;
		pos = 0;	
		for(i = 0; i < input_length; i++)
		{
			//srand((unsigned) time ( NULL ) );

			//              pos = peak_length + pos + (rand() %(input_length - pos));
			pos = 6*filter_length + peak_length + pos + (rand()%(input_length- (peak_length + 2*filter_length + pos)));

			if( pos >=(input_length - peak_length - 2*filter_length))
			{
				counts[0] = hit;
				//printf("break after %d \n", counts[0]);
				break;
			}
			else
			{	

				positions[hit] = pos;
				printf("\n hit at %d \n", hit);
				hit++;

			}
		}
		//nur abbrechen wenn mindestens 1 Peak gesetzt wurde! 
		if(counts[0] > 1)
			break;



	}

	for(i = 0; i < hit; i ++)
	{
		printf("the positions are %d\t", positions[i]);
	}



	counts[1] = pos;
	printf("adressing count vektor worked \n");
	for(i = 0; i < hit; i++)
	{
		printf("inside the first loop for positioning selection \n");


		for(x=0; x < peak_length; x++)
		{

			if(positions[i] + peak_length >= input_length)
			{
				//		printf("the last peak was created at %d \n", positions[i - 1]);
				break;

			}			
			else
			{
				input_signal[positions[i] + x] = base + peak;
				if(positions[i] + x >= input_length)
				{
					printf("Overflow in der Signalerzeugung! \n");	
					exit(1);
				}
			}
		}

	}

	//	printf("peak creation worked \n");


	for(i = 0; i < input_length; i++)
	{
		input_signal[i] = input_signal[i] + (rand() %noise) - noise;
	}
	//	printf("noise addition worked \n");




}






#endif
