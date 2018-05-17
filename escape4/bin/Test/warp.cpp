#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace cv;
 
int main( )
{
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
    return 0;
}


