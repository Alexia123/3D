#-*- coding: utf-8 -*-
#!python
from PIL import Image

width = 1024 
height = 768 
#background = (0, 0, 0, 255)
background = (0,0,0,0)

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
	if lignedata[2]=="Plane":
		#print "Plane"
		px=int(lignedata[0])
		py=int(lignedata[1])
		pixels[px, py] = (0, 0, 0, 255)

image.save("image.png")

# Générer les images des petits rectangles
for z in range(-2,3):
	for x in range (-2,3):
		image = Image.new("RGBA", (width, height), background)
		pixels = image.load()

		fichier=open("map.txt","r")
		lignes=fichier.readlines()
		fichier.close()
		for ligne in lignes:
			ligne=ligne.strip("\n")
			lignedata=ligne.split(",")
			#print lignedata
			if(z>=0):
				if(x>=0):
					if lignedata[2]== "Wall0"+str(z)+"0"+str(x):
						#print ("Wall0"+str(z)+"0"+str(x))
						px=int(lignedata[0])
						py=int(lignedata[1])
						pixels[px, py] = (0, 0, 0, 255)
				else:
					if lignedata[2]== "Wall0"+str(z)+"-0"+str(abs(x)):
						#print ("Wall0"+str(z)+"-0"+str(abs(x)))
						px=int(lignedata[0])
						py=int(lignedata[1])
						pixels[px, py] = (0, 0, 0, 255)
			else:
				if(x>=0):
					if lignedata[2]== "Wall-0"+str(abs(z))+"0"+str(x):
						#print ("Wall-0"+str(abs(z))+"0"+str(x))
						px=int(lignedata[0])
						py=int(lignedata[1])
						pixels[px, py] = (0, 0, 0, 255)
				else:
					if lignedata[2]== "Wall-0"+str(abs(z))+"-0"+str(abs(x)):
						#print ("Wall-0"+str(abs(z))+"-0"+str(abs(x)))
						px=int(lignedata[0])
						py=int(lignedata[1])
						pixels[px, py] = (0, 0, 0, 255)

		image.save("image"+str(z)+str(x)+".png")
		
'''

image = Image.new("RGBA", (width, height), background)
pixels = image.load()
fichier=open("map.txt","r")
lignes=fichier.readlines()
fichier.close()
z = 1
x =1
for ligne in lignes:
	ligne=ligne.strip("\n")
	lignedata=ligne.split(",")
	#print lignedata
	if lignedata[2]== "Wall0"+str(z)+"0"+str(x):
		print ("Wall0"+str(z)+"0"+str(x))
		px=int(lignedata[0])
		py=int(lignedata[1])
		pixels[px, py] = (0, 0, 0, 255)
			

image.save("image"+str(z)+str(x)+".png")
'''