# arduino-programas
import numpy as np
import cv2

capture = cv2.VideoCapture(1)
while(1):
	_, frame = capture.read()
	font = cv2.FONT_HERSHEY_SIMPLEX
	# Convert BGR to HSV
	#hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # define range of blue color in HSV
	lower_blueB = np.array([150,0,0])
	upper_blueB = np.array([255,100,100])
	lower_blueG = np.array([0,150,150])
	upper_blueG = np.array([100,255,255])
	lower_blueR = np.array([0,0,150])
	upper_blueR = np.array([100,100,255])
	matriz = np.ones((5,5),np.uint8)
    # Threshold the HSV image to get only blue colors
	maskB = cv2.inRange(frame, lower_blueB, upper_blueB)
	maskG = cv2.inRange(frame, lower_blueG, upper_blueG)
	maskR = cv2.inRange(frame, lower_blueR, upper_blueR)
	kernel = np.ones((5,5),np.uint8)
	maskRR = cv2.morphologyEx(maskB, cv2.MORPH_OPEN, kernel)
	#mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, matriz)
	res, hierarchy = cv2.findContours(maskB,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
    # Bitwise-AND mask and original image
	#res = cv2.bitwise_and(frame,frame, mask= mask)
	#cv2.imshow('frame',frame)
	#cv2.imshow('maskB',maskB)
	#cv2.imshow('maskG',maskG)
	
	#cv2.imshow('res',res)
	
	Amax=45
	Amin=1
	font = cv2.FONT_HERSHEY_SIMPLEX 
	for i in range(len(res)):
		cnt=res[i]
		moments = cv2.moments(cnt)
		area=moments['m00']
		print(area)
		cnts=0
		if(Amax>area>Amin):
			cnts=[cnts, cnt]
			cx = int(moments['m10']/moments['m00'])
			cy = int(moments['m01']/moments['m00'])
			img = cv2.circle(maskRR,(cx,cy), 10, (0,0,255), 2)
			cv2.putText(maskRR,str(i),(cx+10,cy+10), font, 0.5,(0,0,255),2)
	cv2.imshow('maskR',maskB)
	cv2.imshow('maskRR',maskRR)
	k = cv2.waitKey(5) & 0xFF
	if k == 27:
		break
cv2.destroyAllWindows()
