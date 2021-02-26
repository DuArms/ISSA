import numpy as np
import cv2

WIDTH = int(1920 * 4 / 12)
HEIGHT = int(1080 * 4 / 12)

threshold = 255 // 10


def function(val, threshold=255 // 10):
    if threshold < val:
        return 255
    return 0


def contrast(frame):
    f = np.vectorize(function)
    for i, l in enumerate(frame):
        frame[i] = f(l)
    return frame


def crop(new_frame, lenght):
    new_frame[:, :  int(lenght * 0.20)] *= 0
    new_frame[:, -int(lenght * 0.20):] *= 0
    new_frame[-int(lenght * 0.05):, :] *= 0

    return new_frame


def grey_scale_by_hand(frame):
    new_frame = np.zeros((HEIGHT, WIDTH), dtype=np.uint8)
    j = 0
    for linie in frame:
        i = 0
        for pixel in linie:
            new_frame[j][i] = np.multiply(pixel, [0.2126, 0.7152, 0.0722]).sum()
            i += 1
        j += 1
    return new_frame


puncte_linie_stanga = ((0, 0), (0, 1))
puncte_linie_dreapta = ((0, 0), (0, 1))
vec = [-1 * (10 ** 5), (10 ** 5)]

def check_line(linia):

    de_verificat = []
    x, y = zip(*linia)
    de_verificat.append(y)

    isOk = True
    for l in de_verificat:
        for el in l:
            if el < vec[0] or vec[1] < el:
                isOk = False

    return isOk

def create_trapez():
    new_frame = np.zeros((HEIGHT, WIDTH), dtype=np.uint8)

    def calc_point(x, y):
        x = int(WIDTH * x)
        y = int(HEIGHT * y)
        return (x, y)

    upper_left = calc_point(0.35, 0.80)
    upper_right = calc_point(0.65, 0.80)
    lower_right = calc_point(1, 1)
    lower_left = calc_point(0, 1)

    points_of_poly = np.array([upper_left, upper_right, lower_right, lower_left])

    cv2.fillConvexPoly(new_frame, points_of_poly, 1)

    # for i in range(10000):
    #     cv2.imshow('Original', new_frame)
    #     if cv2.waitKey(1) & 0xFF == ord('q'):
    #         break

    return new_frame, points_of_poly


soberl_vertical = np.float32([
    [-1, -2, -1],
    [0, 0, 0],
    [+1, +2, +1]
])

soberl_horizontal = np.transpose(soberl_vertical)


def sobel(frame):
    frame_f32 = np.float32(frame)
    vec = [
        cv2.filter2D(frame_f32, -1, soberl_vertical),
        cv2.filter2D(frame_f32, -1, soberl_horizontal)
    ]

    vec2 = [
        np.uint8(vec[0]),
        np.uint8(vec[1])
    ]

    # cv2.imshow('Original', cv2.convertScaleAbs (vec2[0]) )
    # cv2.waitKey(1000) & 0xFF == ord('q')
    #
    # cv2.imshow('Original', cv2.convertScaleAbs(vec2[1]))
    # cv2.waitKey(1000) & 0xFF == ord('q')

    m = np.sqrt(vec[0] * vec[0] + vec[1] * vec[1])
    m = cv2.convertScaleAbs(m)
    # cv2.imshow('Original',m )
    # cv2.waitKey(1) & 0xFF == ord('q')

    return m


def liniar_regretion(listax, listay):
    return np.polyfit(listax, listay, deg=1)


def find_x(list=[], y=1):
    return int((y - list[1]) / list[0])
