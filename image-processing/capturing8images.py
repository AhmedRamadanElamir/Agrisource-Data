# cv2 package , python 2.x

import time
import cv2
ports = [1,2,3,4,5,6,7,8]; #specify port numbers
for i in ports:
  camera_port = i
  camera = cv2.VideoCapture(camera_port)
  time.sleep(0.1)  # If you don't wait, the image will be dark
  return_value, image = camera.read()
  cv2.imwrite("D:\ASD internship\"+i+".png", image) # specify the directory to be saved
  del(camera)  # so that others can use the camera as soon as possible

