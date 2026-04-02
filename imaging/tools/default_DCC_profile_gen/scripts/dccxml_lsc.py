"""
    DCC XML Generator Functions
    
    LSC
"""

import os
import io
import dcc
import dccxml
import sys
import utils
import numpy as np

# read spec file for 4 colors in 5 columns
def read_spec(spec_file, pitch_in_mm, gamma):
    spec_lut = np.loadtxt(spec_file)
    spec_lut_4c = np.zeros([spec_lut.shape[0], 5])
    if spec_lut.shape[1] == 5:
        spec_lut_4c = spec_lut
    elif spec_lut.shape[1] == 2:
        spec_lut_4c[:,:2] = spec_lut
        spec_lut_4c[:,2] = spec_lut[:,1]
        spec_lut_4c[:,3] = spec_lut[:,1]
        spec_lut_4c[:,4] = spec_lut[:,1]
    else:
        sys.exit('spec lut file format is wrong')

    spec_lut_4c[:, 0] = spec_lut_4c[:, 0] / pitch_in_mm
    for i in range(1, 5):
        spec_lut_4c[:, i] = (spec_lut_4c[0, i] / spec_lut_4c[:, i]) ** gamma

    return spec_lut_4c

def gen_lsc_lut(spec_file, pitch_in_mm, W, H, hc, vc, gamma_pre):
# spec_file         LSC shading property file
# pitch_in_mm       sensor pixel pitch
# W                 image width
# H                 image height
# hc                horiztonal image center (1x1 or 4x1)
# vc                vertical image center (1x1 or 4x1)

    spec_lut_4c = read_spec(spec_file, pitch_in_mm, gamma_pre)

    # N: LSC LUT down-sampling factor (8, 16, 32, 64, or 128)
    for N in [3,4,5,6,7]:
        S = int(np.ceil(W / 2**N + 1)) * int(np.ceil(H / 2**N + 1)) * 4
        if  S <= 19456:
            break
    n = 2**N

    # calculate LSC LUT table size
    h_end = np.ceil(W / n) * n
    v_end = np.ceil(H / n) * n
    [xx, yy] = np.meshgrid(np.arange(0, h_end+1, n), np.arange(0, v_end+1, n))
    g_tbl_4c = np.zeros([xx.shape[0], xx.shape[1]*4])

    # calculate LSC LUT
    for i in range(4):
        r = np.sqrt( (xx - hc)**2 + (yy - vc)**2 )
        r = np.minimum(r, spec_lut_4c[-1, 0])
        g_tbl_4c[:,i::4] = np.interp(r, spec_lut_4c[:,0], spec_lut_4c[:,i+1])

    # Q: LSC LUT integer encoding format (5, 6, 7, or 8) with +1
    max_gain = g_tbl_4c.max()
    for Q in [8, 7, 6, 5]:
        if (max_gain - 1) * 2**Q <= 255 * 1.01:
            break

    g_tbl_4c = np.around((g_tbl_4c - 1) * 2**Q)
    g_tbl_4c = np.minimum(255, np.maximum(g_tbl_4c, 0))

    string_buffer = io.StringIO()
    np.savetxt(string_buffer , g_tbl_4c.astype('uint8'), fmt="%3d")
    lut_out = string_buffer.getvalue()
    string_buffer.close()

    return lut_out, N, Q, S


def GenLSCParams(handle, LSC_params, cls_id, tuning_params):
    spec_file = tuning_params['LSC_LENS_SPEC_FILE']
    pitch = tuning_params['PITCH']
    W = tuning_params['WIDTH']
    H = tuning_params['HEIGHT']
    hc = tuning_params['LSC_INPUT_H_CENTER']
    vc = tuning_params['LSC_INPUT_V_CENTER']

    gamma_pre = 1.0
    if tuning_params['WDR_MODE']:
        gamma_pre = tuning_params['GAMMA_PRE'] / 100
    lut_text, N, Q, S = gen_lsc_lut(spec_file, pitch, W, H, hc, vc, gamma_pre)

    handle.write('  1           // lsc_cfg_enable         0:disable, 1: enable\n')
    handle.write('  {}           // lsc_cfg_gain_mode_m    3:8x, 4:16x, 5: 32x, 6: 64x, 7: 128x\n'.format(N))
    handle.write('  {}           // lsc_cfg_gain_mode_n    3:8x, 4:16x, 5: 32x, 6: 64x, 7: 128x\n'.format(N))
    handle.write('  {}           // lsc_cfg_gain_format    0:Q8, 1:Q8+1, 2:Q7, 3:Q7+1, 4:Q6, 5:Q6+1, 6:Q5, 7:Q5+1\n'.format(17 - 2*Q))
    handle.write('  {}           // lsc_cfg_lut_size       in bytes\n'.format(S))
    handle.write('  {\n')
    handle.write(lut_text)
    handle.write('  }\n')
    return

