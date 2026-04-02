# loging of the file based on image file name, and collecting various statistics
import numpy as np
import glob, os
import sys, getopt
import csv
import struct

def print_help():
  print ('Comparing the results of ego localization')
  print ('python compare_results.py -i <result file> -j <reference result dir>')
#################################################################################
try:
  argv = sys.argv[1:]
  opts, args = getopt.getopt(argv,"hi:j:k:l:",["ipResultFile=",  "ipRefDir=", "ipRefDirFlag=", "opLogFile="])


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
    self.ipResultFile = '/ti/j7/workarea/vision_apps/vl_out/log.txt'
    self.ipRefDir = '/ti/j7/workarea/tiadalg/data/visual_localization/posesCam.txt'
    self.ipRefDirFlag = 2899
    self.opLogFile = "op_log.txt"


  def displayArgs(self):
    print ("==================================")
    print ("ipResultFile    " , self.ipResultFile)
    print ("ipRefDir        " , self.ipRefDir)
    print ("ipRefDirFlag    " , self.ipRefDirFlag)
    print ("opLogFile       " , self.opLogFile)
    print ("==================================")

###################################################################################
params=Params()

for opt, arg in opts:
  if opt == '-h':
    print_help()
    sys.exit()
  elif opt in ("-i", "--ipResultFile"):
    params.ipResultFile = arg
  elif opt in ("-j", "--ipRefDir"):
    params.ipRefDir = arg
  elif opt in ("-k", "--ipRefDirFlag"):
    params.ipRefDirFlag = int(arg)
  elif opt in ("-l", "--opLogFile"):
    params.opLogFile = arg

params.displayArgs()

feat_file_tag = 'locTapPt07_postFilterOutput6D.txt'
#feat_file_tag  = 'locTapPt07_postFilterOutput6D.txt'

tot_dist_err = 0
skip_fact = 1
max_dist_err = 0

try:
  with open(params.ipResultFile) as f:
      lines = f.readlines()
except Exception as e:
  sys.stderr.write('could not open the log file%s\n' %e)
  sys.exit(1)

if params.ipRefDirFlag != 0:
  try:
    with open(params.ipRefDir) as f_ref:
        lines_ref = f_ref.readlines()
  except Exception as e:
    sys.stderr.write('could not open the log file%s\n' %e)
    sys.exit(1)

file_log = open(params.opLogFile,"w")

for cur_frame_num, line in enumerate(lines):
  file_base_name = line.split()[0]
  file_base_name = os.path.splitext(file_base_name)[0] # sometime log file may have file name with extension
  cur_pose = line.split()[1:]
  print(cur_pose[0])
  cur_pose = cur_pose[0].split(',')
  print(cur_pose)

  cur_pose_tx = float(cur_pose[9])
  cur_pose_ty = float(cur_pose[10])
  cur_pose_tz = float(cur_pose[11])

  if params.ipRefDirFlag == 0:
    # for the flow where, each frame reference output is presnet in different file
    file_base_name = '{:010d}'.format(int(file_base_name)+1500)
    ref_file_name = file_base_name + '_' + feat_file_tag
    print(f'opening refrence file {ref_file_name}')
    try:
      with open(os.path.join(params.ipRefDir,ref_file_name)) as f:
          content = f.readlines()
    except Exception as e:
      sys.stderr.write('Could not open the reference folder file%s\n' %e)
      sys.exit(1)
    ref_pose_tx = float(content[0].split()[0])
    ref_pose_ty = float(content[0].split()[1])
    ref_pose_tz = float(content[0].split()[2])
  else:
    # for the flow where, each frame reference output is presnet in singel file
    cur_pose_ref = lines_ref[params.ipRefDirFlag + cur_frame_num*skip_fact].split()
    ref_pose_tx = float(cur_pose_ref[3])
    ref_pose_ty = float(cur_pose_ref[7])
    ref_pose_tz = float(cur_pose_ref[11])

  print(f'ref tx = {ref_pose_tx}, ty = {ref_pose_ty}, tz = {ref_pose_tz}')


  print(f'estimated tx = {cur_pose_tx}, ty = {cur_pose_ty}, tz = {cur_pose_tz}')

  cur_dist_err = np.sqrt((ref_pose_tx-cur_pose_tx)**2+
                          (ref_pose_ty-cur_pose_ty)**2 +
                          (ref_pose_tz-cur_pose_tz)**2)

  tot_dist_err += cur_dist_err
  if cur_dist_err > max_dist_err:
    max_dist_err = cur_dist_err

  print(f'Current distortion error is {cur_dist_err}')

  write_str= file_base_name
  write_str = write_str + ': ' + str(ref_pose_tx)+ ' ' + str(ref_pose_ty) + ' ' + str(ref_pose_tz)
  write_str = write_str + ' ' + str(cur_pose_tx)+ ' ' + str(cur_pose_ty)+ ' ' + str(cur_pose_tz) + '::' + str(cur_dist_err) + ' \n'

  file_log.write(write_str)

tot_dist_err = tot_dist_err/len(lines)
file_log.close()
print(f'Average error is {tot_dist_err}')
print(f'Max error is {max_dist_err}')

