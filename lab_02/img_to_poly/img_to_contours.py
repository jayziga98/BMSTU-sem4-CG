import copy
import svgwrite
import cv2 as cv
import numpy as np
from contour_to_edges import to_edges as cte

image = cv.imread('./data/22.png')

cv.imshow('Source', image)
cv.waitKey(0)
cv.destroyAllWindows()

src = image.copy()

src = cv.cvtColor(src, cv.COLOR_BGR2GRAY)
ret, src = cv.threshold(src, 150, 255, cv.THRESH_BINARY)

kernel = np.ones((5, 5), np.uint8)
src = cv.medianBlur(src, 5)
src = cv.erode(src, kernel, iterations=1)
src = cv.medianBlur(src, 9)
kernel = np.array([[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]])
src = cv.filter2D(src, -1, kernel)
src = cv.threshold(src, 0, 255, cv.THRESH_BINARY_INV + cv.THRESH_OTSU)[1]

# Upscale
# src = cv.resize(src, dsize=None, fx=2, fy=2)
# cv.imshow('Resized', src)

cv.imshow('Preproc', src)
cv.waitKey(0)
cv.destroyAllWindows()

# Получение контура
contours = cv.findContours(src, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)[0]
# contours = contours[0] if len(contours) == 2 else contours[1]

# Апроксимация
epss = np.linspace(0.000, 0.12, 50)
i = 0
while -1 < i < len(epss):
    eps = epss[i]

    blank = np.zeros(src.shape[0:2], dtype=np.uint8)
    blank.fill(255)

    p_amount = 0
    approxed_contour = list()
    for c in contours[:-1]:
        print(len(c))
        peri = cv.arcLength(c, True)
        if peri > 1000:
            approx = cv.approxPolyDP(c, 0, True)
        else:
            approx = cv.approxPolyDP(c, eps * peri, True)
        cv.drawContours(blank, [approx], -1, (0, 200, 0), 1)
        p_amount += len(approx)
        approxed_contour.append(approx)

    text = "eps={:.4f}, amount={}".format(eps, p_amount)
    cv.putText(blank, text, (20, 20), cv.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

    cv.imshow("Approximated Contour", blank)
    k = cv.waitKey(0)
    if k == 3:
        i = i + 1 if i + 1 < len(epss) else 0
    elif k == 2:
        i = i - 1 if i - 1 > -1 else len(epss) - 1
    elif k == 32:
        cte(approxed_contour)
        break
