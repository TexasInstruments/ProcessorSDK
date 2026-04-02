"""
    J7 VPAC specific configurations and methods.
"""

import os
import inspect
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import PercentFormatter


REPORT_CALLER = True

WAIT_ON_EXIT = False
AWB_MAX_IMG_CNT = 20 

g_tuning_output = {}

def init_params():
    # INITIALIZATION START
    sys_params = {}
    sys_params['LSC'] = {}
    sys_params['AWB'] = {}
    sys_params['AE'] = {}
    sys_params['AE']['IMAGE'] = {}
    sys_params['AE']['VIDEO'] = {}
    sys_params['AE']['WIGHTTABLE'] = {}
    for _ in range (0,4):
        sys_params['LSC'][_] = {}
    for _ in range (0,AWB_MAX_IMG_CNT):
        sys_params['AWB'][_] = {}
    sys_params['GAMMA'] = {}
    sys_params['GAMMA']['VIDEO'] = {}
    sys_params['GAMMA']['IMAGE'] = {}
    sys_params['GAMMA']['IMAGE']['OUTDOOR']={}
    sys_params['GAMMA']['IMAGE']['INDOOR']={}
    sys_params['GAMMA']['IMAGE']['INDOOR_DARK']={}
    sys_params['GAMMA']['VIDEO']['OUTDOOR']={}
    sys_params['GAMMA']['VIDEO']['INDOOR']={}
    sys_params['GAMMA']['VIDEO']['INDOOR_DARK']={}
    sys_params['NOISE_PIPE'] ={}
    for _ in range (0,6):
        sys_params['NOISE_PIPE'][_] = {}
    sys_params['LSC']['FILECNT'] =0
    sys_params['AWB']['FILECNT'] =0
    sys_params['INPUT_TYPE'] = '.raw'
    # INITIALIZATION FINISH
    return sys_params


def error(message, skip=False, warn=False):
    """
        Print error message and quit if not skip.
        Print warning instead of error if warn.
    """

    if REPORT_CALLER:
        try:
            message = '[%s:%d] %s' %(inspect.stack()[2][3], inspect.stack()[2][2], message,)
        except IndexError:
            pass

    if warn:
        message = '[WARNING] %s' %message
    else:
        message = '[ERROR] %s' %message

    print(message)

    if not skip:
        if WAIT_ON_EXIT:
            input("Press ENTER to exit")
        os.sys.exit(1)



def get_params(fname):
    """
        Load configuration from file into specified dict.
        Lines beginning with # will be ignored as comments.
        Anything after the KEY VALUE double will be ignored.
        Single or multiple whitespace is exclusive delimiter.
    """
    params = init_params()
    try:
        config_file = open(fname, 'r')
    except IOError as err:
        error('Params file not found: %s' %err.filename, skip=True)
        return 1

    fileindex = 0
    for line in config_file:
        if line[0] == '#':
            continue
        config = line.split()[:10]
        """
            Config Length 2 for Sensor Parameters
            > 2 for LSC, AWB
        """
        comp_name_idx = line.find("AE")
        if ( comp_name_idx == 0 ):
            if len(config) == 3:
                params['AE'][config[1]] =config[2].strip()
            else:
                usecase = config[1]
                params['AE'][usecase][config[2]] = config[3]
            continue
        if len(config) == 2:
            try:
                params[config[0]] = config[1].strip()
            except ValueError:
                params[config[0]] = config[1]
            continue
        # LSC IMG_TYPE TEMP FILENAME %GECORRECT VALID
        comp_name_idx = line.find("LSC")
        if ( comp_name_idx == 0 ):
            val = int(config[5])
            temperature = int(config[2].strip())
            ShdCorrect = int(config[4].strip())
            if val == 1 :
                fileindex = params['LSC']['FILECNT']
                params['LSC'][fileindex]['TEMP'] = temperature
                try:
                    params['LSC'][fileindex]['FILENAME'] = config[3]
                except ValueError:
                    params['LSC'][fileindex]['FILENAME']  = config[3]
                params['LSC'][fileindex]['SHDCORECT'] = ShdCorrect
                params['LSC']['FILECNT'] = params['LSC']['FILECNT'] + 1
            continue
        # # AWB STD/ADL TEMP GAIN EXP FLASH APERTURE FILENAME VALID
        comp_name_idx = line.find("AWB")
        if ( comp_name_idx == 0 ):
            val = int(config[9])
            ImageType = config[1].strip()
            temperature = int(config[3].strip())
            Gain = int(config[4].strip())
            Exposure = int(config[5].strip())
            Flash = int(config[6].strip())
            Aperture = int(config[7].strip())
            if val == 1 :
                fileindex = params['AWB']['FILECNT']
                params['AWB'][fileindex]['IMG_TYP'] = ImageType
                params['AWB'][fileindex]['TEMP'] = temperature
                params['AWB'][fileindex]['GAIN'] = Gain
                params['AWB'][fileindex]['EXP'] = Exposure
                params['AWB'][fileindex]['FLASH'] = Flash
                params['AWB'][fileindex]['APERTURE'] = Aperture
                params['AWB'][fileindex]['FILENAME'] = config[8]
                params['AWB']['FILECNT'] = params['AWB']['FILECNT'] + 1
            continue

        # GAMMA VIDEO/IMAGE OUTDOOR/INDOOR/INDOORDARK GAMMAFILE VALID         
        comp_name_idx = line.find("GAMMA")
        if ( comp_name_idx == 0 ):
            usecase = config[1].strip()
            region = config[2].strip()
            params['GAMMA'][usecase][region]['FILENAME']=config[3].strip()
            continue 

        comp_name_idx = line.find("NOISE_PIPE")
        # NOISE_PIPE OUTDOOR?INDOOR OUTDOOR/HIGH/MID_HIGH/MID/MID_LOW/LOW NOISE SHARPNESS
        RegionList = {'OUTDOOR':0,'HIGH':1,'MID_HIGH':2,'MID':3,'MID_LOW':4,'LOW':5}
        if ( comp_name_idx == 0 ):
            key = str(config[2].strip())
            RegionIdx = RegionList[key]
            params['NOISE_PIPE'][RegionIdx]['NOISE'] = int(config[3].strip())
            params['NOISE_PIPE'][RegionIdx]['SHARPNESS'] = int(config[4].strip())
            continue
    config_file.close()
    return params

