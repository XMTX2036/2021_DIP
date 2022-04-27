#coding=utf-8
import cv2
import numpy as np  
 
img = cv2.imread("4.bmp", 0)
 
gray_lap = cv2.Laplacian(img, -1, ksize = 3)
dst = cv2.convertScaleAbs(gray_lap)
 
cv2.imshow('laplacian',img-dst)
cv2.waitKey(0)
cv2.destroyAllWindows()