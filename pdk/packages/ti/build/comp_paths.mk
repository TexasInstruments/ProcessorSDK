################################################################################
# Individual component path, needed to build a component in a different location
# as that of PDK
################################################################################

PDK_SDL_ROOT_PATH ?= $(pdk_PATH)
export PDK_SDL_COMP_PATH = $(PDK_SDL_ROOT_PATH)/ti/sdl
PDK_CSL_ROOT_PATH ?= $(pdk_PATH)
export PDK_CSL_COMP_PATH = $(PDK_CSL_ROOT_PATH)/ti/csl
PDK_CSL2_ROOT_PATH  ?= $(pdk_PATH)
export PDK_CSL2_COMP_PATH = $(PDK_CSL2_ROOT_PATH)/ti/csl
PDK_PM_ROOT_PATH ?= $(pdk_PATH)
export PDK_PM_COMP_PATH = $(PDK_PM_ROOT_PATH)/ti/drv/pm
PDK_PMIC_ROOT_PATH ?= $(pdk_PATH)
export PDK_PMIC_COMP_PATH = $(PDK_PM_ROOT_PATH)/ti/drv/pmic
PDK_OSAL_ROOT_PATH ?= $(pdk_PATH)
export PDK_OSAL_COMP_PATH = $(PDK_OSAL_ROOT_PATH)/ti/osal
PDK_I2C_ROOT_PATH ?= $(pdk_PATH)
export PDK_I2C_COMP_PATH = $(PDK_I2C_ROOT_PATH)/ti/drv/i2c
PDK_GPIO_ROOT_PATH ?= $(pdk_PATH)
export PDK_GPIO_COMP_PATH = $(PDK_GPIO_ROOT_PATH)/ti/drv/gpio
PDK_GTC_ROOT_PATH ?= $(pdk_PATH)
export PDK_GTC_COMP_PATH = $(PDK_GTC_ROOT_PATH)/ti/drv/gtc
PDK_FATFS_ROOT_PATH ?= $(pdk_PATH)
export PDK_FATFS_COMP_PATH = $(PDK_FATFS_ROOT_PATH)/ti/fs/fatfs
PDK_MMCSD_ROOT_PATH ?= $(pdk_PATH)
export PDK_MMCSD_COMP_PATH = $(PDK_MMCSD_ROOT_PATH)/ti/drv/mmcsd
PDK_PCIE_ROOT_PATH ?= $(pdk_PATH)
export PDK_PCIE_COMP_PATH = $(PDK_PCIE_ROOT_PATH)/ti/drv/pcie
PDK_USB_ROOT_PATH ?= $(pdk_PATH)
export PDK_USB_COMP_PATH = $(PDK_USB_ROOT_PATH)/ti/drv/usb
PDK_SERDES_DIAG_ROOT_PATH ?= $(pdk_PATH)
export PDK_SERDES_DIAG_COMP_PATH = $(PDK_SERDES_DIAG_ROOT_PATH)/ti/diag/serdes_diag
PDK_MCASP_ROOT_PATH ?= $(pdk_PATH)
export PDK_MCASP_COMP_PATH = $(PDK_MCASP_ROOT_PATH)/ti/drv/mcasp
PDK_MCBSP_ROOT_PATH ?= $(pdk_PATH)
export PDK_MCBSP_COMP_PATH = $(PDK_MCBSP_ROOT_PATH)/ti/drv/mcbsp
PDK_PROFILING_ROOT_PATH ?= $(pdk_PATH)
export PDK_PROFILING_COMP_PATH = $(PDK_PROFILING_ROOT_PATH)/ti/utils/profiling
PDK_COPYVECS_ROOT_PATH ?= $(pdk_PATH)
export PDK_COPYVECS_COMP_PATH = $(PDK_COPYVECS_ROOT_PATH)/ti/utils/copyVecs2Atcm
PDK_TRACE_ROOT_PATH ?= $(pdk_PATH)
export PDK_TRACE_COMP_PATH = $(PDK_TRACE_ROOT_PATH)/ti/utils/trace
PDK_VECT_COPY_ROOT_PATH ?= $(pdk_PATH)
export PDK_VECT_COPY_PATH = $(PDK_VECT_COPY_ROOT_PATH)/ti/utils/copyVecs2Atcm
PDK_PRUSS_ROOT_PATH ?= $(pdk_PATH)
export PDK_PRUSS_COMP_PATH = $(PDK_PRUSS_ROOT_PATH)/ti/drv/pruss
PDK_ICSS_EMAC_ROOT_PATH ?= $(pdk_PATH)
export PDK_ICSS_EMAC_COMP_PATH = $(PDK_ICSS_EMAC_ROOT_PATH)/ti/drv/icss_emac
PDK_EMAC_ROOT_PATH ?= $(pdk_PATH)
export PDK_EMAC_COMP_PATH = $(PDK_EMAC_ROOT_PATH)/ti/drv/emac
PDK_UART_ROOT_PATH ?= $(pdk_PATH)
export PDK_UART_COMP_PATH = $(PDK_UART_ROOT_PATH)/ti/drv/uart
PDK_SPI_ROOT_PATH ?= $(pdk_PATH)
export PDK_SPI_COMP_PATH = $(PDK_SPI_ROOT_PATH)/ti/drv/spi
PDK_BOARD_ROOT_PATH ?= $(pdk_PATH)
export PDK_BOARD_COMP_PATH = $(PDK_BOARD_ROOT_PATH)/ti/board
PDK_SBL_ROOT_PATH ?= $(pdk_PATH)
export PDK_SBL_COMP_PATH = $(PDK_SBL_ROOT_PATH)/ti/boot/sbl
PDK_KEYWR_ROOT_PATH ?= $(pdk_PATH)
export PDK_KEYWR_COMP_PATH = $(PDK_KEYWR_ROOT_PATH)/ti/boot/keywriter
PDK_BOARD_IND_ROOT_PATH ?= $(pdk_PATH)
export PDK_BOARD_IND_COMP_PATH = $(PDK_BOARD_IND_ROOT_PATH)/ti/addon/board_ind
PDK_BOARD_UTILS_ROOT_PATH ?= $(pdk_PATH)
export PDK_BOARD_UTILS_COMP_PATH = $(PDK_BOARD_UTILS_ROOT_PATH)/ti/board/utils
PDK_BOARD_DIAG_ROOT_PATH ?= $(pdk_PATH)
export PDK_BOARD_DIAG_COMP_PATH = $(PDK_BOARD_DIAG_ROOT_PATH)/ti/board/diag
PDK_SA_ROOT_PATH ?= $(pdk_PATH)
export PDK_SA_COMP_PATH = $(PDK_SA_ROOT_PATH)/ti/drv/sa

