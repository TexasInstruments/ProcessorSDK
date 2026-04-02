"""
    DCC XML Generator Functions
    
    H3A
"""

import os
import dcc
import dccxml
import utils

def GenH3A_AEWBParams(handle, h3a_aewb_params, cls_id):
    return

def GenH3A_AEWBXML(directory, filebase, params, h3a_aewb_params):

    if (os.path.exists(directory) == False):
        print ('Creating directory:  %s\n' %directory)
        try:
            os.makedirs(directory)
        except OSError as err:
            utils.error('%s: %s' %(err.strerror, err.filename), skip=True)

    filename = os.path.join(directory, '%s_%s.xml' %(params['SENSOR_NAME'], filebase));
    print ('Creating XML File:  %s\n' %filename	)

    module_params = {}
    module_params['NAME']   = 'VISS_H3A_AEWB_CFG'
    module_params['STRUCT_NAME'] = 'iss_h3a_grid_size'
    module_params['DCC_ID'] = dcc.DCC_ID_H3A_AEWB_CFG
    module_params['FUNC_GENPARAMS'] = GenH3A_AEWBParams
    
    handle = dccxml.OpenFile(filename)
    dccxml.GenHeader(handle, params, module_params)
    
    # Create the DCC Structure definition
    handle.write('  <!--=======================================================================-->\n')
    handle.write('      <typedef>\n')
    handle.write('      <%s type="struct"> \n' %module_params['STRUCT_NAME'])

    handle.write('      <enable type="uint8"> </enable> <!-- enable h3a aewb-->\n')
    handle.write('      <mode type="uint8"> </mode> <!-- 0 = SUM_SQ, 1=MINMAX, 2=SUM_ONLY-->\n')
    handle.write('      <v_start type="uint16"> </v_start> <!-- Paxel_0 Start Coordinate Y in Pixels -->\n')
    handle.write('      <h_start type="uint16"> </h_start> <!-- Paxel_0 Start Coordinate H in Pixels -->\n')
    handle.write('      <v_size type="uint8"> </v_size> <!-- Paxel Height in Pixels -->\n')
    handle.write('      <h_size type="uint8"> </h_size> <!-- Paxel Width in Pixels -->\n')
    handle.write('      <v_count type="uint8"> </v_count> <!-- number of vertical paxels -->\n')
    handle.write('      <h_count type="uint8"> </h_count> <!-- number of horizontal paxels -->\n')
    handle.write('      <v_skip type="uint8"> </v_skip> <!--   vertical subsampling factor (0-15) -->\n')
    handle.write('      <h_skip type="uint8"> </h_skip> <!--  horizontal subsampling factor (0-15) -->\n')
    handle.write('      <saturation_limit type="uint16"> </saturation_limit> <!--  saturation_limit (0-1023) -->\n')
    handle.write('      <blk_win_numlines type="uint16"> </blk_win_numlines> <!--  Win Height for the single black line of windows (2-256)-->\n')
    handle.write('      <blk_row_vpos type="uint16"> </blk_row_vpos> <!--  Vertical Position of Black Row -->\n')
    handle.write('      <sum_shift type="uint8"> </sum_shift> <!--  Sum Shift (0-15) -->\n')
    handle.write('      <ALaw_En type="uint8"> </ALaw_En> <!--  A Law Enable (0/1) -->\n')
    handle.write('      <MedFilt_En type="uint8"> </MedFilt_En> <!--  Median Filter Enable (0/1) -->\n')

    handle.write('      </%s> \n' %module_params['STRUCT_NAME'])
    
    handle.write('   </typedef>\n')
    handle.write('  <!--=======================================================================-->\n')

    #Default config is 32x16 windows
    w, h = params['SENSOR_WIDTH'], params['SENSOR_HEIGHT']
    h_count, v_count = 32, 16
    if w < 32*32 + 8:
        h_count, v_count = 16, 16
    if w < 32*16 + 8:
        h_count, v_count = 8, 8

    h_size = (w - 8) // (2 * h_count) * 2
    v_size = (h - 8) // (2 * v_count) * 2

    npix = h_size * v_size // 4
    for k in range(1, 6):
        if npix <= 64 * 4**k:
            break
    h_skip = v_skip = 2**k
    h_start = (w - h_size * h_count) // 4 * 2
    v_start = (h - v_size * v_count) // 4 * 2

    blk_win_numlines = 2
    blk_row_vpos = params['SENSOR_HEIGHT'] - 4

    handle.write('		<use_case val="65535"> \n')
    
    handle.write('        		<n-space>\n')
    handle.write('        			<region0 class="0">\n')
    handle.write('        				<gain                      val="0" min="0" max="10240">  </gain>\n')
    handle.write('        				<exposure              val="1" min="0" max="10000000">  </exposure>\n')
    handle.write('        				<colortemperature val="2" min="0" max="10000">  </colortemperature>\n')
    handle.write('        			</region0>\n')
    handle.write('        		</n-space>\n')   
    handle.write('        <parameter_package>\n')
    handle.write('            <h3a_aewb_dcc type="iss_h3a_grid_size">\n')
    handle.write('              {\n')
    handle.write('                  1,     // enable:   u8\n')
    handle.write('                  2,     // mode:     u8\n')
    handle.write('                  %d,     // v_start:  u16\n' %v_start)
    handle.write('                  %d,     // h_start:  u16\n' %h_start)
    handle.write('                  %d,     // v_size:   u8\n' %v_size)
    handle.write('                  %d,     // h_size:   u8\n' %h_size)
    handle.write('                  %d,     // v_count:  u8\n' %v_count)
    handle.write('                  %d,     // h_count:  u8\n' %h_count)
    handle.write('                  %d,     // v_skip:   u8\n' %v_skip)
    handle.write('                  %d,     // h_skip:   u8\n' %h_skip)
    handle.write('                  1000,     // saturation_limit: u16\n')
    handle.write('                  %d,     // blk_win_numlines: u16\n' %blk_win_numlines)
    handle.write('                  %d,     // blk_row_vpos:     u16\n' %blk_row_vpos)
    handle.write('                  2,     // Sum Shift:   u8\n')
    handle.write('                  0,     // ALaw_En:     u8\n')
    handle.write('                  0,     // MedFilt_En:  u8\n')
    handle.write('              }\n')
    handle.write('            </h3a_aewb_dcc>\n')
    handle.write('        </parameter_package>\n')
    handle.write('		</use_case> \n')

    dccxml.GenFooter(handle, module_params)
    dccxml.CloseFile(handle)
