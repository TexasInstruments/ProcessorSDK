import subprocess
import sys
import json
import os
import math
import gen_mem_addr_csv_j7

def usage():
    print("Usage: python main.py <SOC> <Autogen Path>")
    print("       python main.py -h                 for help")

if len(sys.argv) < 3:
    usage()
    exit()

if sys.argv[1] in ["-h", "--help"]:
    usage()
    exit()

# This module is importing clocks_inc.py from autogen.
# clocks_inc.py requires 2 arguments to execute.
# Here we are creating those arguments and adding to system args.

soc = sys.argv[1]
autogen_path = sys.argv[2]

arg_pass1 = "-s" + soc
arg_pass2 = autogen_path+"/soc_data/" + soc


sys.path.insert(0, autogen_path+'/scripts')
sys.argv = [autogen_path+'/scripts/clocks_inc.py', '-i', arg_pass2, '-s', soc]

import clocks_inc


def create_json_file(filename, data):
    # Create json file with given filename and data
    folder = os.path.dirname(filename)
    if not os.path.exists(folder):
        os.makedirs(folder)
    with open(filename, 'w') as outfile:
        json.dump(data, outfile, indent=4)

soc_clk_data=dict()
soc_clk_data_mux_parents=dict()
soc_clk_data_mux=dict()
soc_clk_data_div = dict()
soc_pll_data = dict()
soc_ip_clock_list = dict()

def process_clocks_data(clocks_inc):
    """Extracts clock data from clocks_inc.clk_data"""
    soc_clk_data_mux = {}
    soc_clk_data_mux_parents = {}
    soc_clk_data_div = {}
    soc_pll_data = {}

    for clk in clocks_inc.clk_data:
        struct_type = clk.type_name.split(' ')[3]
        name = clk.type_name.split(' ')[-1]
        if struct_type == 'clk_data_mux_reg':
            soc_clk_data_mux[name] = {}
            soc_clk_data_mux[name]["parents"] = clk.val["data_mux"]["parents"]
            soc_clk_data_mux[name]["reg"] = hex(eval(clk.val["reg"].replace('UL', '')))
            if "bit" in clk.val:
                soc_clk_data_mux[name]["start_bit"] = int(clk.val["bit"])
        elif struct_type == 'clk_parent':
            name = name.split('[')[0]
            soc_clk_data_mux_parents[name] = [{'clk_name': clk_name, 'div': int(div)} for clk_name, div in clk.val]
        elif struct_type == 'clk_data_div_reg':
            n=int(clk.val['data_div']["n"])
            if "start_at_1" in clk.val:
                soc_clk_data_div[name]["start_at_1"] = int(clk.val["start_at_1"])
                if int(clk.val["start_at_1"])==1:
                    n=n+1

            soc_clk_data_div[name] = {}
            soc_clk_data_div[name]["reg"] = hex(eval(clk.val["reg"].replace('UL', '')))
            soc_clk_data_div[name]["start_bit"] = int(clk.val["bit"])
            soc_clk_data_div[name]["bitfield_width"] = int(math.ceil(math.log2(n))) if n>1 else 1

        elif struct_type == 'clk_data_pll_16fft':
            soc_pll_data[name] = {}
            soc_pll_data[name]["base"] = clk.val["base"]
            soc_pll_data[name]["idx"] = int(clk.val["idx"])

    # calculate mux parent list size and add it to mux data
    for mux_reg in soc_clk_data_mux:
        parent_list_size = len(soc_clk_data_mux_parents[soc_clk_data_mux[mux_reg]["parents"]])
        soc_clk_data_mux[mux_reg]["bitfield_width"] = int(math.ceil(math.log2(parent_list_size)))

    return soc_clk_data_mux, soc_clk_data_mux_parents, soc_clk_data_div, soc_pll_data


def process_clock_data(clocks_inc):
    """Populates soc_clk_data dictionary with data from clocks_inc.clocks"""

    soc_clk_data = {}

    for name, clock in list(clocks_inc.clocks.items()):
        data = {}
        if "type" in clock:
            data['type'] = clock['type']

        if "parent" in clock:
            data['parent'] ={'clk_name': clock['parent'][0], 'div': int(clock['parent'][1])}

        if 'drv' in clock:
           data['drv']=clock['drv'].split('.')[0][1:]

        if 'data' in clock:
            data_name = clock['data'].split('.')[0][1:]
            data['data']=data_name

        if data:
            soc_clk_data[name] = data

    return soc_clk_data



