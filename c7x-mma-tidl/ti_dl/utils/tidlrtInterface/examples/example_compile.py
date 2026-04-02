import tidlruntime
import numpy as np
import sys
import os
current_dir = os.path.dirname(os.path.abspath(__file__))
if current_dir not in sys.path:
    sys.path.append(current_dir)
from utils import populate_config

config = {}
if len(sys.argv) > 1:
    populate_config(config, sys.argv[1:])

session = tidlruntime.CompileSession(config)

input_details = session.get_input_details()
input_dict = {}
# Fill input dict based on input_details if needed
session.run(input_dict)