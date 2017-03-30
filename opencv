import numpy as np
import cv2

cara=cv2.CascadeClassifier('C:\\Users\\facundo\\Downloads\\opencv\\build\etc\haarcascades\\haarcascade_frontalface_default.xml')
ojos=cv2.CascadeClassifier('C:\\Users\\facundo\\Downloads\\opencv\\build\etc\haarcascades\\haarcascade_eye.xml')

cam=cv2.VideoCapture(1)

while (True):
	ret,trama=cam.read()
	gray=cv2.cvtColor(trama, cv2.COLOR_BGR2GRAY)
	face=cara.detectMultiScale(gray, 1.3, 5)
	for (x,y,w,h) in face:
		#print face
		#print x,y,w,h
		cv2.rectangle(trama,(x,y),(x+w,y+h),(255,0,0),2)
		roi_gray = gray[y:y+h, x:x+w]
		roi_color = trama[y:y+h, x:x+w]
		eyes = ojos.detectMultiScale(roi_gray)
		for (ex,ey,ew,eh) in eyes:
			cv2.rectangle(roi_color,(ex,ey),(ex+ew,ey+eh),(0,255,0),2)
	cv2.imshow('img',trama)
	#cv2.rectangle(trama,(x,y),(x+w,y+h),(255,0,0),2)
#IndentationError: expected an indented block
#[Finished in 0.2s with exit code 1]
	k=cv2.waitKey(5) & 0xFF
	if k==27:
		break
