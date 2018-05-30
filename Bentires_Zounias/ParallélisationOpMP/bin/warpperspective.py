#-*- coding: utf-8 -*-
#!python
from PIL import Image
import cv2
import numpy as np
import sys


# Traitement pour le grand rectangle
drawing = cv2.imread('fatalex_' + sys.argv[1] + '.png')
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
	if lignedata[0] == sys.argv[1]:
		flag = 1 # Les 4 lignes précédentes contiennent les coordonnées
#print(p1,p2,p3,p4)
orig_pts = np.float32([p1,p4,p3,p2])
dest_pts = np.float32([[0, 0], [512, 0], [512,512], [0, 512]])
M = cv2.getPerspectiveTransform(orig_pts, dest_pts)
newdrawing = cv2.warpPerspective(drawing, M, (512, 512))
cv2.imwrite('Warped_fatalex_' + sys.argv[1] + '.png',newdrawing)

