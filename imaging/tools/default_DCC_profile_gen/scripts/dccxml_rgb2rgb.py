"""
    DCC XML Generator Functions
    
    VISS RGB2RGB
"""

import os
import dcc
import dccxml
from utils import sample_cc
from utils import get_wdr_knee_points
from utils import g_tuning_output
import numpy as np

def wb_cc(cc_mean):

    w = np.diag([2.0, 5, 10, 15, 23, 31])
    w = np.sqrt(w)
    A = np.ones( (6,2), dtype='float')
    A[:,0] = np.array(cc_mean[-6:, 1])
    b = np.array([3324, 1696, 1024, 610, 344, 174], dtype='float')
    x = np.linalg.lstsq(np.matmul(w, A), np.matmul(w, b), rcond=None)[0]

    wb = np.zeros( (3,1), dtype='float')
    of = np.zeros( (3,1), dtype='float')
    wb[1] = round(x[0] * 512)
    of[1] = x[1] / x[0]

    b = np.array(cc_mean[-6:, 1], dtype='float')
    b = (b + of[1]) * wb[1] / 512

    w = np.diag([3.0, 5, 10, 15, 23, 31])
    w = np.sqrt(w)
    A[:,0] = np.array(cc_mean[-6:, 0])
    x = np.linalg.lstsq(np.matmul(w, A), np.matmul(w, b), rcond=None)[0]
    wb[0] = round(x[0] * 512)
    of[0] = x[1] / x[0]

    A[:,0] = np.array(cc_mean[-6:, 2])
    x = np.linalg.lstsq(np.matmul(w, A), np.matmul(w, b), rcond=None)[0]
    wb[2] = round(x[0] * 512)
    of[2] = x[1] / x[0]

    for k in range(3):
        cc_mean[:, k] = (cc_mean[:, k] + of[k]) * wb[k] / 512

    return cc_mean

def tune_ccm(cc_m):
    b = np.array([552,1567,410,355,700,443,2200,289,1567,397,997,2580,165,255,1272,2884,1444,0,3272,1696,1024,610,344,169,321,901,618,499,672,1460,636,379,361,208,1444,1048,213,877,161,1672,338,730,3324,1696,1024,610,344,174,249,691,984,240,1272,1160,137,1104,436,499,217,145,888,274,208,90,888,1104,3272,1696,1024,610,344,169], dtype='float')
    A = np.zeros([24*3, 9])
    A[ 0:24, 0:3] = cc_m
    A[24:48, 3:6] = cc_m
    A[48:,   6: ] = cc_m

    C = np.zeros([3,9])
    C[0, 0:3] = 1
    C[1, 3:6] = 1
    C[2, 6: ] = 1

    d = np.ones(3)
    W = np.array([256, 36, 441, 484, 196, 361, 16, 625, 36, 441, 100, 9, 961, 729, 64, 9, 64, 1681, 1, 1, 1, 1, 1, 1, 576, 121, 225, 289, 196, 64, 225, 441, 484, 784, 64, 100, 784, 144, 961, 16, 529, 169, 1, 1, 1, 1, 1, 1, 729, 196, 100, 729, 64, 81, 1156, 100, 361, 289, 784, 1089, 121, 676, 784, 1369, 121, 100, 1, 1, 1, 1, 1, 1], dtype='float')
    W = np.diag(W)

    H = np.matmul(np.matmul(A.T, W), A) + np.matmul(C.T, C)
    x0 = np.linalg.solve(H, np.matmul(np.matmul(A.T, W), b))
    T = np.linalg.pinv(np.matmul(C, np.linalg.solve(H, C.T)))
    T = np.matmul(C.T, T)
    T = np.matmul(T, d - np.matmul(C, x0))
    x = x0 + np.linalg.solve(H, T)

    return x


