# TfLite Operator Mappings:

<center>

| S. No. | TfLite Operator | TIDL Layer | Constraints |
|:------:|:----------------|:-----------|:------|
| 1 | DepthwiseConv2d/Conv2d | TIDL_ConvolutionLayer | <ul> <li> Only one variable input is allowed </li><li> Number of non-singleton variable input dimensions must be less than <= 4 </li><li> Weight tensor dimension must match the kernel_shape </li><li> Stride must be the same along both horizontal and vertical dimensions </li><li> Kernel size 3x3 with stride 3 is not supported in AM62A and AM67A </li><li> Kernel size greater than 7 with stride 2 is not supported </li><li> Depthwise (Fully Grouped) convolution is only supported for 1x3s1, 3x3s1, 3x3s2, 5x5s1, 5x5s2, 7x7s1 & 7x7s2 filters </li><li> Padding greater than input width is not supported for AM62A and AM67A </li><li> Stride 4 is only supported with Kernel size 11x11 </li><li> Input width less than MAX(PadL, PadR) is not supported for AM62A and AM67A </li></ul> | 
| 2 | AveragePool2d/Mean/MaxPool2d | TIDL_PoolingLayer | <ul> <li> Input should be variable </li><li> Number of non-singleton variable input dimensions must be less than <= 4 </li><li> Pooling has been validated for the following kernel sizes: 3x3,2x2s,1x1 with stride 1 and stride 2 (both horizontal and vertical dimensions) </li></ul> | 
| 3 | Prelu/Relu | TIDL_ReLULayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 6 </li></ul> | 
| 4 | Mul/Add/Sub/Div | TIDL_EltWiseLayer | <ul> <li> Only 2 inputs are supported in Add/Mul/Sub/Div layers </li><li> Number of non-singleton variable input dimensions in Add/Mul/Sub/Div must be less than <= 6 </li><li> Both inputs as variable are supported in Sub only if firmware version >= 10_01_06_00 </li><li> The variable inputs in Add/Mul/Div/Sub layer must of be same dimensions or broadcast-able </li><li> Eltwise operator(Add/Mul/Div/Sub layer) is supported only with operands of similar dimensions or broadcast supported patterns of both inputs </li><li> 1D vector dimension should match with channel or width dimension </li></ul> | 
| 5 | FullyConnected | TIDL_InnerProductLayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 4 </li><li> Filter tensor input should have atleast 2 dimensions </li><li> Filter and input must be of same dimensions or broadcast-able </li><li> Bias tensor input should be a vector of [1, N] or [N] where N should match output dimension </li></ul> | 
| 6 | Softmax | TIDL_SoftMaxLayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 6 </li></ul> | 
| 7 | TransposeConv | TIDL_Deconv2DLayer | <ul> <li> Only one variable input is allowed </li><li> Number of non-singleton variable input dimensions must be less than <= 4 </li><li> Weight tensor size should match with proto kernel_shape </li><li> Only 4x4, 3x3 and 2x2 kernels with 2x2 stride are supported </li><li> Change to Resize if possible. Resize will be more efficient </li><li> 16-bit Deconvolution is not suppported on AM62A and AM67A </li><li> Only default dilations values (1,1) are supported </li></ul> | 
| 8 | Concatenation | TIDL_ConcatLayer | <ul> <li> Only supported across the width, height or channel axis </li><li> Not supported along the batch dimension </li><li> Number of non-singleton variable input dimensions must be less than <= 6 </li></ul> | 
| 9 | StridedSlice | TIDL_SliceLayer | <ul> <li> Must have 4 inputs </li><li> Only one input should be variable </li><li> Should have 3 constant inputs </li><li> Number of dimensions for variable inputs must be 4 </li><li> Number of dimensions for constant inputs must be 1 </li><li> Constant inputs must have 4 values </li><li> Number of dimensions for output tensor must be 4 </li><li> Only batch size = 1 is supported </li><li> Only supports non-batch dimension </li><li> Non-singular stride are not supported individually </li></ul> | 
| 10 | ArgMax | TIDL_ArgMaxLayer | <ul> <li> Only axis = -3 is supported </li><li> Number of non-singleton variable input dimensions must be less than <= 4 </li></ul> | 
| 11 | ResizeBilinear/ResizeNearestNeighbor | TIDL_ResizeLayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 4 </li></ul> | 
| 12 | DepthToSpace | TIDL_DepthToSpaceLayer | <ul> <li> Input should be four-dimensional (4D) </li><li> Input depth (channel dimension) should be multiple of (blocksize * blocksize) </li></ul> | 
| 13 | Logistic | TIDL_SigmoidLayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 6 </li></ul> | 
| 14 | Pad | TIDL_PadLayer | <ul> <li> Maximum number of input dimension supported is 6 </li><li> Padding is only supported for width/height axes </li><li> Pad layer is expected to provide 8 pad values </li></ul> | 
| 15 | Quantize/Dequantize | TIDL_DataConvertLayer | <ul> </ul> | 
| 16 | Squeeze | TIDL_SqueezeLayer | <ul> </ul> | 
| 17 | Tanh | TIDL_TanhLayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 6 </li></ul> | 
| 18 | Elu | TIDL_ELULayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 6 </li></ul> | 
| 19 | Reshape | TIDL_ReshapeLayer | <ul> <li> Variable shape is not supported </li><li> Number of non-singleton variable input dimensions must be less than <= 6 </li><li> Input volume should be equal to output volume </li></ul> | 
| 20 | Transpose | TIDL_TransposeLayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 6 </li><li> For firmware version < 10_01_04_00, only permutes are suported when number of dimensions > 4 </li><li> Transpose over batch dimension is only supported in firmware version >= 10_01_04_00 </li></ul> | 
| 21 | LeakyRelu | TIDL_LeakyReluLayer | <ul> <li> Number of non-singleton variable input dimensions must be less than <= 6 </li></ul> | 
| 22 | BatchToSpaceNd | TIDL_BatchToSpaceLayer | <ul> </ul> | 
| 23 | SpaceToBatchNd | TIDL_SpaceToBatchLayer | <ul> </ul> | 
| 24 | Pack | TIDL_PackLayer | <ul> </ul> | 
| 25 | Cast | TIDL_CastLayer | <ul> </ul> | 
| 26 | SpaceToDepth | TIDL_SpaceToDepthLayer | <ul> <li> Input should be four-dimensional (4D) </li><li> Input height and width should be multiple of blocksize </li></ul> | 

</center>
