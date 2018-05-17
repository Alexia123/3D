import cv2
import numpy as np

drawing = cv2.imread('fatine.png')

#square [[[271 510]]
# [[752 511]]
# [[656 307]]
# [[367 307]]]

orig_pts = np.float32([[364.0, 477.0],[486.0, 362.0],[652.0, 435.0], [545.0, 576.0]])
dest_pts = np.float32([[0, 0], [512, 0], [512,512], [0, 512]])
M = cv2.getPerspectiveTransform(orig_pts, dest_pts)
print M
newdrawing = cv2.warpPerspective(drawing, M, (512, 512))
cv2.imwrite('fatine_warped.png',newdrawing)
