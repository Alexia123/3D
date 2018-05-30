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
    MPI_Status status;

    /* Chronometrage */
    double debut, fin;
    /* debut du chronometrage */
    debut = my_gettimeofday();

    if (NP != 2)
    {
        cout << "Nombre de proc doit être égal à 2" << endl;
        return 0;
    } // Un processeur va lire le fichier et l'autre exécuter le code python

    if (rang == 0)
    {
        ifstream fichier("images.txt", ios::in);  // on ouvre en lecture
        string ligne;
        string nom;
        string argument; // Va contenir l'appel à python
        if(fichier)  // si l'ouverture a fonctionné
            {
                int continu = 0;
                getline(fichier, ligne);  // 1ere ligne
                while(ligne != "") // Tant que l'on est pas à la fin du fichier
                {
                    argument = "python warpperspective.py " + ligne;
                    char * arg = new char[argument.size() + 1]; // La fonction sytem prend des char* en argument
                    copy(argument.begin(), argument.end(), arg);
                    arg[argument.size()] = '\0';
                    int taille = argument.size() + 1;
                    MPI_Send(&taille,1, MPI_INT, 1, 0, MPI_COMM_WORLD);
                    MPI_Send(arg, taille, MPI_CHAR, 1, 1, MPI_COMM_WORLD);
                    getline(fichier, ligne);  // on passe à la ligne suivante
                    if (ligne == "")
                        continu = 1;
                    MPI_Send(&continu,1, MPI_INT, 1, 2, MPI_COMM_WORLD);
                }
                fichier.close(); // Fermeture du fichier
            }
        else
                cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    else
    {
        char * arg;
        int taille;
        int continu = 0;
        while(continu == 0)
        {
            MPI_Recv(&taille,1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(arg, taille, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
            system(arg); // On appelle le programme python
            MPI_Recv(&continu,1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
        }
        
    }

    waitKey(0);
    MPI_Finalize();
    fin = my_gettimeofday();
    fprintf( stderr, "Temps total d'exécution de warp' : %g sec\n", 
       fin - debut);
    fprintf( stdout, "%g\n", fin - debut);
    return 0;
}