def process_ip_clocks(clocks_inc):
    """Process clocks for IPs"""
    soc_ip_clock_list = {}

    for inst in clocks_inc.python_inc.abi_id_array:
        if inst is None:
            continue
        if inst['inst_design_name'] not in list(clocks_inc.clk_ips.keys()):
                continue;
        clk_ip = clocks_inc.clk_ips[inst['inst_design_name']]
        dev_clk_idx_name = '%s_DEV_%s' % (
                inst['soc_name'].upper(), inst['inst_name'].upper())
        soc_ip_clock_list[dev_clk_idx_name]=[]
        for name, conn in clk_ip.clk_in.items():
            if name not in clk_ip.clk_in_idx:
                    # Ignore non-assigned clock
                    continue
            if not clk_ip.clk_in_connected(name) or conn.src_inst.inst['inst_design_name'] not in clocks_inc.clk_ips:
                continue

            dev_clk_name = 'DEV_%s_%s' % (
                inst['inst_name'].upper(),
                name.upper())

            # Check for mux
            src_clk_ip = conn.src_inst
            soc_clk_name = conn.id()

            # Skip over dividers when adding device mux parents
            mux_conn = conn
            while src_clk_ip.div:
                mux_conn = src_clk_ip.conn_parent_of(mux_conn)
                src_clk_ip = mux_conn.src_inst

            if src_clk_ip is not None and src_clk_ip.mux and name in clk_ip.clk_mux_idx:
                soc_ip_clock_list[dev_clk_idx_name].extend([{"input_name":dev_clk_name,"clk_name":soc_clk_name,"div":mux_conn.div}])

            else:
                soc_ip_clock_list[dev_clk_idx_name].extend([{"input_name":dev_clk_name,"clk_name":soc_clk_name,"div":conn.div}])

    return soc_ip_clock_list



def load_json_file(file_path):
    """
    Load JSON data from a file and return it as a Python object.

    If the file does not exist, print an error message and return None.
    If the file contains invalid JSON, print an error message and return None.

    :param file_path: The path to the file to load.
    :return: The loaded JSON data, or None on error.
    """
    try:
        with open(file_path, 'r') as file:
            return json.load(file)
    except FileNotFoundError:
        print(f"File not found: {file_path}")
        return None
    except json.JSONDecodeError:
        print(f"Error decoding JSON from file: {file_path}")
        return None



def read_psc_data_from_json(data_dict):

    """
    Reads PSC data from json file as it is

    This dictionary will be used to store data for each PSC module.
    The key will be the LPSC name and the value will be a dictionary
    with the following keys:
    - gpsc_name: global PSC name
    - lpsc_name: local PSC name
    - base_addr: base address of the PSC module
    - lpsc_index: index of the PSC module
    - pd_name: name of the power domain
    - controlled_ip_instances: list of controlled ip instances

    The purpose of this step is to select only the required data from the
    json file and store it in a format that is easy to access later.
    """
    global pd_name_index

    psc_data = dict()
    pd_name_index =dict()
    name_map = dict()
    for ip in data_dict['ip_instances']:
         if 'ip_name' in ip and (ip["ip_name"] == "PSC" or ip["ip_name"] == "j7am_bolt_psc_wrap"):
           for pd in ip['psc_powerdomains']:
              pd_name_index[pd['pd_name']]=pd['pd_index']

         if 'ip_name' in ip :
            name_map[ip['inst_design_name']]=ip['inst_name']

    for ip in data_dict['ip_instances']:

        if 'ip_name' in ip and (ip["ip_name"] == "PSC" or ip["ip_name"] == "j7am_bolt_psc_wrap"):
            inst_name=ip['inst_name']
            # print(inst_name)
            base_addr = ip['regions'][0]['base']

            for pd in ip['psc_powerdomains']:
                pd_name_index[pd['pd_name']]=pd['pd_index']

            for psc_module in ip['psc_modules']:
                lpsc_name = psc_module['lpsc_name']
                # print(psc_module['lpsc_name'],psc_module['lpsc_index'],psc_module['pd'])
                # print(psc_module)
                controlled_ip_instances = list(set(ip['inst_design_name'] for ip in psc_module['controlled_ip_instances']))
                connected_ips=[name_map[ip] for ip in controlled_ip_instances]
                pd_index = pd_name_index[psc_module['pd']]
                psc_data[lpsc_name]= {'gpsc_name':inst_name,'lpsc_name':psc_module['lpsc_name'],'base_addr':base_addr,'lpsc_index':psc_module['lpsc_index'],'pd_name':psc_module['pd'],"pd_index":pd_index,'controlled_ip_instances':sorted(connected_ips)}

    return psc_data


soc_val=clocks_inc.python_inc.soc_name
soc_folder = os.path.join(os.path.dirname(os.path.dirname(os.path.dirname(__file__))), f'soc_data/{soc_val}')
soc_json_path = os.path.join(autogen_path, "soc_data", soc, f"{soc}.json")

soc_json_data = load_json_file(soc_json_path)

psc_hw_attributes = read_psc_data_from_json(soc_json_data)
soc_ip_clock_list = process_ip_clocks(clocks_inc)
soc_clk_data = process_clock_data(clocks_inc)
soc_clk_data_mux, soc_clk_data_mux_parents, soc_clk_data_div, soc_pll_data = process_clocks_data(clocks_inc)

create_json_file(f'{soc_folder}/soc_psc_data.json', psc_hw_attributes)
create_json_file(f'{soc_folder}/soc_clk_data.json', soc_clk_data)
create_json_file(f'{soc_folder}/soc_clk_data_mux_parents.json', soc_clk_data_mux_parents)
create_json_file(f'{soc_folder}/soc_clk_data_mux.json', soc_clk_data_mux)
create_json_file(f'{soc_folder}/soc_clk_data_div.json', soc_clk_data_div)
create_json_file(f'{soc_folder}/soc_pll_data.json', soc_pll_data)
create_json_file(f'{soc_folder}/soc_ip_clock_list.json', soc_ip_clock_list)

gen_mem_addr_csv_j7.createAddrFromSocData(soc_val, autogen_path)