# loging of the file based on image file name, and collecting various statistics
import numpy as np
import glob, os
import sys, getopt
import csv
import struct

def print_help():
  print ('Comparing the results key points and descriptor')
  print ('python compare_results.py -i <1st set of files> -j <2nd set of files>')
#################################################################################
try:
  argv = sys.argv[1:]
  opts, args = getopt.getopt(argv,"hi:j:",["ipset1=",  "ipset2="])


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
    self.ipResultFile = ''
    self.ipRefDir = ''


  def displayArgs(self):
    print ("==================================")
    print ("ipset1    " , self.ipset1)
    print ("ipset2    " , self.ipset2)
    print ("==================================")

###################################################################################
params=Params()

for opt, arg in opts:
  if opt == '-h':
    print_help()
    sys.exit()
  elif opt in ("-i", "--ipset1"):
    params.ipset1 = arg
  elif opt in ("-j", "--ipset2"):
    params.ipset2 = arg

params.displayArgs()

for subdir, dirs, files in os.walk(params.ipset1):

    files.sort()    
    for file in files:
      if(os.path.splitext(file)[1] == '.txt'):
        print(f'reading the file {os.path.join(params.ipset1,file)}')
        
        try:
          with open(os.path.join(params.ipset1,file)) as f:
              set1 = f.readlines() 
              f.close()
        except Exception as e:
          sys.stderr.write('%s\n' %e)    
          sys.exit(1)
          
        if os.path.exists(os.path.join(params.ipset2,file)):
          print(f'reading the file {os.path.join(params.ipset2,file)}')
          f = open(os.path.join(params.ipset2,file))
          set2 = f.readlines()
          f.close() 

          for line_1 in set1[1:]:
            set1_cur_data = line_1.split()

            set1_cur_data_x = float(set1_cur_data[0])
            set1_cur_data_y = float(set1_cur_data[1])
            
            for line_2 in set2:
              set2_cur_data = line_2.split()
              set2_cur_data_x = float(set2_cur_data[0])
              set2_cur_data_y = float(set2_cur_data[1])    
              if  abs(set2_cur_data_x - set1_cur_data_x) < 0.5 and  abs(set2_cur_data_y - set1_cur_data_y) < 0.5:
                print(f'Match of the key point is found ({set1_cur_data_x} , {set1_cur_data_y}) with ({set2_cur_data_x},{set2_cur_data_y})')
                for k in range(64):
                  #print(f'{k}, {float(set1_cur_data[3+k])}, {(float(set2_cur_data[2+k])*2.0)}')
                  if abs(float(set1_cur_data[3+k])-(float(set2_cur_data[2+k])*1.0)) > 0.0:
                    print(f'Mismatch of the descriptor has happened for {k}th and mismatch are {float(set1_cur_data[3+k])}, {float(set2_cur_data[2+k])*1.0}')
                    break

            
      
      
            
      
      
      
      
      
  
  

