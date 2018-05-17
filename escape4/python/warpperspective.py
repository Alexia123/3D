#-*- coding: utf-8 -*-
#!python
from PIL import Image
import cv2
import numpy as np


# Traitement pour le grand rectangle
drawing = cv2.imread('fatalex_Plane.png')
flag = -1 # Repère pour la suite du code
fichier=open("planes.txt","r")
lignes=fichier.readlines()
fichier.close()
for ligne in lignes:
	ligne=ligne.strip("\n")
	lignedata=ligne.split(",")
	#print lignedata
	if (flag == 1 and lignedata[0] == "0"): # Premier point
		p1 = [lignedata[1], lignedata[2]]
	if flag == 1 and lignedata[0] == "1" :
		p2 = [lignedata[1], lignedata[2]]
	if flag == 1 and lignedata[0] == "2" :
		p3 = [lignedata[1], lignedata[2]]
	if flag == 1 and lignedata[0] == "3":
		p4 = [lignedata[1], lignedata[2]]
		flag = -1
	if lignedata[0] == "Plane":
		flag = 1 # Les 4 lignes précédentes contiennent les coordonnées
#print(p1,p2,p3,p4)
orig_pts = np.float32([p1,p4,p3,p2])
dest_pts = np.float32([[0, 0], [512, 0], [512,512], [0, 512]])
M = cv2.getPerspectiveTransform(orig_pts, dest_pts)
#print M
newdrawing = cv2.warpPerspective(drawing, M, (512, 512))
cv2.imwrite('fatalex_Plane_warped.png',newdrawing)


# Traitement des petits rectangles
for z in range(-2,3):
	for x in range (-2,3):
		drawing = cv2.imread('fatalex_image'+str(z)+str(x)+'.png')
		flag = -1 # Repère pour la suite du code
		fichier=open("planes.txt","r")
		lignes=fichier.readlines()
		fichier.close()
		for ligne in lignes:
			ligne=ligne.strip("\n")
			lignedata=ligne.split(",")
			#print lignedata
			if (flag == 1 and lignedata[0] == "0"): # Premier point
				p1 = [lignedata[1], lignedata[2]]
			if flag == 1 and lignedata[0] == "1" :
				p2 = [lignedata[1], lignedata[2]]
			if flag == 1 and lignedata[0] == "2" :
				p3 = [lignedata[1], lignedata[2]]
			if flag == 1 and lignedata[0] == "3":
				p4 = [lignedata[1], lignedata[2]]
				flag = -1
			if(z>=0):
				if(x>=0):
					if lignedata[0]== "Wall0"+str(z)+"0"+str(x):
						flag = 1 
				else:
					if lignedata[0]== "Wall0"+str(z)+"-0"+str(abs(x)):
						flag = 1 
			else:
				if(x>=0):
					if lignedata[0]== "Wall-0"+str(abs(z))+"0"+str(x):
						flag = 1 
				else:
					if lignedata[0]== "Wall-0"+str(abs(z))+"-0"+str(abs(x)):
						flag = 1 
		orig_pts = np.float32([p1,p4,p3,p2])
		dest_pts = np.float32([[0, 0], [512, 0], [512,512], [0, 512]])
		M = cv2.getPerspectiveTransform(orig_pts, dest_pts)
		newdrawing = cv2.warpPerspective(drawing, M, (512, 512))
		cv2.imwrite('fatalex_image'+str(z)+str(x)+'_warped.png',newdrawing)



#square [[[271 510]]
# [[752 511]]
# [[656 307]]
# [[367 307]]]

