import matplotlib.pyplot as plt
import numpy as np

img_path = "pic/ljz.jpg"

plt_img = plt.imread(img_path)

print(len(plt_img))
print(len(plt_img[0]))
pixel = ""
with open('pic.txt', 'w') as f:
    for i in range(64):
        for j in range(64):
            for k in range(3):
                pixel += str(plt_img[i][j][k]) + ','
    f.write(pixel)