def get_wdr_knee_points(knee_str):
    kp = knee_str.split(',')
    return [int(k) for k in kp]

def raw_mean(fraw, xp, yp, plot=0):

    pix = np.fromfile(fraw, dtype='uint16')
    if xp:
        pix = np.interp(pix.astype('float'), xp, yp)
    
    pmean = np.around(pix.mean())

    if plot:

        pmax  = np.maximum(255, np.around(pix.max()))
        pmax  = np.percentile(pix, 99.99)
        plt.hist(pix, bins=np.arange(0, pmax+1), density=True)
        plt.gca().yaxis.set_major_formatter(PercentFormatter(1))
        plt.title(f'black level = {pmean:.1f}')
        plt.savefig(f'{os.path.basename(fraw)}_hist.png')
        plt.show()

    return pmean

def rgb_to_normalized_uv(R, G, B):
    Y = np.around( (77 * R + 150 * G + 29 * B + 128) / 256   )
    U = np.around( (-43 * R -  85 * G + 128 * B)     / (Y+1) )
    V = np.around( (128 * R - 107 * G -  21 * B)     / (Y+1) )
    return np.vstack( [Y, U, V] ).T

def sample_cc(fin, w, h, color, xp, yp, black_pre=0, black_post=0, plot=0):
    import cv2
    raw = np.fromfile(fin, dtype='uint16')
    raw = raw.reshape([h, w])
    rh, rv = color % 2, color // 2

    image = np.zeros(list(raw.shape) + [3], dtype='float')              # RGB
    image[:,:,0] = np.kron(raw[(0+rv)::2, (0+rh)::2], np.ones([2,2]))   # R
    image[:,:,1] = np.kron(raw[(1-rv)::2, (0+rh)::2], np.ones([2,2]))   # Gr
    image[:,:,2] = np.kron(raw[(1-rv)::2, (1-rh)::2], np.ones([2,2]))   # B

    im8 = (image * 255 / image.max()).astype('uint8')

    detector = cv2.mcc.CCheckerDetector_create()
    detector.process(im8[:,:,::-1], cv2.mcc.MCC24, 1)

    checkers = detector.getListColorChecker()
    checker = checkers[0]
    cc_pos = checker.getColorCharts()

    if plot:
        plt.imshow(im8)
        for k in range(0, 96, 4):
            l = [k, k+1, k+2, k+3, k]
            plt.plot(cc_pos[l, 0], cc_pos[l, 1], color='red', linewidth=1)
        plt.show()

    image = np.maximum(0, image - black_pre)
    if xp:
        image = np.interp(image, xp, yp)
    image = np.maximum(0, image - black_post)
    
    cc_b = np.zeros([24, 4])
    cc_m = np.zeros([24, 3], dtype='float')
    for k in range(0, 24):
        sh = int(np.ceil(cc_pos[4*k+0][0]))
        sv = int(np.ceil(cc_pos[4*k+0][1]))
        eh = int(np.floor(cc_pos[4*k+2][0]))
        ev = int(np.floor(cc_pos[4*k+2][1]))

        cc_b[k, 0] = sh
        cc_b[k, 1] = sv
        cc_b[k, 2] = eh - sh
        cc_b[k, 3] = ev - sv

        cc_m[k,0] = np.median(image[sv:ev, sh:eh, 0])
        cc_m[k,1] = np.median(image[sv:ev, sh:eh, 1])
        cc_m[k,2] = np.median(image[sv:ev, sh:eh, 2])

        #print(k, cc_b[k,:], cc_m[k,:])

    return cc_b, cc_m

