import cv2
import numpy as np

img = cv2.imread("saved_photos/155_zed.png")

img_read = np.zeros(img.shape, np.float32)

with open("saved_photos/155_depth_file.txt") as f:
    row = 0
    col = 0
    for line in f:
        x = float(line.split()[0])
        img_read[col, row] = x
        col += 1
        if(col >= img.shape[0]):
            col = 0
            row += 1
        if(row >= img.shape[1]):
            break

print("LEWE:")
print(img_read[240*2, 483*2], img_read[241*2, 484*2])

print("PRAWE:")
print(img_read[243*2, 600*2], img_read[241*2, 600*2])

