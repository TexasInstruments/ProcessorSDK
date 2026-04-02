#include <drivers/dss.h>
#include <drivers/dss/v0/soc/dss_soc.h>

#define DSI_TOP                    (CSL_DSS_DSI0_DSI_TOP_VBUSP_CFG_DSI_0_DSI_BASE)
#define DSI_TOP_SIZE               (DSI_TOP + CSL_DSS_DSI0_DSI_TOP_VBUSP_CFG_DSI_0_DSI_SIZE)
#define DSI_WRAPPER                (CSL_DSS_DSI0_DSI_WRAP_MMR_VBUSP_CFG_DSI_WRAP_BASE)
#define DSI_WRAPPER_SIZE           (DSI_WRAPPER + CSL_DSS_DSI0_DSI_WRAP_MMR_VBUSP_CFG_DSI_WRAP_SIZE)
#define DSI_DPHY                   (CSL_DPHY_TX0_BASE)
#define DSI_DPHY_SIZE              (DSI_DPHY + CSL_DPHY_TX0_SIZE)


uint32_t CPS_ReadReg32(volatile uint32_t* address)
{
    uint32_t data;
    uintptr_t address_int = (uintptr_t)address;

    if (((address_int >= DSI_TOP) && (address_int < DSI_TOP_SIZE)) ||
             ((address_int >= DSI_DPHY) && (address_int < DSI_DPHY_SIZE)) ||
             ((address_int >= DSI_WRAPPER) && (address_int < DSI_WRAPPER_SIZE)))
    {
        data = CSL_REG32_RD(address_int);
    }
    else
    {
        GT_1trace(DssTrace,
                  GT_ERR,
                  "Address %d (read) doesn't map to any DP bus",
                  address);
        data = 0u;
    }

    return data;
}

void CPS_WriteReg32(volatile uint32_t* address, uint32_t value)
{
    uintptr_t address_int = (uintptr_t)address;

    if (((address_int >= DSI_TOP) && (address_int < DSI_TOP_SIZE)) ||
             ((address_int >= DSI_DPHY) && (address_int < DSI_DPHY_SIZE)) ||
             ((address_int >= DSI_WRAPPER) && (address_int < DSI_WRAPPER_SIZE)))
    {
        CSL_REG32_WR(address_int, value);
    }
    else
    {
        GT_1trace(DssTrace,
                  GT_ERR,
                  "Address %d (write) doesn't map to any DP bus",
                  address);
    }
}
