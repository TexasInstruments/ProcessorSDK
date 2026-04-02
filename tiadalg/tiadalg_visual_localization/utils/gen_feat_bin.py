# loging of the file based on image file name, and collecting various statistics
import numpy as np
import glob, os
import sys, getopt
import csv
import struct

def print_help():
  print ('Loging of the file based on image file name, and collecting various statistics')
  print ('python gen_feat_bin -i <feat file> -o <output dir>')
#################################################################################
try:
  argv = sys.argv[1:]
  opts, args = getopt.getopt(argv,"hi:o:",["ipFeatDir=",  "opDir="])


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
    self.ipFeatDir = ''
    self.opDir = ''


  def displayArgs(self):
    print ("==================================")
    print ("ipFeatDir    " , self.ipFeatDir)
    print ("opDir    " , self.opDir)
    print ("==================================")

###################################################################################
params=Params()

for opt, arg in opts:
  if opt == '-h':
    print_help()
    sys.exit()
  elif opt in ("-i", "--ipFeatDir"):
    params.ipFeatDir = arg
  elif opt in ("-o", "--opDir"):
    params.opDir = arg

params.displayArgs()

feat_file_tag = 'locTapPt03_imageFeaturePoints'
file_list_name = 'names.txt'
f = open(os.path.join(params.opDir,file_list_name),"w")

for subdir, dirs, files in os.walk(params.ipFeatDir):
  files.sort()
  for file in files:
    if feat_file_tag in file:

      print(f'Processing the file {file}')
      file_base = file.split('_')[0]
      file_path = os.path.join(subdir,file)
      f.write(file_base)
      f.write('\n')
      ext_feat_file  = os.path.join(params.opDir,'ext_feat_'+file_base+'.bin')
      ext_desc_file  = os.path.join(params.opDir,'ext_desc_'+file_base+'.bin')

      os.makedirs(os.path.dirname(ext_feat_file), exist_ok=True)
      os.makedirs(os.path.dirname(ext_desc_file), exist_ok=True)

      fp_ext_feat  = open(ext_feat_file,'wb')
      fp_ext_desc  = open(ext_desc_file,'wb')

      if (os.path.exists(file_path)):
        with open(file_path, 'r') as myfile:
            mapFile=myfile.read().split('\n')
      else :
        print('Could not open the log file')
        sys.exit()

      cur_tot_desc = 0
      feat_size=64

      for idx, line in enumerate(mapFile):
        if (len(line) > 20):
          line = line.split()
          cur_feat = struct.pack('ii',int(float(line[0])*1024.0),int(float(line[1])*1024.0))
          #cur_feat = struct.pack('ff',float(line[0]),float(line[1]))
          #print(f'original points were {float(line[0])} , {float(line[1])}')
          #print(f'points after conversion is {int(float(line[0])*256.0)/256.0} , {int(float(line[1])*256.0)/256.0}')
          fp_ext_feat.write(cur_feat)
          cur_tot_desc = cur_tot_desc+ 1
          for feat_idx in range(feat_size):
            cur_desc = struct.pack('h',int(float(line[2+feat_idx])))
            fp_ext_desc.write(cur_desc)


      fp_ext_feat.flush()
      fp_ext_feat.close()
      fp_ext_desc.flush()
      fp_ext_desc.close()
      f.flush()
      f.close

      print('Total number of descriptors are ',cur_tot_desc)
      print('Finished writing the log output file')





