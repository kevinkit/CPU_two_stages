#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "settings.h"
#include "input.h"


int main(int argc, char* argv[])
{



	gint32		i = 0;
	gint32	 	a = 0;
	gint32		b = 0;
	gint32	        c = 0;
	gint32          d = 0;
	gint32          g = 0;
	gint32	      fil = 0;
	double        tit = 0;
	double	tit_first = 0;
	double tit_second = 0;


	GTimer* timer = g_timer_new();
	GTimer* first_stage = g_timer_new();
	GTimer* second_stage = g_timer_new();

	gint32* input_vector = (gint32*) malloc(sizeof(gint32) * (input_length+1));
	gint32* pos          = (gint32*) malloc(sizeof(gint32) * (input_length+1));
	int counter;

	gint32 output[40000];

	gint32 noise      = 2000;
	gint32 threshhold = 5000;
	gint32 ener       = 0;
	gint32 expected = 5;

	FILE* time_taken;
	FILE* time_stage1;
	FILE* time_stage2;

	FILE* input;
	FILE* fil1;
	FILE* fil2;
	
	
	char input_n[1024] = "input_fil32.txt";
	char fil1_n[1024] = "stage_1_fil32.txt";
	char fil2_n[1024] = "stage_2_fil32.txt";


	


	int run = 1;
	char time_file[1024] = "time_fil2_cpu_old.txt";
	char time_file_stage1[1024] = "time_fil2_stage_1.txt";
	char time_file_stage2[1024] = "time_fil2_stage_2.txt";
	srand ((unsigned) time (NULL));
	for(; run <= 9; run++)
	{	
		printf(" \n \n \n \n -----------------------RUN = %d", run);
		switch(run)
		{
			case 1: filter_length = 2; break;
			case 2: filter_length = 4; break;
			case 3: filter_length = 8; break;
			case 4: filter_length = 16; break;
			case 5: filter_length = 32; break;
			case 6: filter_length = 64; break;
			case 7: filter_length = 128; break;
			case 8: filter_length = 256; break;
			case 9: filter_length = 512; break;
		}

		sprintf(time_file, "time_fil%d_cpu_old.txt", filter_length);
		sprintf(input_n, "input_fil%d.txt", filter_length);
		sprintf(fil1_n, "stage_1_fil%d.txt", filter_length);
		sprintf(fil2_n, "stage_2_fil%d.txt", filter_length);
		sprintf(time_file_stage1, "time_fil%d_stage_1.txt",filter_length);
		sprintf(time_file_stage2, "time_fil%d_stage_2.txt", filter_length);
		




		gint32* firstder     = (gint32*) malloc(sizeof(gint32)*(input_length - 2*filter_length +2));
		gint32* secondder    = (gint32*) malloc(sizeof(gint32)*(input_length - 4*filter_length +2));

		printf("setup arrays \n");
		counter = rand_rects(expected,3,input_length,4*filter_length,4*filter_length,4*filter_length,peak_length,base+peak, input_vector, noise, base, 0,pos);



		a = 0;
		b = 0;
		c = 0;
		d = 0;
		g_timer_start(timer);
		g_timer_start(first_stage);

		//Init for first derivation
		for(i=0;i <=filter_length;i++)
		{
			a += input_vector[i];
			b += input_vector[i + filter_length];
		}

		firstder[0] = a-b;

		//First derivation
		for(i=0; i < input_length - (filter_length << 1); i++)
		{
			firstder[i+1] = firstder[i] + input_vector[i] - (input_vector[i + filter_length] << 1) + input_vector[i + (filter_length << 1)];
		}
		g_timer_stop(first_stage);
		g_timer_start(second_stage);
		//Init for second derivation
		for(i = 0; i <= filter_length; i++)
		{
			c += firstder[i];
			d += firstder[i+filter_length];

		}

		secondder[0] = c -d;
		g = 0;
		//Second derivation and Trigger
		for(i = 0; i < input_length - (filter_length << 2);i++)
		{
			secondder[i+1] = secondder[i] + firstder[i] - (firstder[i+filter_length] << 1) + firstder[i + (filter_length << 1)];
		
			if(secondder[i] >= 0 && secondder[i+1] < 0 && i > filter_length)
			{
				ener = firstder[i+filter_length]/filter_length;
			             if(ener >= threshhold)
                                {
                                        output[g << 1] = ener;
                                        output[(g << 1) +1] = i - filter_length;
                                        printf("%d \n %d \n\n\n", output[g << 1] , output[(g <<1) + 1]);
                                        g++;
                                }  	

			}
		}
		g_timer_stop(second_stage);
		g_timer_stop(timer);

		time_taken = fopen(time_file, "a+");
		time_stage1 = fopen(time_file_stage1, "a+");
		time_stage2 = fopen(time_file_stage2, "a+");	
		if(time_taken == NULL || time_stage1 == NULL || time_stage2 == NULL)
		{
			printf("Could not open file ! \n");
			exit(1);
		}


		tit = g_timer_elapsed(timer, NULL);
		tit_first = g_timer_elapsed(first_stage,NULL);
		tit_second = g_timer_elapsed(second_stage,NULL);
		fprintf(time_taken,"%f\t", 1000*tit);
		fprintf(time_stage1, "%f\t", 1000*tit_first);
		fprintf(time_stage2, "%f\t", 1000*tit_second);
		free(firstder);
		free(secondder);
		fclose(time_taken);
		fclose(time_stage1);
		fclose(time_stage2);

	}
	free(pos);
	free(input_vector);
}
