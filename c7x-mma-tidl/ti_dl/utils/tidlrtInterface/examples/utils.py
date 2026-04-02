import sys
import os

def append_to_dict(main_dict, append_dict):
    '''
    Funtion to append one dictionary to another if key is not present in the
    main dictionaty

    @args
        main_dict : Dictionary to append to
        append_dict : Dictionary to append from
    '''
    for k,v in append_dict.items():
        if k not in main_dict:
            main_dict[k] = v

def parse_from_txt_file(filepath):
    '''
    Funtion to parse a text file and convert to dictionary where
    each line is a key,value pair seperated by '='

    @args
        filepath : Path to txt file
    '''
    config = {}
    try:
        with open(filepath, 'r') as file:
            for line_num, line in enumerate(file, 1):
                line = line.strip()
                if not line or line.startswith('#'):
                    continue
                if '=' not in line:
                    print(f"[WARNING] Line {line_num} doesn't contain '=' separator: {line}", file=sys.stderr)
                    continue
                key, value = line.split('=', 1)
                key = key.strip()
                value = value.strip()
                
                # Remove quotes from start and end iteratively (handles cases like ""value"")
                while value and (value.startswith('"') or value.startswith("'")):
                    value = value[1:]
                while value and (value.endswith('"') or value.endswith("'")):
                    value = value[:-1]
                config[key] = value
    except FileNotFoundError:
        print(f"[ERROR] Config file '{filepath}' not found.", file=sys.stderr)
        return {}
    except Exception as e:
        print(f"[ERROR] Reading config file '{filepath}': {e}", file=sys.stderr)
        return {}
    return config



def populate_config(config, arguments):
    '''
        Funtion to populate config dictionary from arguments

        @args
            config : ,Main config dictionary
            arguments : sys.argv
    '''
    txt_config = {}
    arg_config = {}
    i = 0
    while i < len(arguments):
        data = arguments[i].strip()
        if data == "--config":
            txt_config = parse_from_txt_file(arguments[i+1].strip())
        elif data.startswith("--"):
            key = data[2:]
            value = []
            while i+1 < len(arguments):
                data2 = arguments[i+1].strip()
                if data2.startswith("--"):
                    break
                value.append(data2)
                i+=1

            if len(value) == 1:
                arg_config[key] = value[0]
            elif len(value) > 1:
                arg_config[key] = ' '.join(value)
        i+=1

    append_to_dict(config, arg_config)
    append_to_dict(config, txt_config)