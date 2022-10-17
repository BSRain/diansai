# Blob Detection Example
#
# This example shows off how to use the find_blobs function to find color
# blobs in the image. This example in particular looks for dark green objects.

import sensor, image, time, pyb,os,tf
import car
from pid import PID
from pyb import UART
uart = UART(3, 19200)
# You may need to tweak the above settings for tracking green things...
# Select an area in the Framebuffer to copy the color settings.

sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # use RGB565.
sensor.set_framesize(sensor.QQVGA) # use QQVGA for speed.
sensor.set_windowing(240,240)
sensor.skip_frames(10) # Let new settings take affect.
sensor.set_auto_whitebal(False) # turn this off.
net = "trained.tflite"
labels = [line.rstrip('\n') for line in open("labels.txt")]
clock = time.clock() # Tracks FPS.

# For color tracking to work really well you should ideally be in a very, very,
# very, controlled enviroment where the lighting is constant...
green_threshold   = (29, 58, 9, 67, -9, 49)
size_threshold = 2000
x_pid = PID(p=0.5, i=1, imax=100)
h_pid = PID(p=0.05, i=0.1, imax=50)
ROI = (2,47,155,72)
STOP = 0
stop = 0
zuida = 0
ShuFig = 0
times = 8
relax = {}
def findmax(result):
    index = -1
    max = -1
    for i in range(len(result)):
        if(result[i][1]> max):
            index = result[i][0]
            max  = result[i][1]
    return index


def find_shu():
    for obj in tf.classify(net, img, min_scale=1.0, scale_mul=0.8, x_overlap=0.5, y_overlap=0.5):
        img.draw_rectangle(obj.rect())
        result = list(zip(labels, obj.output()))

        predictions_list =[]
        for i in result:
            predictions_list.append(list(i))
        return  findmax(predictions_list)

def find_max(blobs):
    max_size=0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

while(True):
 if '1'==uart.read():
    relax.clear()
    clock.tick()  # Track elapsed milliseconds between snapshots().
    for i in range(times):#times减少扫描时间
        img = sensor.snapshot() # Take a picture and return the image.
        f = find_shu()
        if(f in relax.keys()):
            relax[f] = relax[f]+1
        else:
            relax[f] = 1
    max = -1
    print(relax)
    max_key = -1
    for key in relax.keys():
        if(relax[key] > max):
            max_key = key
            max = relax[key]

    print(max_key)
clock.tick()
blobs = img.find_blobs([green_threshold],roi=ROI)
if blobs:
    max_blob = find_max(blobs)
    x_error = max_blob[5]-img.width()/2
    h_error = max_blob[2]*max_blob[3]-size_threshold
 #   print("x error: ", x_error)
    '''
    for b in blobs:
        # Draw a rect around the blob.
        img.draw_rectangle(b[0:4]) # rect
        img.draw_cross(b[5], b[6]) # cx, cy
    '''
    img.draw_rectangle(max_blob[0:4]) # rect
    img.draw_cross(max_blob[5], max_blob[6]) # cx, cy
    x_output=x_pid.get_pid(x_error,1)
    h_output=h_pid.get_pid(h_error,1)
   # print(h_output)
    if h_output>450 & STOP==0:
        STOP = 1
    if STOP==1:
       STOP=2
       car.run(40,-10)
       pyb.delay(700)
       car.run(50,-50)
       pyb.delay(300)
       stop = 1
    if stop:
       car.run(28-x_output,28+x_output)
    else :
        car.run(58-x_output,58+x_output)
else:
    car.run(0,0)