#Below applicable only for K3 devices
PDK_UDMA_ROOT_PATH ?= $(pdk_PATH)
export PDK_UDMA_COMP_PATH = $(PDK_UDMA_ROOT_PATH)/ti/drv/udma
PDK_DSS_ROOT_PATH ?= $(pdk_PATH)
export PDK_DSS_COMP_PATH = $(PDK_DSS_ROOT_PATH)/ti/drv/dss
PDK_SCICLIENT_ROOT_PATH ?= $(pdk_PATH)
export PDK_SCICLIENT_COMP_PATH = $(PDK_SCICLIENT_ROOT_PATH)/ti/drv/sciclient
PDK_FVID2_ROOT_PATH ?= $(pdk_PATH)
export PDK_FVID2_COMP_PATH = $(PDK_FVID2_ROOT_PATH)/ti/drv/fvid2
PDK_CAL_ROOT_PATH ?= $(pdk_PATH)
export PDK_CAL_COMP_PATH = $(PDK_CAL_ROOT_PATH)/ti/drv/cal
PDK_CSIRX_ROOT_PATH ?= $(pdk_PATH)
export PDK_CSIRX_COMP_PATH = $(PDK_CSIRX_ROOT_PATH)/ti/drv/csirx
PDK_IPC_ROOT_PATH ?= $(pdk_PATH)
export PDK_IPC_COMP_PATH = $(pdk_PATH)/ti/drv/ipc
PDK_ENET_ROOT_PATH ?= $(pdk_PATH)
export PDK_ENET_COMP_PATH = $(PDK_ENET_ROOT_PATH)/ti/drv/enet
PDK_LWIP_ROOT_PATH ?= $(pdk_PATH)
export PDK_LWIP_COMP_PATH = $(PDK_LWIP_ROOT_PATH)/ti/transport/lwip
PDK_TSN_ROOT_PATH ?= $(pdk_PATH)
export PDK_TSN_COMP_PATH = $(pdk_PATH)/ti/transport/tsn
PDK_CSITX_ROOT_PATH ?= $(pdk_PATH)
export PDK_CSITX_COMP_PATH = $(PDK_CSITX_ROOT_PATH)/ti/drv/csitx
PDK_SDR_ROOT_PATH ?= $(pdk_PATH)
export PDK_SDR_COMP_PATH = $(pdk_PATH)/ti/diag/sdr
PDK_DIAG_EX_ROOT_PATH ?= $(pdk_PATH)
export PDK_DIAG_EX_COMP_PATH = $(pdk_PATH)/ti/diag/examples
PDK_LPM_ROOT_PATH ?= $(pdk_PATH)
export PDK_LPM_COMP_PATH = $(PDK_LPM_ROOT_PATH)/ti/drv/lpm
SAFETY_CHECKERS_ROOT_PATH ?= $(pdk_PATH)
export SAFETY_CHECKERS_COMP_PATH = $(SAFETY_CHECKERS_ROOT_PATH)/ti/safety_checkers
PDM_UTILS_ROOT_PATH ?= $(pdk_PATH)
export PDM_UTILS_COMP_PATH = $(PDM_UTILS_ROOT_PATH)/ti/drv/pdm_utils
IP_FMA_ROOT_PATH ?= $(pdk_PATH)
export IP_FMA_COMP_PATH = $(IP_FMA_ROOT_PATH)/ti/ip_fma

