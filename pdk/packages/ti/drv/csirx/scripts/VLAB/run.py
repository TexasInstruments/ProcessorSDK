import vlab
import os
import sys
import struct

# to select specific toolbox version in case multiple are installed
vlab.select_toolbox_version("keystone", (0, 14, 5, u'final', 0))
vlab.load(u"keystone_scripts.j7es_ccs", args= ["--testbench=testbench.py", "--no_c66_cluster", "--no_c7x_cluster"]  + __args__)
vlab.display_terminal(vlab.terminal.mcu_island_usart0);

# a variable to keep track if the image is transmitted or not
core = 'pulsar0_cr5f_0_proxy'
lines = 1080
columns = 1920
vc = 0
rgb_image_cnt = 0
raw_image_cnt = 0
_transmit_image_result = False

def transmit_image_result(success):
    global _transmit_image_result
    _transmit_image_result = success
    if not _transmit_image_result:
        print "ERROR: The image was not transmitted"
    else:
        print "The image was transmitted successfully"

def transmit_rgb_image(bp):
    global _transmit_image_result
    global lines
    global columns
    global vc
    global format
    global rgb_image_cnt
    _transmit_image_result = False

    ppi_generator = vlab.get_instance("PPI_GENERATOR0")
    # parameter 1: image name, parameter 2: lines, parameter 3: columns
    # columns should be in term of pixel number
    test_img = "test.png"
    vc = 0
    lines = 1080
    columns = 1920
    ppi_generator.obj.send_rgb_file(test_img, callback=transmit_image_result, vc=vc)
    rgb_image_cnt = rgb_image_cnt + 1

    print "RGB888 Image transmitted:", rgb_image_cnt

def transmit_raw12_image(bp):
    global _transmit_image_result
    global lines
    global columns
    global vc
    global format
    global raw_image_cnt
    _transmit_image_result = False

    ppi_generator = vlab.get_instance("PPI_GENERATOR0")
    # parameter 1: image name, parameter 2: lines, parameter 3: columns
    # columns should be in term of pixel number
    test_img = "test_raw.bin"
    vc = 0
    lines = 600
    columns = 800
    ppi_generator.obj.generate_raw12_image(test_img, lines, columns, random_seed=44)
    ppi_generator.obj.reset_vc(vc)
    ppi_generator.obj.send_raw12_file(test_img, lines, columns, transmit_image_result, vc)
    raw_image_cnt = raw_image_cnt + 1
    print "RAW12 Image transmitted:", raw_image_cnt

def send_images():
    global format
    global core
    vlab.run()
    print "::Supported Image Formats::"
    print "RGB888"
    print "RAW12"

    vlab.write_memory(0x80000000, 0x0, core=core, pack="=L")
    ip = struct.unpack('I', vlab.read_memory(0x80000000, 4, core=core))[0]
    print "Waiting for Application input..."
    while ip == 0:
        ip = struct.unpack('I', vlab.read_memory(0x80000000, 4, core=core))[0]

    format = struct.unpack('I', vlab.read_memory(0x80000004, 4, core=core))[0]
    _num_of_frames_to_send = struct.unpack('I', vlab.read_memory(0x80000008, 4, core=core))[0]
    print "Read inputs:"
    print "Format:" , format
    print "Number of frames to send per channel" , _num_of_frames_to_send
    #Send only one image each time periodic breakpoint is hit
    if format == 36:
        print "Image format detected: RGB888!!!"
        vlab.add_breakpoint(vlab.trigger.periodic(30, units='ms', periods=(_num_of_frames_to_send)), action=transmit_rgb_image)
    elif format == 44:
        print "Image format detected: RAW12!!!"
        vlab.add_breakpoint(vlab.trigger.periodic(30, units='ms', periods=(_num_of_frames_to_send)), action=transmit_raw12_image)
    else:
        print "Invalid choice of Format"

send_images()

