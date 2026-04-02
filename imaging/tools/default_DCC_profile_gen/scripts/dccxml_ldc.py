"""
    DCC XML Generator Functions
    
    Mesh LDC
"""

import os
import dcc
import dccxml
import numpy as np
import utils

def gtc_normalize(x, y, x_c, y_c, f):
    xn = (x - x_c) / f;
    yn = (y - y_c) / f;
    return xn, yn

def gtc_denormalize(x, y, x_c, y_c, f):
    xn = x * f + x_c
    yn = y * f + y_c
    return xn, yn

def pol2cart(theta, rho):
    x = rho * np.cos(theta)
    y = rho * np.sin(theta)
    return x, y

def cart2pol(x, y):
    theta = np.arctan2(y, x)
    rho = np.sqrt(x**2 + y**2)
    return theta, rho

def gtc_projection(lut, x, y, z):
    [phi, r] = cart2pol(x, y)
    theta = np.arctan2(r, z)
    rho = np.interp(theta/np.pi*180, lut[:,0], lut[:,1]);
    xp, yp = pol2cart(phi, rho)
    return xp, yp

def gtc_rotate_2D(x, y, phi):
    m = np.array([[np.cos(phi), -np.sin(phi)], 
                    [np.sin(phi),  np.cos(phi)]]) 

    t = np.vstack((x.ravel(), y.ravel()))
    t = m.dot(t)
    xr = t[0,:].reshape(x.shape)
    yr = t[1,:].reshape(y.shape)

    return xr, yr

def read_spec(spec_file, pitch_in_mm, f_in_mm=0):
    lut = np.loadtxt(spec_file)
    f_nominal = lut[1,1] / np.tan(lut[1,0] * np.pi / 180)

    if f_in_mm > 0:
        lut[:,1] = lut[:,1] / f_nominal * f_in_mm
    else:
        f_in_mm = f_nominal

    lut[:,1] /= pitch_in_mm
    return lut, f_in_mm

def gen_mesh(sz,       # mesh frame size        : [w,h]
             center,   # input image center     : [hc, vc]
             sview,    # focal scale factor     :
             f,        # focal length           : in pixel
             m,        # mesh down sampling     : 1, 2, 3, 4, ...
             lut       # lut
             ):

    smp_x = np.arange(sz[0]+1)
    smp_y = np.arange(sz[1]+1)
    
    h_p, v_p = np.meshgrid(smp_x, smp_y)
    hn, vn = gtc_normalize(h_p, v_p, center[0], center[1], f/sview)
    zn = np.ones(hn.shape)

    h_dn, v_dn = gtc_projection(lut, hn, vn, zn)
    h_d, v_d = gtc_denormalize(h_dn, v_dn, center[0], center[1], 1)
    h_d = np.maximum(0, np.minimum(h_d, sz[0]-2))
    v_d = np.maximum(0, np.minimum(v_d, sz[1]-2))

    h_delta = np.around((h_d - h_p) * 8)
    v_delta = np.around((v_d - v_p) * 8)

    mh = h_delta[::2**m, ::2**m].astype('int16')
    mv = v_delta[::2**m, ::2**m].astype('int16')

    return mh, mv

def add_mesh_lut_row_padding(mh, mv):
    lut = np.zeros([mh.shape[0], mh.shape[1]*2], dtype='int16')
    lut[:, 0::2] = mv
    lut[:, 1::2] = mh
    lut = lut.view('uint16')

    row_size = lut.shape[1]                       # in # of uint16_t
    row_pitch = int(np.ceil(row_size / 8) * 8)    # in # of uint16_t

    fstr = '0x{:04x},'
    pad = fstr.format(0) * (row_pitch - row_size) if row_pitch > row_size else ''
    text = ''
    for row in lut:
        text = text + ''.join( (fstr.format(k) for k in row) ) + pad + '\n'

    return text, row_pitch*2, row_pitch*lut.shape[0]
   
def gen_ldc_mesh_lut_with_padding(h_s, v_s, h_c, v_c, view_s, m, p_in_mm, f_lut):
    sz = [h_s, v_s]        # mesh frame size
    center = [h_c, v_c]    # image center in input

    lut, f_in_mm = read_spec(f_lut, p_in_mm)
    f = f_in_mm / p_in_mm

    mh, mv = gen_mesh(sz, center, view_s, f, m, lut)
    lut_txt, row_pitch, mesh_lut_size = add_mesh_lut_row_padding(mh, mv)

    return lut_txt, row_pitch, mesh_lut_size

