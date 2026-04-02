# loging of the file based on image file name, and collecting various statistics
import numpy as np
import glob, os
import sys, getopt
import csv
import struct
import cv2

def print_help():
  print ('Comparing the results key points and descriptor')
  print ('python compare_results.py -i <ipDir> -j <opDir>')
#################################################################################
try:
  argv = sys.argv[1:]
  opts, args = getopt.getopt(argv,"hi:j:o:",["trackFile=","imgDir=","opDir="])


except getopt.GetoptError:
  print("\n\n**Something went wrong in passing the arguments**\n\n")
  print ("argv: ", argv)
  print ('syntax error')
  print_help()
  sys.exit(2)
###################################################################################
class Params:
  #Class for Params
  def __init__(self):
    self.trackFile = '/user/a0393749/deepak_files/temp/track.txt'
    self.imgDir    = '/user/a0393749/deepak_files/temp/input/'
    self.opDir     = '/user/a0393749/deepak_files/temp/'


  def displayArgs(self):
    print ("==================================")
    print ("trackFile    " , self.trackFile)
    print ("imgDir       " , self.imgDir)
    print ("opDir        " , self.opDir)
    print ("==================================")

###################################################################################
params=Params()

for opt, arg in opts:
  if opt == '-h':
    print_help()
    sys.exit()
  elif opt in ("-i", "--trackFile"):
    params.trackFile = arg
  elif opt in ("-j", "--imgDir"):
    params.imgDir = arg
  elif opt in ("-o", "--opDir"):
    params.opDir = arg

params.displayArgs()


try:
    with open(os.path.join(params.trackFile,params.trackFile)) as f:
        lines = f.readlines()
        f.close()
except Exception as e:
    sys.stderr.write('%s\n' %e)
    sys.exit(1)

prev_file_name = ''
for idx, line in enumerate(lines):
    if "Frame #" in line:

        if prev_file_name != '':
            op_img_name = os.path.join(params.opDir,prev_file_name)
            print(f"writing the file {op_img_name}")
            cv2.imwrite(op_img_name, img)

        file_name = line.split('#')[1]
        file_name = file_name.replace(" ","")
        file_name = file_name.replace("\n","")

        in_img_name = file_name + ".png"
        prev_file_name = in_img_name
        in_img_name = os.path.join(params.imgDir,in_img_name)
        print(f"reading the file {in_img_name}")
        img = cv2.imread(in_img_name)
    elif ((idx%100) == 0):
        kps  = line.split("::")[2].replace("(","").replace(")","").replace(" ","")
        kps  = kps.replace("\n","")
        kps  = kps.split(",")
        kps.remove('')
        age  = int(line.split("::")[1])
        track_id = int(line.split("::")[0])
        if((len(kps) != 2*age) or (age < 1)):
            print("something went wrong")

        start_point = (int(float(kps[0])),int(float(kps[1])))

        cv2.putText(img, str(track_id), start_point, cv2.FONT_HERSHEY_SIMPLEX, 0.25, (255,0,0), 1, cv2.LINE_AA)

        if(int(age) == 1):
            cv2.arrowedLine(img, start_point, start_point, [255,0,0], 2)

        for i in range(age-1):
            start_point = (int(float(kps[2*i+ 0])),int(float(kps[2*i+1])))
            end_point   = (int(float(kps[2*i + 2])),int(float(kps[2*i + 3])))
            cv2.arrowedLine(img, start_point, end_point, [40*i,40*i,40*i], 1)

            dist = ((end_point[0] - start_point[0])^2 + (end_point[1] - start_point[1])^2)

            if(dist > 20*20):
                print (f"{file_name} {track_id} {i}")


op_img_name = os.path.join(params.opDir,prev_file_name)
print(f"writing the file {op_img_name}")

