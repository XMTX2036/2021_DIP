import cv2
import numpy as np

img = cv2.imread(r'./1.bmp',1)
dstimg = cv2.Laplacian(img, -1, ksize=3)
# dstimg = cv2.convertScaleAbs(dstimg)
dstimg = img + abs(dstimg)*0.5
dstimg = cv2.convertScaleAbs(dstimg)
cv2.imwrite('1_Laplace_py.bmp', dstimg)
 
# cv2.waitKey(0)#保持图像