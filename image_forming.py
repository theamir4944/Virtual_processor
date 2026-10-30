from numpy import *
from PIL import Image

img = Image.open("itsded.png").convert("RGBA")
arr = array(img)

print(arr)
print(arr.shape)

file = open("itsded.txt", "wb")
encoding_str = "utf-8"

max_pos_y = 875
max_pos_x = 1200
counter = 0

for y in range(0, max_pos_y):
    for x in range(0, max_pos_x):
        new_pixel = arr[y][x]
        for ind in range(0, 4):
            file.write(f"PUSH {new_pixel[ind]}\n".encode(encoding_str))
            file.write(f"POP [{counter}]\n".encode(encoding_str))
            counter += 1
        
file.write("DRW\nHLT\n".encode(encoding_str))




# import cv2

# cap = cv2.VideoCapture("mybadapple.mp4")
# file = open("image.txt", "wb")

# max_pos_y = 480
# max_pos_x = 752

# encoding_str = "utf-8"

# while True:

#     counter = 0
#     ret, frame = cap.read()
#     if not ret: break
#     rgba = cv2.cvtColor(frame, cv2.COLOR_BGR2RGBA)

#     for y in range(0, max_pos_y): 
#         for x in range(0, max_pos_x):
#             for ind in range(4):
#                 file.write(f"PUSH {rgba[y][x][ind]}\nPOP [{counter}]\n".encode(encoding_str))
#                 counter += 1
#     file.write("DRW\n".encode(encoding_str))

# file.write("HLT\n".encode(encoding_str))
# cap.release()
# file.close()