def GenLDCParams(handle, LDC_params, cls_id):
	return

def GenLDCXML(directory, filebase, params, LDC_params):

    if (os.path.exists(directory) == False):
        print ('Creating directory:  {}\n'.format(directory))
        try:
            os.makedirs(directory)
        except OSError as err:
            utils.error('{}: {}'.format(err.strerror, err.filename), skip=True)

    filename = os.path.join(directory, '{}_{}.xml'.format(params['SENSOR_NAME'], filebase));
    print ('Creating XML File:  {}\n'.format(filename))

    module_params = {}
    module_params['NAME']   = 'ldc_odc'
    module_params['STRUCT_NAME'] = 'cfg_ldc_vars'
    module_params['DCC_ID'] = dcc.DCC_ID_MESH_LDC
    module_params['FUNC_GENPARAMS'] = GenLDCParams

    width = params.get('LDC_INPUT_WIDTH', params['SENSOR_WIDTH'])
    height = params.get('LDC_INPUT_HEIGHT', params['SENSOR_HEIGHT'])
    m = params.get('LDC_MESH_DOWNSAMPLE_M', 3)

    mesh_lut_txt = '                0, 0, 0, 0,     0, 0, 0, 0, \n' * 2
    row_pitch_in_bytes = 16
    mesh_lut_size  = 16
    if 'LDC_SENSOR_PIXEL_PITCH_IN_MM' in params and 'LDC_LENS_SPEC_FILE' in params:
        h_c = params.get('LDC_INPUT_H_CENTER', width / 2)
        v_c = params.get('LDC_INPUT_V_CENTER', height / 2)
        v_s = params.get('LDC_OUTPUT_VIEW_SCALER', 1)
        mesh_lut_txt, row_pitch_in_bytes, mesh_lut_size = gen_ldc_mesh_lut_with_padding(
                width,
                height,
                h_c,
                v_c,
                v_s,
                m,
                params['LDC_SENSOR_PIXEL_PITCH_IN_MM'],
                params['LDC_LENS_SPEC_FILE'])

    handle = dccxml.OpenFile(filename)
    dccxml.GenHeader(handle, params, module_params)
    
    # Create the DCC Structure definition
    handle.write('  <!--=======================================================================-->\n')
    handle.write('      <typedef>\n')
    handle.write('      <{} type="struct"> \n'.format(module_params['STRUCT_NAME']))
    handle.write('                      <ldc_en type="uint16"> </ldc_en>\n')
    handle.write('                      <ldc_ldmapen type="uint16"> </ldc_ldmapen>\n')
    handle.write('                      <ldc_datamode type="uint16"> </ldc_datamode>\n')
    handle.write('                      <ldc_opdatamode type="uint16"> </ldc_opdatamode>\n')
    handle.write('                      <ldc_ip_dfmt type="uint16"> </ldc_ip_dfmt>\n')
    handle.write('                      <ldc_pwarpen type="uint16"> </ldc_pwarpen>\n')
    handle.write('                      <ldc_yint_typ type="uint16"> </ldc_yint_typ>\n')
    handle.write('                      <ldc_regmode_en type="uint16"> </ldc_regmode_en>\n')
    handle.write('                      <ldc_meshtable_m type="uint16"> </ldc_meshtable_m>\n')
    handle.write('                      <ldc_mesh_frsz_w type="uint16"> </ldc_mesh_frsz_w>\n')
    handle.write('                      <ldc_mesh_frsz_h type="uint16"> </ldc_mesh_frsz_h>\n')
    handle.write('                      <ldc_compute_frsz_w type="uint16"> </ldc_compute_frsz_w>\n')
    handle.write('                      <ldc_compute_frsz_h type="uint16"> </ldc_compute_frsz_h>\n')
    handle.write('                      <ldc_initx type="uint16"> </ldc_initx>\n')
    handle.write('                      <ldc_inity type="uint16"> </ldc_inity>\n')
    handle.write('                      <ldc_input_frsz_w type="uint16"> </ldc_input_frsz_w>\n')
    handle.write('                      <ldc_input_frsz_h type="uint16"> </ldc_input_frsz_h>\n')
    handle.write('                      <ldc_obw type="uint16"> </ldc_obw>\n')
    handle.write('                      <ldc_obh type="uint16"> </ldc_obh>\n')
    handle.write('                      <ldc_pixpad type="uint16"> </ldc_pixpad>\n')
    handle.write('                      <ldc_a type="int16"> </ldc_a>\n')
    handle.write('                      <ldc_b type="int16"> </ldc_b>\n')
    handle.write('                      <ldc_c type="int16"> </ldc_c>\n')
    handle.write('                      <ldc_d type="int16"> </ldc_d>\n')
    handle.write('                      <ldc_e type="int16"> </ldc_e>\n')
    handle.write('                      <ldc_f type="int16"> </ldc_f>\n')
    handle.write('                      <ldc_g type="int16"> </ldc_g>\n')
    handle.write('                      <ldc_h type="int16"> </ldc_h>\n')
    handle.write('                      <ldc_sf_width type="uint16[3]"> </ldc_sf_width>\n')
    handle.write('                      <ldc_sf_height type="uint16[3]"> </ldc_sf_height>\n')
    handle.write('                      <ldc_sf_en  type="uint16[3][3]"> </ldc_sf_en>\n')
    handle.write('                      <ldc_sf_obw type="uint16[3][3]"> </ldc_sf_obw>\n')
    handle.write('                      <ldc_sf_obh type="uint16[3][3]"> </ldc_sf_obh>\n')
    handle.write('                      <ldc_sf_pad type="uint16[3][3]"> </ldc_sf_pad>\n')
    handle.write('                      <ldc_ylut_en type="uint16"> </ldc_ylut_en>\n')
    handle.write('                      <ldc_yin_bitdpth type="uint16"> </ldc_yin_bitdpth>\n')
    handle.write('                      <ldc_yout_bitdpth type="uint16"> </ldc_yout_bitdpth>\n')
    handle.write('                      <ldc_clut_en type="uint16"> </ldc_clut_en>\n')
    handle.write('                      <ldc_cin_bitdpth type="uint16"> </ldc_cin_bitdpth>\n')
    handle.write('                      <ldc_cout_bitdpth type="uint16"> </ldc_cout_bitdpth>\n')
    handle.write('                      <ldc_y_lut type="uint16[513]"> </ldc_y_lut>\n')
    handle.write('                      <ldc_c_lut type="uint16[513]"> </ldc_c_lut>\n')
    handle.write('              		<mesh_table_pitch_in_bytes type="uint32"> </mesh_table_pitch_in_bytes>\n')
    handle.write('              		<mesh_table_size type="uint32"> </mesh_table_size>\n')
    handle.write('              		<mesh_lut type="uint16*"> </mesh_lut>\n')

    handle.write('      </{}> \n'.format(module_params['STRUCT_NAME']))
    
    handle.write('   </typedef>\n')
    handle.write('  <!--=======================================================================-->\n')


    handle.write('		<use_case val="65535"> \n')
    handle.write('        		<n-space>\n')
    handle.write('        			<region0 class="0">\n')
    handle.write('        				<gain             val="0" min="0" max="10240">     </gain>\n')
    handle.write('        				<exposure         val="1" min="0" max="10000000">  </exposure>\n')
    handle.write('        				<colortemperature val="2" min="0" max="10000">     </colortemperature>\n')
    handle.write('        			</region0>\n')
    handle.write('        		</n-space>\n')
    handle.write('        <parameter_package>\n')
    handle.write('            <ldc_dcc type="{}">\n'.format(module_params['STRUCT_NAME']))

    handle.write('            {\n')
    handle.write('                0     // LDC_CTRL  LDC_EN(0)         LDC Enable, 0: Disable,  1: Enable\n')
    handle.write('                1     // LDC_CTRL  LDMAPEN(1)        LD Mapping enable, 0: disable, 1: enable\n')
    handle.write('                2     // LDC_CTRL  DATAMODE(4:3)     Input data mode, 0:YUV422, 1:Y only, 2:YUV420, 3:YUV420 UV\n')
    handle.write('                1     // LDC_CTRL  OP_DATAMODE  Output data mode, 0: keep UYVY; 1: convert to 420\n')
    handle.write('                0     // LDC_CTRL  IP_DFMT(6:5) Input pixel format, 0:8b, 1:12b packed, 2:12b unpacked\n')
    handle.write('                1     // LDC_CTRL  PWARPEN(7)   Perspective warp 0: Disable . 1: Enable\n')
    handle.write('                1     // LDC_CFG   YINT_TYP(6)  Interpolation type for Y .  0: Bicubic,  1: Bilinear\n')
    handle.write('                0     // LDC_CFG   REGMODE_EN           Region mode, 0: disable, 1: enable\n')
    handle.write('                {}     // LDC_MESHTABLE_CFG     M(2:0)   Mesh table subsampling factor (0-7)\n'.format(m))
    handle.write('                {}  // LDC_MESH_FRSZ       W(13:0)  Mesh frame width (0-8192)\n'.format(width))
    handle.write('                {}  // LDC_MESH_FRSZ       H(29:16)             Mesh frame height (0-8192)\n'.format(height))
    handle.write('                {}   // LDC_COMPUTE_FRSZ      W(13:0)  Compute width (0-8192)\n'.format(width))
    handle.write('                {}   // LDC_COMPUTE_FRSZ      H(29:16)             Compute height (0-8192)\n'.format(height))
    handle.write('                 0     // LDC_INITXY    INITX(13:0)  Output starting horizontal coordinate (0-8192)\n')
    handle.write('                 0     // LDC_INITXY    INITY(29:16) Output starting vertical coordinate (0-8192)\n')
    handle.write('                {}  // LDC_INPUT_FRSZ        W(29:16)             Input frame width\n'.format(width))
    handle.write('                {}  // LDC_INPUT_FRSZ        H(13:0)  Input frame height\n'.format(height))
    handle.write('                128    // LDC_BLOCK_SIZE        OBW(7:0)             Output block width (0-255)\n')
    handle.write('                64    // LDC_BLOCK_SIZE        OBH(15:8)            Output block height (0-255)\n')
    handle.write('                1     // LDC_BLOCK_SIZE        PIXPAD(19:16)        Pixel pad (0-15)\n')
    handle.write('                4096  // LDC_AB    A(15:0)  Affine Transform warp, A S16Q12\n')
    handle.write('                0     // LDC_AB    B(31:16)             Affine Transform warp, B S16Q12\n')
    handle.write('                0     // LDC_CD    C(15:0)  Affine Transform warp, C S16Q3\n')
    handle.write('                0     // LDC_CD    D(31:16)             Affine Transform warp, D S16Q12\n')
    handle.write('                4096  // LDC_EF    E(15:0)  Affine Transform warp, E S16Q12\n')
    handle.write('                0     // LDC_EF    F(31:16)             Affine Transform warp, F S16Q3\n')
    handle.write('                0     // LDC_GH    G(15:0)  Affine Transform warp, G S16Q23\n')
    handle.write('                0     // LDC_GH    H(31:16)             Affine Transform warp, H S16Q23\n')
    handle.write('                {0, 0, 0}     //ldc_sf_width [3]\n')
    handle.write('                {0, 0, 0}     //ldc_sf_height[3]\n')
    handle.write('                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}     //ldc_sf_en [3][3]\n')
    handle.write('                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}     //ldc_sf_obw[3][3]\n')
    handle.write('                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}     //ldc_sf_obh[3][3]\n')
    handle.write('                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}     //ldc_sf_pad[3][3]\n')
    handle.write('                0     // LDC_DUALOUT_CFG       YLUT_EN  Luma LUT enable (0-1)\n')
    handle.write('                8     // LDC_DUALOUT_CFG       YIN_BITDPTH          Luma input bit depth (8-12)\n')
    handle.write('                8     // LDC_DUALOUT_CFG       YOUT_BITDPTH         Luma output bit depth (8-12)\n')
    handle.write('                0     // LDC_DUALOUT_CFG       CLUT_EN  Chroma LUT enable (0-1)\n')
    handle.write('                8     // LDC_DUALOUT_CFG       CIN_BITDPTH          Chroma input bit depth (8-12)\n')
    handle.write('                8     // LDC_DUALOUT_CFG       COUT_BITDPTH         Chroma output bit depth (8-12)\n')
    handle.write('                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},\n')

    handle.write('                {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},\n')

    handle.write('                {},\n'.format(row_pitch_in_bytes))
    handle.write('                {},\n'.format(mesh_lut_size))
    handle.write('                {\n')
    handle.write(mesh_lut_txt)
    handle.write('                }\n')
    handle.write('            }\n')

    handle.write('            </ldc_dcc>\n')
    handle.write('        </parameter_package>\n')
    handle.write('		</use_case> \n')

    dccxml.GenFooter(handle, module_params)
    dccxml.CloseFile(handle)
