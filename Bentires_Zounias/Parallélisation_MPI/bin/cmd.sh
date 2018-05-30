#! /bin/bash
rm -f p1
rm -f warp
rm -f fatalex*
rm -f Warped*
mpic++ -std=c++11 -o p1 p1.cpp `pkg-config --cflags --libs opencv`
mpic++ -std=c++11 -o warp warp.cpp `pkg-config --cflags --libs opencv`
mpirun -n 2 ./p1
mpirun -n 2 ./warp

cd /home/sasl/encad/pecheux/Urho3D-1.5/bin/Data/Textures/Fatalex
rm -f *
cp ~/S4/Pecheux/Bentires_Zounias/Sans_parallélisation/bin/Warped* .