def GenRGB2RGB1Params(handle, rgb2rgb_params, cls_id, params):

    if params['RAW_RGB2RGB']:
        xp, yp = [], []
        if params['WDR_MODE'] == 'wdr':
            xp = get_wdr_knee_points(params['XP'])
            yp = get_wdr_knee_points(params['YP'])
            blk_pre = g_tuning_output.get('wdr_black_pre', 0)
            blk_post = g_tuning_output.get('wdr_black_post', 0)
        else: # linear mode
            blk_pre = 0
            blk_post = g_tuning_output.get('linear_black_level', 0)

        _, cc_mean = sample_cc(params['RAW_RGB2RGB'][cls_id][0],
                               params['WIDTH'],
                               params['HEIGHT'],
                               params['COLOR'],
                               xp, yp,
                               blk_pre,
                               0,   #blk_post,
                               0)
        cc_mean2 = wb_cc(cc_mean.copy())
        ccm = tune_ccm(cc_mean2)
        ccm = np.around(ccm*256).astype('int').reshape([3,3])

        for i in range(3):
            s = sum(ccm[i, :])
            if s != 256:
                ccm[i,i] = ccm[i,i] + 256 - s
    else:
        ccm = np.diag([256,256,256])

    g_tuning_output[f'ccm_{cls_id}'] = ccm

    handle.write('                {\n')
    handle.write('                    {{ {:4d}, {:4d}, {:4d}, 0}},\n'.format(ccm[0,0], ccm[0,1], ccm[0,2]))
    handle.write('                    {{ {:4d}, {:4d}, {:4d}, 0}},\n'.format(ccm[1,0], ccm[1,1], ccm[1,2]))
    handle.write('                    {{ {:4d}, {:4d}, {:4d}, 0}} \n'.format(ccm[2,0], ccm[2,1], ccm[2,2]))
    handle.write('                },\n')
    handle.write('                {0,0,0}\n')

def GenRGB2RGBXML(directory, filebase, params, RGB2RGB_params):

    if (os.path.exists(directory) == False):
        print ('Creating directory:  %s\n' %directory)
        try:
            os.makedirs(directory)
        except OSError as err:
            from utils import error
            error('%s: %s' %(err.strerror, err.filename), skip=True)

    filename = os.path.join(directory, '%s_%s.xml' %(params['SENSOR_NAME'], filebase));
    print ('Creating XML File:  %s\n' %filename)
    	
    module_params = {}
    module_params['NAME']   = 'ipipe_rgb2rgb_dcc.xml'
    module_params['STRUCT_NAME'] = 'cfg_rgb2rgb'
    module_params['DCC_ID'] = dcc.DCC_ID_IPIPE_RGB_RGB_1
    module_params['FUNC_GENPARAMS'] = GenRGB2RGB1Params
    
    handle = dccxml.OpenFile(filename)
    dccxml.GenHeader(handle, params, module_params)
    
    # Create the DCC Structure definition
    handle.write('    <!--=======================================================================-->\n')
    handle.write('  <typedef>\n')
    handle.write('			<cfg_rgb2rgb type="struct">\n')
    handle.write('        <!-- [RR] [GR] [BR] [CR]\n')
    handle.write('             [RG] [GG] [BG] [CG]\n')
    handle.write('             [RB] [GB] [BB] [CB]-->\n')
    handle.write('        <!-- Blending + Chroma values (S12Q8 format) -->\n')
    handle.write('			    <ccm type="int16[3][4]"> </ccm> <!-- ipipe rgb2rgb matrix: S12Q8 -->\n')
    handle.write('					<!-- Blending offset value for R,G,B - (S13) -->\n')
    handle.write('					<offset type="int16[3]"> </offset> <!-- ipipe rgb2rgb1 offset: S13 -->\n')
    handle.write('    </cfg_rgb2rgb>\n')
    handle.write('</typedef>\n')
    handle.write('<!--=======================================================================-->\n')
    
    # Create a DCC Use Case
    for i in range(RGB2RGB_params['NUM_USE_CASE']):
        module_params['tuning_params'] = {
                'RAW_RGB2RGB': params.get('RAW_RGB2RGB', None),
                'WIDTH': params['SENSOR_WIDTH'],
                'HEIGHT': params['SENSOR_HEIGHT'],
                'COLOR': params['COLOR_PATTERN'],
                'XP': params['WDR_KNEE_X'],
                'YP': params['WDR_KNEE_Y'],
                'WDR_MODE': params['WDR_MODE'],
                }
        dccxml.AddUseCase(handle, module_params, RGB2RGB_params['USE_CASE'][i])
    dccxml.GenFooter(handle, module_params)
    dccxml.CloseFile(handle)
    return 