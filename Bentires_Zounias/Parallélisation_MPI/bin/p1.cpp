#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <mpi.h>

#include <time.h>   /* chronometrage */
#include "sys/time.h"

using namespace std;
using namespace cv;

double my_gettimeofday(){
  struct timeval tmp_time;
  gettimeofday(&tmp_time, NULL);
  return tmp_time.tv_sec + (tmp_time.tv_usec * 1.0e-6L);
}

int main(int argc, char **argv)
{

	 /* Par processeur */ 
  	int rang; // rang
  	int NP; // NP = nombre de processus
    /* Initialisation MPI */
  	MPI_Init(&argc,&argv);
  	MPI_Comm_size(MPI_COMM_WORLD, &NP);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rang); 

	/* Chronometrage */
  	double debut, fin;
    /* debut du chronometrage */
  	debut = my_gettimeofday();


  	if (NP != 2)
  	{
  		cout << "Nombre de proc doit être égal à 2" << endl;
  		return 0;
  	}
  	// On va imposer le nombre de processeurs à 2

  	// Le processeur de rang 0 se charge de cela
  	if (rang == 0)
  	{
  		// Traitement pour le grand rectangle
    	system("python p1.py Plane");
  	}
	int z, x;
	string argument;

    // Traitement pour les petits rectangles
    // Le rang 0 s'occupe des indices z négatifs et le 1 des z positifs

    if (rang != 0)
    {
    	
		for (z = 0; z<3; z++)
		{
			for (x = -2; x<3 ; x++)
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
		}
	}
	else 
	{
		for (z = -2; z<0; z++)
		{
			for (x = -2; x<3 ; x++)
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
  	MPI_Finalize();
    fin = my_gettimeofday();
  	fprintf( stderr, "Temps total d'exécution de p1' : %g sec\n", 
	   fin - debut);
  	fprintf( stdout, "%g\n", fin - debut);

    return 0;
}
