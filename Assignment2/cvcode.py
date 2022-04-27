import cv2
img = cv2.imread('3.bmp', 0)

cv2.imshow('original image',img)
# cv2.waitKey()

retval, dst = cv2.threshold(img, 0, 255, cv2.THRESH_OTSU)
print(retval)
element = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))
dst1 = cv2.dilate(dst, element, iterations=1)
dst2 = cv2.erode(dst, element, iterations=1)
dst3 = cv2.dilate(dst2, element, iterations=1)
dst4 = cv2.erode(dst1, element, iterations=1)
cv2.imwrite(r'./test_binary_py.bmp', dst)
cv2.imwrite(r'./test_dilation_py.bmp', dst1)
cv2.imwrite(r'./test_erosion_py.bmp', dst2)
cv2.imwrite(r'./test_opening_py.bmp', dst3)
cv2.imwrite(r'./test_closing_py.bmp', dst4)

