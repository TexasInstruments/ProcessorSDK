# loging of the file based on image file name, and collecting various statistics
import numpy as np
import glob, os
import sys, getopt
import csv
import struct
import math

def print_help():
  print ('Loging of the file based on image file name, and collecting various statistics')
  print ('python gen_map_bin -i <map file> -o <output dir>')
#################################################################################
try:
  argv = sys.argv[1:]
  opts, args = getopt.getopt(argv,"hi:o:",["ipMapFile=",  "opDir="])


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
    self.ipMapFile = '/ti/j7/workarea/tiadalg/data/visual_localization/ptCld_carla_set17_768x384_exp121_0_1500_2997_Skp3.txt'
    self.opDir = '/ti/j7/workarea/tiovx/conformance_tests/test_data/psdkra/app_visual_localization'


  def displayArgs(self):
    print ("==================================")
    print ("ipMapFile    " , self.ipMapFile)
    print ("opDir    " , self.opDir)
    print ("==================================")

###################################################################################
params=Params()

for opt, arg in opts:
  if opt == '-h':
    print_help()
    sys.exit()
  elif opt in ("-i", "--ipMapFile"):
    params.ipMapFile = arg
  elif opt in ("-o", "--opDir"):
    params.opDir = arg

params.displayArgs()

map_voxel_file = os.path.join(params.opDir,'map_voxel.bin')
map_feat_file  = os.path.join(params.opDir,'map_feat.bin')
map_desc_file  = os.path.join(params.opDir,'map_desc.bin')

fp_map_voxel = open(map_voxel_file,'wb')
fp_map_feat  = open(map_feat_file,'wb')
fp_map_desc  = open(map_desc_file,'wb')

if (os.path.exists(params.ipMapFile)):
  with open(params.ipMapFile, 'r') as myfile:
      mapFile=myfile.read().split('\n')
else :
  print('Could not open the log file')
  sys.exit()

markerString='#[X,Y,Z,P]:'
offset = 0
cur_tot_desc = 0
feat_size=64
tot_vox_count = 0
map_desc_8bit = True
scale_factor = 1.0
data_offset = 0 #128
for idx, line in enumerate(mapFile):
  if markerString in line:
    line = line[len(markerString):]
    line = line.split()
    cur_tot_desc = int(line[3])
    cur_voxel = struct.pack('<iiiii',int(line[0]),int(line[1]),int(line[2]),cur_tot_desc,offset)
    offset += cur_tot_desc
    fp_map_voxel.write(cur_voxel)
    tot_vox_count =  tot_vox_count + 1
    print(f'Total data written is  {int(line[0])}, {int(line[1])}, {int(line[2])}, {cur_tot_desc}, {offset}')
  elif (len(line) > 20) and (cur_tot_desc > 0):
    cur_tot_desc -= 1
    line = line.split()
    #print(line)
    cur_feat = struct.pack('fff',(float(line[0])),(float(line[1])),(float(line[2])))
    fp_map_feat.write(cur_feat)
    for feat_idx in range(feat_size):
      #print(f'original value is {line[3+feat_idx]} and U16 value is {float(line[3+feat_idx])*16384}')
      #cur_feat = float(line[3+feat_idx])*16384.0
      cur_feat = float(line[3+feat_idx])

      if cur_feat > 0.0:
        cur_feat = cur_feat + 0.5
      else:
        cur_feat = cur_feat - 0.5

      if map_desc_8bit == True:
        cur_feat = int(cur_feat / scale_factor) + data_offset # this is done to bring in the range of 0 to 255. As matching routine expects descriptors to be in 0-255 range
        cur_desc = struct.pack('B', int(cur_feat))
      else:
        cur_feat = int(cur_feat) + data_offset
        cur_desc = struct.pack('H', int(cur_feat))

      fp_map_desc.write(cur_desc)


fp_map_voxel.flush()
fp_map_voxel.close()
fp_map_feat.flush()
fp_map_feat.close()
fp_map_desc.flush()
fp_map_desc.close()

print('Total numbe of voxels are',tot_vox_count)
print('Total number of descriptors are ',offset)
print('Finished writing the log output file')