def GenLSCXML(directory, filebase, params, LSC_params):

    if (os.path.exists(directory) == False):
        print ('Creating directory:  {}\n'.format(directory))
        try:
            os.makedirs(directory)
        except OSError as err:
            utils.error('{}: {}'.format(err.strerror, err.filename), skip=True)

    filename = os.path.join(directory, '{}_{}.xml'.format(params['SENSOR_NAME'], filebase));
    print ('Creating XML File:  {}\n'.format(filename))

    module_params = {}
    module_params['NAME']   = 'cfg_rawfe_lsc'
    module_params['STRUCT_NAME'] = 'cfg_lsc_vars'
    module_params['DCC_ID'] = dcc.DCC_ID_LSC
    module_params['FUNC_GENPARAMS'] = GenLSCParams

    module_params['tuning_params'] = {
            'WIDTH': params['SENSOR_WIDTH'],
            'HEIGHT': params['SENSOR_HEIGHT'],
            'WDR_MODE': params['WDR_MODE'],
            'GAMMA_PRE': params['GAMMA_PRE'],
            'PITCH': params['LSC_SENSOR_PIXEL_PITCH'],
            'SPEC_FILE': params['LSC_LENS_SPEC_FILE'],
            'LSC_INPUT_H_CENTER': params['LSC_INPUT_H_CENTER'],
            'LSC_INPUT_V_CENTER': params['LSC_INPUT_V_CENTER'],
            'LSC_LENS_SPEC_FILE': params['LSC_LENS_SPEC_FILE'],
            }

    if 'LSC_INPUT_WIDTH' in params:
        module_params['tuning_params']['WIDTH'] = params['LSC_INPUT_WIDTH']
    if 'LSC_INPUT_HEIGHT' in params:
        module_params['tuning_params']['HEIGHT'] = params['LSC_INPUT_HEIGHT']

    handle = dccxml.OpenFile(filename)
    dccxml.GenHeader(handle, params, module_params)
    
    # Create the DCC Structure definition
    handle.write('  <!--=======================================================================-->\n')
    handle.write('    <typedef>\n')
    handle.write('        <{} type="struct"> \n'.format(module_params['STRUCT_NAME']))
    handle.write('            <lsc_cfg_enable type="uint32"> </lsc_cfg_enable>\n')
    handle.write('            <lsc_cfg_gain_mode_m type="uint32"> </lsc_cfg_gain_mode_m>\n')
    handle.write('            <lsc_cfg_gain_mode_n type="uint32"> </lsc_cfg_gain_mode_n>\n')
    handle.write('            <lsc_cfg_gain_format type="uint32"> </lsc_cfg_gain_format>\n')
    handle.write('            <lsc_cfg_lut_size    type="uint32"> </lsc_cfg_lut_size>\n')
    handle.write('            <lsc_lut type="uint8*"> </lsc_lut>\n')
    handle.write('        </{}> \n'.format(module_params['STRUCT_NAME']))
    handle.write('    </typedef>\n')
    handle.write('  <!--=======================================================================-->\n')

    # Create a DCC Use Case
    for i in range(LSC_params['NUM_USE_CASE']):
        dccxml.AddUseCase(handle, module_params, LSC_params['USE_CASE'][i])
    
    dccxml.GenFooter(handle, module_params)
    dccxml.CloseFile(handle)

