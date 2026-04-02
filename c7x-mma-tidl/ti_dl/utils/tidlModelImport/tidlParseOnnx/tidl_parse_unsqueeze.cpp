
#include "tidl_parse_onnx.h"
using namespace std;
using namespace onnx;
 
template<> int32_t TidlParseOnnx:: parse<OnnxStr("Unsqueeze")> ()
{
    layer.layerType = TIDL_UnsqueezeLayer;
    layer.numInBufs = 1;
    NodeProto node  = graph.node(index);
      int32_t axes_status = -1, axesIdx = -1;
    int32_t j, ii, numDim = 0;
    int32_t num_axes_4_squeeze = -1;
    int32_t axes[TIDL_DIM_MAX];

    sTIDL_allowlistingMetaData md = layer.allowlistingMetaData;

    axesIdx = getAttrIdx(node, "axes");
    if (axesIdx != TIDL_IMPORT_DIAGNOSIS_RETURN_FAIL)
    {
        axes_status = axesIdx;
        num_axes_4_squeeze = node.attribute(axesIdx).ints_size();
        for(ii = 0; ii< num_axes_4_squeeze; ii++)
        {
        getIntAttr(node, "axes",   &axes[ii], ii);
        }
    }
    else if(md.numVarInputs <= 1)
    {
        /* axes can be an input in opset 18*/
        sBuffer_t buf;
        axes_status = copyFloatConst(graph, index, 1, buf, INPUT_REQUIRED);
        if (axes_status != TIDL_ALLOWLISTING_LAYER_CHECK_FAILED)
        {
            int32_t status = 0;
            TensorProto tensor = getInitializerTensor(graph, node.input(1), index, status);
            num_axes_4_squeeze = buf.bufSize;
            for (int i = 0; i < num_axes_4_squeeze; i++)
            {
                if (tensor.data_type() == TensorProto_DataType_INT64)
                {
                    axes[i] = (int32_t)(*((int64_t*)buf.ptr + i));
                }
                else if (tensor.data_type() == TensorProto_DataType_INT32)
                {
                    axes[i] = (int32_t)(*((int32_t*)buf.ptr + i));
                }
                else if (tensor.data_type() == TensorProto_DataType_INT8)
                {
                    axes[i] = (int32_t)(*((int8_t*)buf.ptr + i));
                }
                else if (tensor.data_type() == TensorProto_DataType_UINT8)
                {
                    axes[i] = (int32_t)(*((uint8_t*)buf.ptr + i));
                }
            }
        }
        free (buf.ptr);
    }
    else
    {
        TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Squeeze layer : Variable input for axes is not supported");
        return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
    }

    if (axes_status == TIDL_ALLOWLISTING_LAYER_CHECK_FAILED)
    {
        TIDL_LOG_UNSUPPORTED(gDiags.gDiagList, "Allowlisting : Squeeze layer : No axis given for Squeeze");
        return TIDL_ALLOWLISTING_LAYER_CHECK_FAILED;
    }

    /** If shape inference is not done on model, we assume the tensor is of 4 dimensions by default*/
    if (layer.allowlistingMetaData.varTensorsDims.size() != 0)
    {
        numDim = layer.allowlistingMetaData.varTensorsDims[0].size();
        numDim = (numDim == 0)? 4:numDim;
    }
    else
    {
        numDim = 4;
    }

    for(ii = 0; ii< TIDL_DIM_MAX; ii++)
    {
        layer.layerPCParams.unsqueezeParams.axes[ii] = 0;
        layer.layerPCParams.unsqueezeParams.axis[ii] = 0;
    }

    /* Updating the axes to TIDL max dimensions*/
    for (ii = 0; ii < num_axes_4_squeeze; ii++)
    {
        if(axes[ii] >= 0)
        {
          axes[ii] += (TIDL_DIM_MAX - (num_axes_4_squeeze + numDim));
        }
        else
        {
          axes[ii] += TIDL_DIM_MAX;
        }
        layer.layerPCParams.unsqueezeParams.axes[ii] = axes[ii];
    }

    for(ii = 0; ii< TIDL_DIM_MAX; ii++)
    {
        for(j=0; j < num_axes_4_squeeze; j++)
        {
            if(ii == axes[j])
            {
                layer.layerPCParams.unsqueezeParams.axis[ii] = 1; // squeeze this particular axis
            }
        }
    }
    return 0;
}