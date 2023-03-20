from PIL import Image
import numpy as np


def change_contrast(img, level):
    factor = (259 * (level + 255)) / (255 * (259 - level))
    def contrast(c):
        return 128 + factor * (c - 128)
    return img.point(contrast)


path_to_data = "./data/"

im_frame = change_contrast(Image.open(path_to_data + '12.png').convert('RGB'), 200)

np_frame = np.array(im_frame.getdata()).reshape((im_frame.height, im_frame.width, 3))

points = dict()

for y in range(np.shape(np_frame)[0]):
    for x in range(np.shape(np_frame)[1]):
        if (np_frame[y][x] != np.array([255, 255, 255])).any():
            points[(x, y)] = np_frame[y][x]

width_min = min(points.keys(), key=lambda p: p[0])[0]
height_min = min(points.keys(), key=lambda p: p[1])[1]

width = max(points.keys(), key=lambda p: p[0])[0] - width_min
height = max(points.keys(), key=lambda p: p[1])[1] - height_min

matrix = np.zeros((height, width), dtype=str)

for y in range(height_min, height + 1):
    for x in range(width_min, width + 1):
        if (x, y) in points:
            matrix[y - height_min][x - width_min] = '*'
        else:
            matrix[y - height_min][x - width_min] = '.'

symbols =

for y in range(height):
    for x in range(width):
        h = 1
        w = height
        exist = 0
        for y1 in range(y, height):
            cur_line_w = 0
            for x1 in range(x, height):
                if matrix[y1][x1] == '*':
                    cur_line_w += 1
                else:
                    break

            if cur_line_w == 0:
                break

            if cur_line_w <= w:
                exist = 1
                w = cur_line_w
                h += 1

        if exist:
            print(y, x, h, w)


for y in range(height):
    for x in range(width):
        print(matrix[y][x], sep='', end='')
    print()
