import numpy as np


def to_edges(contours):
    points = list()
    order = list()

    number = 0
    for cont in contours:

        parsed_contour = np.array(cont).reshape((np.array(cont).size // 2, 2))
        size = parsed_contour.size // 2
        for i in range(size):
            points.append((parsed_contour[i][0], parsed_contour[i][1]))
            if i + 1 < size:
                order.append((number, number + 1))

            number += 1

    with open('output.txt', 'w+') as f:
        print(len(points), file=f)
        for point in points:
            print(*point, file=f)

        print(len(order), file=f)
        for i in order:
            print(*i, file=f)