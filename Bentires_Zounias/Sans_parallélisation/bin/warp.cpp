#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


#include <time.h>   /* chronometrage */
#include "sys/time.h"

using namespace std;
using namespace cv;

double my_gettimeofday(){
  struct timeval tmp_time;
  gettimeofday(&tmp_time, NULL);
  return tmp_time.tv_sec + (tmp_time.tv_usec * 1.0e-6L);
}
 
int main( )
{
    /* Chronometrage */
    double debut, fin;
    /* debut du chronometrage */
    debut = my_gettimeofday();

    ifstream fichier("images.txt", ios::in);  // on ouvre en lecture
    string ligne;
    string nom;
    string argument; // Va contenir l'appel à python
    if(fichier)  // si l'ouverture a fonctionné
        {
            getline(fichier, ligne);  // 1ere ligne
            while(ligne != "") // Tant que l'on est pas à la fin du fichier
            {
                argument = "python warpperspective.py " + ligne;
                char * arg = new char[argument.size() + 1]; // La fonction sytem prend des char* en argument
                copy(argument.begin(), argument.end(), arg);
                arg[argument.size()] = '\0';
                system(arg); // On appelle le programme python
                getline(fichier, ligne);  // on passe à la ligne suivante
            }
            fichier.close(); // Fermeture du fichier
        }
        else
                cerr << "Impossible d'ouvrir le fichier !" << endl;
    waitKey(0);
    fin = my_gettimeofday();
    fprintf( stderr, "Temps total d'exécution de warp' : %g sec\n", 
       fin - debut);
    fprintf( stdout, "%g\n", fin - debut);
    return 0;
}