#Below applicable only for TPR12 devices

PDK_MIBSPI_ROOT_PATH ?= $(pdk_PATH)
export PDK_MIBSPI_COMP_PATH = $(PDK_MIBSPI_ROOT_PATH)/ti/drv/mibspi
PDK_CANFD_ROOT_PATH ?= $(pdk_PATH)
export PDK_CANFD_COMP_PATH = $(PDK_CANFD_ROOT_PATH)/ti/drv/canfd
PDK_MAILBOX_ROOT_PATH ?= $(pdk_PATH)
export PDK_MAILBOX_COMP_PATH = $(PDK_MAILBOX_ROOT_PATH)/ti/drv/mailbox
PDK_CBUFF_ROOT_PATH ?= $(pdk_PATH)
export PDK_CBUFF_COMP_PATH = $(PDK_CBUFF_ROOT_PATH)/ti/drv/cbuff
PDK_ESM_ROOT_PATH ?= $(pdk_PATH)
PDK_ADCBUF_ROOT_PATH ?= $(pdk_PATH)
export PDK_ADCBUF_COMP_PATH = $(PDK_ADCBUF_ROOT_PATH)/ti/drv/adcbuf
export PDK_ESM_COMP_PATH = $(PDK_ESM_ROOT_PATH)/ti/drv/esm
PDK_CAN_ROOT_PATH ?= $(pdk_PATH)
export PDK_CAN_COMP_PATH = $(PDK_CAN_ROOT_PATH)/ti/drv/can
PDK_WATCHDOG_ROOT_PATH ?= $(pdk_PATH)
export PDK_WATCHDOG_COMP_PATH = $(PDK_WATCHDOG_ROOT_PATH)/ti/drv/watchdog

#Below applicable only for K3 DMSC; not applicable for PRSDK
DMSC_ROOT_PATH ?= $(pdk_PATH)
export DMSC_COMP_PATH = $(DMSC_ROOT_PATH)/ti/dmsc

#Below applicable only for TDA devices
PDK_DIAG_ROOT_PATH ?= $(pdk_PATH)
export PDK_DIAG_COMP_PATH = $(PDK_DIAG_ROOT_PATH)/ti/diag
PDK_FIREWALL_L3L4_ROOT_PATH ?= $(pdk_PATH)
export PDK_FIREWALL_L3L4_COMP_PATH = $(PDK_FIREWALL_L3L4_ROOT_PATH)/ti/drv/fw_l3l4

#Below applicable only for TDA devices for backward compatibility;not applicable for PRSDK
PDK_STW_LLD_ROOT_PATH ?= $(pdk_PATH)
export PDK_STW_LLD_COMP_PATH = $(PDK_STW_LLD_ROOT_PATH)/ti/drv/stw_lld
PDK_BSP_LLD_ROOT_PATH ?= $(pdk_PATH)
export PDK_BSP_LLD_COMP_PATH = $(PDK_BSP_LLD_ROOT_PATH)/ti/drv/bsp_lld

PDK_FREERTOS_ROOT_PATH ?= $(pdk_PATH)
export PDK_FREERTOS_COMP_PATH = $(PDK_FREERTOS_ROOT_PATH)/ti/kernel

PDK_SAFERTOS_ROOT_PATH ?= $(pdk_PATH)
export PDK_SAFERTOS_COMP_PATH = $(PDK_SAFERTOS_ROOT_PATH)/ti/kernel/safertos