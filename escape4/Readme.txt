Site : Taper dans Google "warpperspective python example brien"


'p' = res.png
'y' = planes.txt => Position des coins de chaque rectangle
',' = map.txt => Pour chaque pixel, nous donne à quel objet il appartient et la distance à la caméra

on utilise p1.py pour récupérer juste le grand rectangle => image.png

On récupère les 4 points via planes.txt qu'on entre dans warpperspective.py

On obtient l'homographie => fatine_warped.png
cp fatine_warped.png /home/sasl/encad/pecheux/Urho3D-1.5/bin/Data/Textures/
cd  ~pecheux/Urho3D-1.5/bin/Data/Materials
cp cube_noir.xml Fatine.xml
vi Fatine.xml => Changer la texture de ref

Aller dans StaticScene.cpp pour changer la texture de l'objet
