"""
    DCC XML Generator Functions

    H3A MUX
"""

import os
import dcc
import dccxml
import utils
import shutil

def GenH3AMUXParams(handle, h3amux_params, cls_id):
    handle.write('                1, //enable\n')
    handle.write('                1, //number of LUTs\n')
    handle.write('                {\n')
    handle.write('                    {#include "lut_remap_16bto10b.txt"},\n')
    handle.write('                    {#include "lut_remap_16bto10b.txt"},\n')
    handle.write('                    {#include "lut_remap_16bto10b.txt"},\n')
    handle.write('                },\n')

def GenH3AMUXXML(directory, filebase, params, h3amux_params):

    if (os.path.exists(directory) == False):
        print ('Creating directory:  %s\n' %directory)
        try:
            os.makedirs(directory)
        except OSError as err:
            utils.error('%s: %s' %(err.strerror, err.filename), skip=True)

    filename = os.path.join(directory, '%s_%s.xml' %(params['SENSOR_NAME'], filebase));
    print ('Creating XML File:  %s\n' %filename)
    
    module_params = {}
    module_params['NAME']   = 'VISS_H3A_MUX_LUTS_CFG'
    module_params['STRUCT_NAME'] = 'iss_h3a_mux_luts'
    module_params['DCC_ID'] = dcc.DCC_ID_H3A_MUX
    module_params['FUNC_GENPARAMS'] = GenH3AMUXParams

    handle = dccxml.OpenFile(filename)
    dccxml.GenHeader(handle, params, module_params)

    # Create the DCC Structure definition
    handle.write('  <!--=======================================================================-->\n')
    handle.write('      <typedef>\n')
    handle.write('    <%s type="struct">\n' %module_params['STRUCT_NAME'])

    handle.write('       <enable type="uint16"> </enable> <!-- enable -->\n')
    handle.write('       <num_luts type="uint16"> </num_luts> <!-- number of LUTs (0 ~ 3) -->\n')
    handle.write('       <h3a_mux_luts type="uint16[3][639]"> </h3a_mux_luts> <!-- H3A LUTs -->\n')

    handle.write('      </%s>\n' %module_params['STRUCT_NAME'])
    handle.write('   </typedef>\n')
    handle.write('  <!--=======================================================================-->\n')

    # Create a DCC Use Case
    for i in range(h3amux_params['NUM_USE_CASE']):
        dccxml.AddUseCase(handle, module_params, h3amux_params['USE_CASE'][i])

    if(params['WDR_MODE'] == 'wdr'):
        import numpy as np
        import dcc_decompand
        gamma = params['GAMMA_PRE']
        n_bit = params['WDR_BIT_DEPTH']
        l_bit = params['H3A_INPUT_LSB']
        luti = dcc_decompand.lut_in(16)
        luto = np.around( (luti/2**16) ** (100/gamma) * 2**(n_bit - l_bit) )
        luto = np.minimum(1023, luto)
        luto.astype('uint16').tofile(os.path.join(directory, 'lut_remap_16bto10b.txt'), sep='\n')
    else:
        shutil.copy('../tables/lut_remap_16bto10b.txt', directory)

    dccxml.GenFooter(handle, module_params)
    dccxml.CloseFile(handle)
