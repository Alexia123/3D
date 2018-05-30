#-*- coding: utf-8 -*-
#!python
from PIL import Image
import sys

width = 1024 
height = 768 
#background = (0, 0, 0, 255)
background = (0,0,0,0)

# On charge l'image de l'énigme

enigme = Image.open('enigme.png', 'r')
pixels_enigme=enigme.load()



# Image du grand rectangle
image = Image.new("RGBA", (width, height), background)
pixels = image.load()

fichier=open("map.txt","r")
lignes=fichier.readlines()
fichier.close()
for ligne in lignes:
	ligne=ligne.strip("\n")
	lignedata=ligne.split(",")
	#print lignedata
	if lignedata[2]==sys.argv[1]:
		#print "Plane"
		px=int(lignedata[0])
		py=int(lignedata[1])
		#pixels[px, py] = (0, 0, 0, 255)
		pixels[px, py] = pixels_enigme[px, py]

image.save("fatalex_" +sys.argv[1]+".png")

