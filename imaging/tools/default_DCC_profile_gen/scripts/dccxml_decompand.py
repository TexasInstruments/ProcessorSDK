"""
    DCC XML Generator Functions

    WDR Decompand
"""

import os
import dcc
import dccxml
import shutil
import utils
from utils import raw_mean
from utils import get_wdr_knee_points
from utils import g_tuning_output

def GenDECMPParams(handle, decmp_params, cls_id, tuning_params):

    bitd, mask = 12, 4095 
    if tuning_params['BIT_DEPTH'] > 12:
        bitd, mask = 16, 65535

    handle.write('		          1     // enable\n')
    handle.write('		          {}  // mask\n'.format(mask))
    handle.write('		          0     // shift\n')
    handle.write('		          {}    // lutBitdepth\n'.format(bitd))
    handle.write('		          65535 // lutClip\n')
    handle.write('		          {\n')
    handle.write('		              #include "lut_rawfe_pwl_vshort.txt"\n')
    handle.write('		          }\n')

def GenDECMPXML(directory, filebase, params, decmp_params):

    if (os.path.exists(directory) == False):
        print ('Creating directory:  %s\n' %directory)
        try:
            os.makedirs(directory)
        except OSError as err:
            utils.error('%s: %s' %(err.strerror, err.filename), skip=True)

    filename = os.path.join(directory, '%s_%s.xml' %(params['SENSOR_NAME'], filebase));
    print ('Creating XML File:  %s\n' %filename)

    module_params = {}
    module_params['NAME']   = 'VISS_RAWFE_decmp_cfg'
    module_params['STRUCT_NAME'] = 'cfg_rawfe_decompand'
    module_params['DCC_ID'] = dcc.DCC_ID_IPIPE_DECMP
    module_params['FUNC_GENPARAMS'] = GenDECMPParams

    handle = dccxml.OpenFile(filename)
    dccxml.GenHeader(handle, params, module_params)

    # Create the DCC Structure definition
    handle.write('  <!--=======================================================================-->\n')
    handle.write('      <typedef>\n')
    handle.write('    <%s type="struct">\n' %module_params['STRUCT_NAME'])

    handle.write('			<pwl_vshort_lut_en type="uint16"> </pwl_vshort_lut_en>\n')
    handle.write('			<pwl_vshort_mask type="uint16"> </pwl_vshort_mask>\n')
    handle.write('			<pwl_vshort_shift type="uint16"> </pwl_vshort_shift>\n')
    handle.write('			<pwl_vshort_lut_bitdepth type="uint16"> </pwl_vshort_lut_bitdepth>\n')
    handle.write('			<pwl_vshort_lut_clip type="uint16"> </pwl_vshort_lut_clip>\n')
    handle.write('			<pwl_vshort_lut type="uint16[639]"> </pwl_vshort_lut>\n')

    handle.write('      </%s>\n' %module_params['STRUCT_NAME'])
    handle.write('   </typedef>\n')
    handle.write('  <!--=======================================================================-->\n')

    # Create a DCC Use Case
    for i in range(decmp_params['NUM_USE_CASE']):
        module_params['tuning_params'] = {
			'BIT_DEPTH': params['BIT_DEPTH'],
		}
        dccxml.AddUseCase(handle, module_params, decmp_params['USE_CASE'][i])

    r_table = '../tables/lut_rawfe_pwl_vshort.txt'
    if params['WDR_MODE'] == 'linear':
        if params['BIT_DEPTH'] == 8:
            r_table = '../tables/lut_remap_8bto16b.txt'
        elif params['BIT_DEPTH'] == 10:
            r_table = '../tables/lut_remap_10bto16b.txt'
        elif params['BIT_DEPTH'] == 12:
            r_table = '../tables/lut_remap_12bto16b.txt'
        else:
            print('Warning: BIT_DEPTH={} in linear mode is not supported (fall back to 12)'.format(params['BIT_DEPTH']))
            r_table = '../tables/lut_remap_12bto16b.txt'
        shutil.copy(r_table, os.path.join(directory, 'lut_rawfe_pwl_vshort.txt'))

    else: # WDR mode
        import dcc_decompand
        import numpy as np

        bit_in = params['BIT_DEPTH']
        luti = dcc_decompand.lut_in(bit_in)

        gam = params['GAMMA_PRE'] / 100.0

        knee_x = get_wdr_knee_points(params['WDR_KNEE_X'])
        knee_y = get_wdr_knee_points(params['WDR_KNEE_Y'])

        bit_out = params['WDR_BIT_DEPTH']
        black_pre = params['BLACK_PRE']
        black_post = params['BLACK_POST']

        if type(black_pre) == str:
            black_pre = int(raw_mean(black_pre, [], []))

        if type(black_post) == str:
            black_post = int(raw_mean(black_post, knee_x, knee_y))

        g_tuning_output['wdr_black_pre'] = black_pre
        g_tuning_output['wdr_black_post'] = black_post

        luto = np.maximum(0, luti - black_pre)
        luto = np.maximum(0, np.interp(luto, knee_x, knee_y) - black_post)
        luto = np.around( (luto / luto[-1]) ** gam * 65535 )
        luto = np.array(luto, dtype='uint16')
        luto.tofile(os.path.join(directory, 'lut_rawfe_pwl_vshort.txt'), sep='\n')

    dccxml.GenFooter(handle, module_params)
    dccxml.CloseFile(handle)
