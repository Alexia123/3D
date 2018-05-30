#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <omp.h>

#include <time.h>   /* chronometrage */
#include "sys/time.h"

using namespace std;
using namespace cv;

double my_gettimeofday(){
  struct timeval tmp_time;
  gettimeofday(&tmp_time, NULL);
  return tmp_time.tv_sec + (tmp_time.tv_usec * 1.0e-6L);
}

int main()
{
	/* Chronometrage */
  	double debut, fin;
    /* debut du chronometrage */
  	debut = my_gettimeofday();
	// Traitement pour le grand rectangle
	printf("start python\n");
    system("python p1.py Plane");

    // Traitement pour les petits rectangles

    // Déclaration des variables
	int z, x;
	string argument;
	#pragma omp parallel for
	for (z = -2; z<3; z++)
	{
		#pragma omp parallel for
		for (x = -2; x<3 ; x++)
		{
			if(z>=0)
			{
				if(x>=0)
				{
					argument = "python p1.py Wall0" + to_string(z) + "0" + to_string(x);
					char * arg = new char[argument.size() + 1]; // La fonction sytem prend des char* en argument
					copy(argument.begin(), argument.end(), arg);
					arg[argument.size()] = '\0';
					system(arg);
				}
				else
				{
					argument = "python p1.py Wall0"+ to_string(z) + "-0" + to_string(abs(x));
					char * arg = new char[argument.size() + 1]; // La fonction sytem prend des char* en argument
					copy(argument.begin(), argument.end(), arg);
					arg[argument.size()] = '\0';
					system(arg);
				}
			}
			else
			{
				if(x>=0)
				{
					argument = "python p1.py Wall-0" + to_string(abs(z)) + "0" + to_string(x);
					char * arg = new char[argument.size() + 1]; // La fonction sytem prend des char* en argument
					copy(argument.begin(), argument.end(), arg);
					arg[argument.size()] = '\0';
					system(arg);
				}
				else
				{
					argument = "python p1.py Wall-0"+ to_string(abs(z)) + "-0" + to_string(abs(x));
					char * arg = new char[argument.size() + 1]; // La fonction sytem prend des char* en argument
					copy(argument.begin(), argument.end(), arg);
					arg[argument.size()] = '\0';
					system(arg);
				}
			}
			
		}
	}

    printf("python finished\n");
    fin = my_gettimeofday();
  	fprintf( stderr, "Temps total d'exécution de p1' : %g sec\n", 
	   fin - debut);
  	fprintf( stdout, "%g\n", fin - debut);
    return 0;
}
