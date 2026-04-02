# Quantization Proto : Providing quantization parameters to TIDL

## Steps to provide the range values via quantization proto:

---
  1. Search for the parameter "quant_params_proto_path" under advanced_options in the config settings.
  2. Specify the path to the the prototext file containing calibrated quantization parameters of your model inordr to use your quantization parameters.
  3. If you want to generate a prototxt file which you wish to use in the future to bypass calibration, provide the path to the file that you want to generate. TIDL's import module will generate a file at that path and fill it up with the model's quantization parameters.
---
The proto file provided is expected to follow the following layout:
```protobuf
// Top level configuration for the message storing TIDL Network Quantization Parameters
message TIDLNetQuantParams {
  optional int32 num_layers = 1;
  optional TidlQuantType quant_type = 2 [default = SYMMETRIC];
  optional TidlCalibType calib_type = 3 [default = PERTENSOR];
  repeated TidlLayerQuantParams layers = 4;
}

// Quantization Type
enum TidlQuantType {
SYMMETRIC = 1;
ASYMMETRIC = 2;
}

// Calibration Type
enum TidlCalibType {
PERTENSOR = 1;
PERCHANNEL = 2;
}

// Configuration for storing TIDL Layer Quantization Parameters
message TidlLayerQuantParams {
  optional string layer_name = 1; 
  optional string layer_type = 2;
  optional uint32 bit_depth = 3 [default = 8];
  repeated TidlTensorQuantParams outputs = 4;
  repeated TidlTensorQuantParams weights = 5;
  repeated TidlTensorQuantParams bias = 6;
  repeated TidlTensorQuantParams slope = 7;
}

// Configuration for storing TIDL Tensor Quantization Parameters
message TidlTensorQuantParams {
  optional float min = 1 [default = -3.4028234664e+38]; 
  optional float max = 2 [default = 3.4028234664e+38]; 
  optional uint32 size = 3;
  optional uint32 element_type = 4;
  repeated double scale = 5 [packed=true];
  repeated uint32 zero_point = 6 [packed=true];  
  repeated double value = 7 [packed=true];
}

```

**Inorder to be able to bypass the calibration step make sure that:**
- Every layer has mimimum and maximum values for all of its tensors
- If any layer has a bias tensor, make sure it has all the bias values and the size of that tensor should be equal to the number of instances of values in it.

Following is an instance of a valid prototxt file for a dummy model with 2 layers:
```protobuf text format
num_layers: 2
quant_type: SYMMETRIC
calib_type: PERTENSOR
layers {
  layer_name: "data"
  layer_type: "TIDL_DataLayer"
  bit_depth: 1
  outputs {
    min: 0
    max: 255
    size: 1
    element_type: 0
    scale: 1
    zero_point: 0
  }
}
layers {
  layer_name: "conv1a"
  layer_type: "TIDL_ConvolutionLayer"
  bit_depth: 1
  outputs {
    min: 0
    max: 4.09663105
    size: 1
    element_type: 0
    scale: 56.387794494628906
    zero_point: 0
  }
  weights {
    min: -0.00830631796
    max: 0.00886716694
    size: 1
    element_type: 0
    scale: 14435.275390625
    zero_point: 0
  }
  bias {
    min: -0.52766174077987671
    max: 0.53365200757980347
    size: 3
    element_type: 0
    scale: 120.27982330322266
    zero_point: 0
    value: 0.28929620981216431
    value: -0.52766174077987671
    value: 0.53365200757980347
  }
}
```
Reasons for this file being considered valid:
- Number of layers are consistent with number of layer instances
- Each tensor has valid minimum and maximum values
- Bias tensor has consistent values
  

**Note**: Make sure the path you provide does not have a file. If you provide a path to an empty file, no quantization parameters will be exported into your file.

## Steps to troubleshoot
In case you are unable to bypass the calibration step, your file might have one of the following issues:

- Invalid format:
  
    If your file does not have the format expected by the TIDL's artifact generation architecture, it wont get parsed.

    Following can be an example for invalid formatting caused by missing texts:
    ```protobuf text format
    ype: SYMMETRIC
    calib_type: PERTENSOR
    layers {
      layer_name: "data"
      layer_type: "TIDL_DataLayer"
      bit_depth: 1
      outpu
    
    layers {
      layer_name: "conv1a"
      layer_type: "TIDL_ConvolutionLayer"
      hts {
        min: -0.00830631796
        max: 0.00886716694
        size: 1
        element_type: 0
        scale: 14435.275390625
        zero_point: 0
      }
      bias {
        min: -0.52766174077987671
        max: 0.53365200757980347
        size: 3
        element_type: 0
        scale: 120.27982330322266
        zero_point: 0
        valu
      }
    }
    ```

- Inadequate data:
  
  The proto file provided by you might be lacking data for one or more layers that is crucial inorder to bypass calibration.

    Following is an example of a proto file that has inadequate data:
    ```protobuf text format
    num_layers: 2
    quant_type: SYMMETRIC
    calib_type: PERTENSOR
    layers {
      layer_name: "data"
      layer_type: "TIDL_DataLayer"
      bit_depth: 1
      outputs {
        min: FLT_MIN
        max: FLT_MAX
        size: 1
        element_type: 0
        scale: 1
        zero_point: 0
      }
    }
    layers {
      layer_name: "conv1a"
      layer_type: "TIDL_ConvolutionLayer"
      bit_depth: 1
      outputs {
        min: 0
        max: FLT_MAX
        size: 1
        element_type: 0
        scale: 56.387794494628906
        zero_point: 0
      }
      weights {
        min: -0.00830631796
        max: 0.00886716694
        size: 1
        element_type: 0
        scale: 14435.275390625
        zero_point: 0
      }
      bias {
        min: FLT_MIN
        max: 0.53365200757980347
        size: 3
        element_type: 0
        scale: 120.27982330322266
        zero_point: 0
      }
    }
    ```
    What makes it inadequate:
    - Tensors have min/max values set to FLT_MIN/FLT_MAX.
    - Inconsistent Bias values.

- The file provided by you might be empty.
