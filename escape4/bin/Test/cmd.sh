#! /bin/bash
rm -f p1
rm -f warp
rm -f fatalex*
rm -f Warped*
g++ -std=c++11 -o p1 p1.cpp `pkg-config --cflags --libs opencv`
g++ -std=c++11 -o warp warp.cpp `pkg-config --cflags --libs opencv`
./p1
./warp

cd /home/sasl/encad/pecheux/Urho3D-1.5/bin/Data/Textures/Fatalex
rm -f *
cp ~/S4/Pecheux/escape4/bin/Test/Warped* .
