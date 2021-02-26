# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.

import cv2
from help import *

cam = cv2.VideoCapture('drum.mp4')


def lane_detect(frame):
    global puncte_linie_stanga, puncte_linie_dreapta
    original_frame = frame.copy()

    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # frame = grey_scale_by_hand(frame)

    trapez, trapezoid_bounds = create_trapez()
    trapezoid_bounds = np.float32(trapezoid_bounds)
    frame_bounds = np.float32(np.array([(0, 0), (WIDTH, 0), (WIDTH, HEIGHT), (0, HEIGHT)]))

    frame = frame * trapez

    cv2.imshow('TRAPEZ', frame)
    cv2.waitKey(1)


    magic = cv2.getPerspectiveTransform(trapezoid_bounds, frame_bounds)
    frame = cv2.warpPerspective(frame, magic, (WIDTH, HEIGHT))

    cv2.imshow('WARP', frame)
    cv2.waitKey(1)
    # ---------------------------------------------------------
    frame = cv2.blur(frame, ksize=(7, 7))
    cv2.imshow('BLUER', frame)
    cv2.waitKey(1)
    frame = sobel(frame)
    cv2.imshow('SOBEL', frame)
    cv2.waitKey(1)
    #frame = contrast(frame)
    # cv2.imshow('CONTRAST', frame)
    # cv2.waitKey(1)

    new_frame = frame.copy()
    new_frame = crop(new_frame, len(frame[0]))
    try:
        mid_point = len(frame[0]) // 2

        stanga, dreapta = new_frame[:, :  mid_point], new_frame[:, mid_point:]

        pozition = []

        pozition.append(np.argwhere(stanga > 25))
        pozition.append(np.argwhere(dreapta > 25))

        try:
            left_xs, left_ys = zip(*pozition[0])
            pstanga = liniar_regretion(left_xs, left_ys)
            linia_din_stanga = [(0, find_x(pstanga, 0)), (HEIGHT, find_x(pstanga, HEIGHT))]
            if check_line(linia_din_stanga):
                puncte_linie_stanga = linia_din_stanga
        except Exception:
            pass

        try:
            right_xs, right_ys = zip(*pozition[1])
            pdrepta = liniar_regretion(right_xs, right_ys)
            linia_din_dreapta = [(mid_point, find_x(pdrepta, 0)), (HEIGHT + mid_point, find_x(pdrepta, HEIGHT))]

            if check_line(linia_din_dreapta):
                puncte_linie_dreapta = linia_din_dreapta
        except Exception:
            pass
        # ------------------------------------------------------------

        def f1(list_of_line, colors):

            def f(puncte_linie):
                lane_detected_frame = np.zeros((HEIGHT, WIDTH), dtype=np.uint8)
                cv2.line(lane_detected_frame, puncte_linie[0], puncte_linie[1], 255, 10)
                magic = cv2.getPerspectiveTransform(frame_bounds, trapezoid_bounds)
                lane_detected_frame = cv2.warpPerspective(lane_detected_frame, magic, (WIDTH, HEIGHT))
                line = np.argwhere(lane_detected_frame == 255)
                return line

            for i, line in enumerate(list_of_line):
                l = f(line)
                y, x = zip(*l)
                pA = (x[0], y[0])
                pB = (x[-1], y[-1])
                cv2.line(original_frame, pA, pB, colors[i], 3)

        f1([puncte_linie_stanga, puncte_linie_dreapta], [(50, 250, 50), (50, 50, 250)])

    except Exception:
        print(Exception)

    return original_frame


if __name__ == '__main__':
    while True:
        ret, frame = cam.read()

        if ret is False:
            break

        frame = cv2.resize(frame, (WIDTH, HEIGHT))
        original_frame = lane_detect(frame)
        cv2.imshow('Original', original_frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

cam.release()
cv2.destroyAllWindows()
# See PyCharm help at https://www.jetbrains.com/help/pycharm/
