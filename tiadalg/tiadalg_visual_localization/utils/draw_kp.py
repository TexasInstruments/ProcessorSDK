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
  opts, args = getopt.getopt(argv,"hi:j:",["ipDir=","opDir="])


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
    self.ipDir = ''
    self.opDir = ''


  def displayArgs(self):
    print ("==================================")
    print ("ipset1    " , self.ipDir)
    print ("ipset2    " , self.opDir)
    print ("==================================")

###################################################################################
params=Params()

for opt, arg in opts:
  if opt == '-h':
    print_help()
    sys.exit()
  elif opt in ("-i", "--ipDir"):
    params.ipDir = arg
  elif opt in ("-j", "--opDir"):
    params.opDir = arg

params.displayArgs()

kp_x_loc = 0
kp_y_loc = 1
kp_score_loc = 2
img_height = 384
img_width = 768

for subdir, dirs, files in os.walk(params.ipDir):

    files.sort()
    for file in files:
      if(os.path.splitext(file)[1] == '.txt'):
        print(f'reading the file {os.path.join(params.ipDir,file)}')

        try:
          with open(os.path.join(params.ipDir,file)) as f:
              set1 = f.readlines()
              f.close()
        except Exception as e:
          sys.stderr.write('%s\n' %e)
          sys.exit(1)

        if os.path.exists(os.path.join(params.opDir,file)):
          op_img_name = os.path.join(params.opDir,file)
          op_img_name = op_img_name.replace(".txt",".png")
          print(f'writing the file {op_img_name}')
          img_data = np.zeros((img_height,img_width,3),np.uint8)

          for line_1 in set1[1:]:
            set1_cur_data = line_1.split()
            kp_x = int(float(set1_cur_data[kp_x_loc]))
            kp_y = int(float(set1_cur_data[kp_y_loc]))
            kp_score = int(float(set1_cur_data[kp_score_loc]))
            img_data[kp_y][kp_x][0] = kp_score
            img_data[kp_y][kp_x][1] = kp_score
            img_data[kp_y][kp_x][2] = kp_score            
            
          print(img_data.shape)
          cv2.imwrite(op_img_name, img_data)
        
        
        
        
