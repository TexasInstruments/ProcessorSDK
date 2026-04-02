"""
    DCC XML Generator Functions
    
    PCID
"""

import os
import dcc
import dccxml
import shutil
import utils

def GenVISS_PCID_Bayer_out(pcid_i_fmt):
    color_map = { 
                 0: 0,  #RGGI --> RGGB 
                 1: 1,  #GRIG --> GRBG
                 2: 0,  #BGGI --> RGGB
                 3: 1,  #GBIG --> GRBG
                 4: 2,  #GIRG --> GBRG
                 5: 3,  #IGGR --> BGGR
                 6: 2,  #GIBG --> GBRG
                 7: 3,  #IGGB --> BGGR
                }
    return color_map[pcid_i_fmt-10]
   
def GenVISS_PCIDParams(handle, VISS_PCID_params, cls_id, tuning_params):

    pcid_i_fmt = 0
    if 10 <= tuning_params['COLOR_PATTERN'] < 18:
        pcid_i_fmt = int(tuning_params['COLOR_PATTERN']) - 10

    handle.write('              {}                          // pcid_i_fmt      // RGBIR format: 0~7\n'.format(pcid_i_fmt))
    handle.write('              0                           // pcid_o_fmt      // 0: B at IR; 1: R at IR\n')
    handle.write('              0                           // pcid_o_sel      // 0: IR to DDR; 1: IR to VISS\n')
    handle.write('              8192                        // pcid_ha_th1\n')
    handle.write('              16320                       // pcid_ha_th2\n')
    handle.write('              32768                       // pcid_ha_th3\n')
    handle.write('              128                         // pcid_hfx_scale\n')
    handle.write('              128                         // pcid_hfx_scale_ir\n')
    handle.write('              {256,256,256,256}           // pcid_irsub_scale\n')
    handle.write('              64745                       // pcid_irsub_cutoff\n')
    handle.write('              5                           // pcid_irsub_trans_bw\n')
    handle.write('              51                          // pcid_irsub_trans_bw_recip\n')
    handle.write('              {256,256,192,128,64}        // pcid_dist_factor[5]\n')
    handle.write('              1                           // pcid_remap      // remapLUT enable\n')
    handle.write('              {\n')
    handle.write('                  #include "lut_pcid_remap_16bto8b_shf8_blk0.txt"    // 16b to 8b\n')
    handle.write('              }\n')

def GenVISS_PCIDXML(directory, filebase, params, VISS_PCID_params):

    if (os.path.exists(directory) == False):
        print ('Creating directory:  %s\n' %directory)
        try:
            os.makedirs(directory)
        except OSError as err:
            utils.error('%s: %s' %(err.strerror, err.filename), skip=True)

    filename = os.path.join(directory, '%s_%s.xml' %(params['SENSOR_NAME'], filebase));
    print ('Creating XML File:  %s\n' %filename	)

    module_params = {}
    module_params['NAME']   = 'VISS_PCID'
    module_params['STRUCT_NAME'] = 'cfg_pcid_vars'
    module_params['DCC_ID'] = dcc.DCC_ID_PCID
    module_params['FUNC_GENPARAMS'] = GenVISS_PCIDParams
    
    handle = dccxml.OpenFile(filename)
    dccxml.GenHeader(handle, params, module_params)
    
    # Create the DCC Structure definition
    handle.write('  <!--=======================================================================-->\n')
    handle.write('      <typedef>\n')
    handle.write('      <%s type="struct"> \n' %module_params['STRUCT_NAME'])

    handle.write('            <pcid_i_fmt type="uint32"> </pcid_i_fmt>\n')
    handle.write('            <pcid_o_fmt type="uint32"> </pcid_o_fmt>\n')
    handle.write('            <pcid_o_sel type="uint32"> </pcid_o_sel>\n')
    handle.write('            <pcid_ha_th1 type="uint32"> </pcid_ha_th1>\n')
    handle.write('            <pcid_ha_th2 type="uint32"> </pcid_ha_th2>\n')
    handle.write('            <pcid_ha_th3 type="uint32"> </pcid_ha_th3>\n')
    handle.write('            <pcid_hfx_scale type="uint32"> </pcid_hfx_scale>\n')
    handle.write('            <pcid_hfx_scale_ir type="uint32"> </pcid_hfx_scale_ir>\n')
    handle.write('            <pcid_irsub_scale type="uint32[4]"> </pcid_irsub_scale>\n')
    handle.write('            <pcid_irsub_cutoff type="uint32"> </pcid_irsub_cutoff>\n')
    handle.write('            <pcid_irsub_trans_bw type="uint32"> </pcid_irsub_trans_bw>\n')
    handle.write('            <pcid_irsub_trans_bw_recip type="uint32"> </pcid_irsub_trans_bw_recip>\n')
    handle.write('            <pcid_dist_factor type="uint32[5]"> </pcid_dist_factor>\n')
    handle.write('            <pcid_remap type="uint16"> </pcid_remap>\n')
    handle.write('            <pcid_remap_lut type="uint16[609]"> </pcid_remap_lut>\n')

    handle.write('      </%s> \n' %module_params['STRUCT_NAME'])
    handle.write('   </typedef>\n')
    handle.write('  <!--=======================================================================-->\n')
    
    # Create a DCC Use Case
    for i in range(VISS_PCID_params['NUM_USE_CASE']):
        module_params['tuning_params'] = {
			'COLOR_PATTERN': params['COLOR_PATTERN'],
			#'WDR_MODE': params['WDR_MODE'],
		}
        dccxml.AddUseCase(handle, module_params, VISS_PCID_params['USE_CASE'][i])
    
    dccxml.GenFooter(handle, module_params)
    dccxml.CloseFile(handle)

    shutil.copy('../tables/lut_pcid_remap_16bto8b_shf8_blk0.txt', directory)


