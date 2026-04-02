
ifeq ($(SOC),$(filter $(SOC), j721e))
  # serdes_cd has compilation errors in host emulation build. This is not required in host emu. Hence disable
  ifneq ($(CORE),$(filter $(CORE), c7x-hostemu))
    PACKAGE_SRCS_COMMON += src/ip/serdes_cd/V0
    PACKAGE_SRCS_COMMON += src/ip/serdes_cd/src_files_serdescd.mk
    PACKAGE_SRCS_COMMON += csl_serdes.h csl_serdes_multilink.h
    PACKAGE_SRCS_COMMON += csl_serdes_ethernet.h csl_serdes_edp.h
    PACKAGE_SRCS_COMMON += csl_serdes_pcie.h csl_serdes_usb.h
    
    SRCS_COMMON += csl_serdes3.c
    SRCS_COMMON += csl_serdes3_edp.c
    SRCS_COMMON += csl_serdes3_ethernet.c
    SRCS_COMMON += csl_serdes3_pcie.c
    SRCS_COMMON += csl_serdes3_usb.c
    SRCS_COMMON += csl_serdes3_multilink.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_QSGMII.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_SGMII.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_SGMII_cmn.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_SGMII_cmn_pll.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_SGMII_ln.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_XAUI.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_XAUI.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_USB.c
    SRCS_COMMON += csl_wiz16m_ct_20b_eDP_cmn_2p7G.c
    SRCS_COMMON += csl_wiz16m_ct_20b_eDP_cmn_5p4G.c
    SRCS_COMMON += csl_wiz16m_ct_20b_eDP_cmn_8p1G.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_cmn_pll_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_cmn_pll1_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_cmn_pll_10G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_cmn_pll1_10G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_cmn_pll_6p25G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_cmn_pll1_6p25G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_20b_eDP_cmn_pll_8p1G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_20b_eDP_cmn_pll_10p8G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_20b_eDP_cmn_pll_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_20b_eDP_cmn_pll1_8p1G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_20b_eDP_cmn_pll1_10p8G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_20b_eDP_cmn_pll1_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_20b_QSGMII_cmn.c
    SRCS_COMMON += csl_wiz16m_ct_refclk19p2MHz_refclk_related.c
    SRCS_COMMON += csl_wiz16m_ct_refclk100MHz_20b_eDP_cmn_pll_8p1G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk100MHz_20b_eDP_cmn_pll_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk100MHz_20b_eDP_cmn_pll1_8p1G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk100MHz_20b_eDP_cmn_pll1_all_vco.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_QSGMII_cmn_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_QSGMII_cmn_pll_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_QSGMII_cmn_pll_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_QSGMII_ln_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_QSGMII_ln_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_SGMII_cmn_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_SGMII_cmn_multilink_pll1_opt3.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_SGMII_cmn_pll_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_SGMII_cmn_pll_multilink_pll1_opt3.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_SGMII_ln_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_SGMII_ln_multilink_pll1_opt3.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_USB_cmn_ext_ssc_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_USB_cmn_pll_ext_ssc_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_USB_ln_ext_ssc_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_32b_PCIe_cmn_pll_ext_ssc_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_32b_PCIe_cmn_pll_int_ssc_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_32b_PCIe_ln_ext_ssc_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_32b_PCIe_ln_int_ssc_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_XAUI_cmn_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_XAUI_cmn_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_XAUI_cmn_pll_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_XAUI_cmn_pll_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_XAUI_ln_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_20b_XAUI_ln_multilink_pll1.c
    SRCS_COMMON += csl_wiz16m_ct_20b_SGMII_cmn.c
    SRCS_COMMON += csl_wiz16m_ct_refclk100MHz_20b_SGMII_QSGMII_cmn_pll_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk100MHz_20b_SGMII_QSGMII_cmn_pll1_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk156p25MHz_20b_XAUI_cmn_pll_6p25G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk156p25MHz_20b_XAUI_cmn_pll_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk156p25MHz_20b_XAUI_cmn_pll1_6p25G_vco.c
    SRCS_COMMON += csl_wiz16m_ct_refclk156p25MHz_20b_XAUI_cmn_pll1_all_vco.c
    SRCS_COMMON += csl_wiz16m_ct_20b_XAUI_cmn.c
    SRCS_COMMON += csl_wiz16m_ct_refclk156p25MHz_refclk_related.c
    SRCS_COMMON += csl_wiz16m_cs_refclk19p2MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_cs_refclk19p2MHz_20b_SGMII.c
    SRCS_COMMON += csl_wiz16m_cs_refclk19p2MHz_20b_USB.c
    SRCS_COMMON += csl_wiz16m_cs_refclk19p2MHz_20b_QSGMII.c
    SRCS_COMMON += csl_wiz16m_cs_refclk156p25MHz_32b_XFI.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_32b_PCIe_cmn_pll_no_ssc_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_32b_PCIe_ln_no_ssc_multilink_pll0.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_QSGMII_cmn_multilink_pll1_opt2.c
    SRCS_COMMON += csl_wiz16m_cs_refclk100MHz_20b_QSGMII_ln_multilink_pll1_opt2.c
    
    SRCDIR += src/ip/serdes_cd/V0
    INCDIR += . src/ip/serdes_cd/V0
  endif
endif

ifeq ($(SOC),$(filter $(SOC), j7200 am64x))
  # serdes_cd has compilation errors in host emulation build. This is not required in host emu. Hence disable
  ifneq ($(CORE),$(filter $(CORE), c7x-hostemu))
    PACKAGE_SRCS_COMMON += src/ip/serdes_cd/V1
    PACKAGE_SRCS_COMMON += src/ip/serdes_cd/src_files_serdescd.mk
    PACKAGE_SRCS_COMMON += csl_serdes.h
    PACKAGE_SRCS_COMMON += csl_serdes_ethernet.h
    PACKAGE_SRCS_COMMON += csl_serdes_pcie.h csl_serdes_usb.h

    SRCS_COMMON += csl_serdes3.c
    SRCS_COMMON += csl_serdes3_pcie.c
    SRCS_COMMON += csl_serdes3_usb.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk20MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk24MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk25MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk26MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk27MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk20MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk24MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk25MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk26MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk27MHz_32b_USB.c
    
    SRCDIR += src/ip/serdes_cd/V1
    INCDIR += . src/ip/serdes_cd/V1

    ifeq ($(SOC),$(filter $(SOC), j7200))
        PACKAGE_SRCS_COMMON += csl_serdes3_multilink.h
        SRCS_COMMON += csl_serdes3_multilink.c
        SRCS_COMMON += csl_serdes3_ethernet.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk125MHz_20b_QSGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk125MHz_20b_SGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk156p25MHz_20b_USXGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk156p25MHz_20b_XAUI.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_20b_QSGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_20b_SGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_20b_XAUI.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk20MHz_20b_QSGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk20MHz_20b_SGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk20MHz_20b_XAUI.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk24MHz_20b_QSGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk24MHz_20b_SGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk24MHz_20b_XAUI.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk25MHz_20b_QSGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk25MHz_20b_SGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk25MHz_20b_XAUI.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk26MHz_20b_QSGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk26MHz_20b_SGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk26MHz_20b_XAUI.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk27MHz_20b_QSGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk27MHz_20b_SGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk27MHz_20b_XAUI.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_PCIe_SGMII_multilink.c
	SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_PCIe_QSGMII_multilink.c
	SRCS_COMMON += csl_wiz16m_ct2_refclk156p25MHz_USXGMII_100MHz_SGMII_multilink.c
	SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_PCIe_156p25MHz_USXGMII_multilink.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_20b_QSGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_20b_SGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_20b_USXGMII.c
        SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_20b_XAUI.c
        SRCDIR += src/ip/serdes_cd/V1/V1_0
    endif

    ifeq ($(SOC),$(filter $(SOC), am64x))
        SRCDIR += src/ip/serdes_cd/V1/V1_1
    endif
  endif
endif

ifeq ($(SOC),$(filter $(SOC), j721s2))
  # serdes_cd has compilation errors in host emulation build. This is not required in host emu. Hence disable
  ifneq ($(CORE),$(filter $(CORE), c7x-hostemu))
    PACKAGE_SRCS_COMMON += src/ip/serdes_cd/V2
    PACKAGE_SRCS_COMMON += src/ip/serdes_cd/src_files_serdescd.mk
    PACKAGE_SRCS_COMMON += csl_serdes.h
    PACKAGE_SRCS_COMMON += csl_serdes_hyperlink.h csl_serdes_edp.h
    PACKAGE_SRCS_COMMON += csl_serdes_pcie.h csl_serdes_usb.h
    PACKAGE_SRCS_COMMON += csl_serdes_multilink.h
    
    SRCS_COMMON += csl_serdes3.c
    SRCS_COMMON += csl_serdes3_edp.c
    SRCS_COMMON += csl_serdes3_hyperlink.c
    SRCS_COMMON += csl_serdes3_pcie.c
    SRCS_COMMON += csl_serdes3_usb.c
    SRCS_COMMON += csl_serdes3_multilink.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_eDP_2p7G.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_eDP_5p4G.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk19p2MHz_eDP_8p1G.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk24MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk24MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk25MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk25MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk26MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk26MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_Hyperlink_5G.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_Hyperlink_10G.c
    SRCS_COMMON += csl_wiz16m_ct2_refclk100MHz_PCIe_USB_multilink.c
    
    SRCDIR += src/ip/serdes_cd/V2
    INCDIR += . src/ip/serdes_cd/V2
  endif
endif
  
ifeq ($(SOC),$(filter $(SOC), j784s4 j742s2))
  # serdes_cd has compilation errors in host emulation build. This is not required in host emu. Hence disable
  ifneq ($(CORE),$(filter $(CORE), c7x-hostemu))
    PACKAGE_SRCS_COMMON += src/ip/serdes_cd/V3
    PACKAGE_SRCS_COMMON += src/ip/serdes_cd/src_files_serdescd.mk
    PACKAGE_SRCS_COMMON += csl_serdes.h
    PACKAGE_SRCS_COMMON += csl_serdes_hyperlink.h csl_serdes_edp.h
    PACKAGE_SRCS_COMMON += csl_serdes_pcie.h csl_serdes_usb.h
    PACKAGE_SRCS_COMMON += csl_serdes_ethernet.h
    PACKAGE_SRCS_COMMON += csl_serdes_multilink.h    
    SRCS_COMMON += csl_serdes3.c
    SRCS_COMMON += csl_serdes3_edp.c
    SRCS_COMMON += csl_serdes3_hyperlink.c
    SRCS_COMMON += csl_serdes3_pcie.c
    SRCS_COMMON += csl_serdes3_usb.c
    SRCS_COMMON += csl_serdes3_ethernet.c
	SRCS_COMMON += csl_serdes3_multilink.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_eDP_1p62G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_eDP_2p7G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_eDP_2p16G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_eDP_2p43G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_eDP_3p24G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_eDP_4p32G.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_eDP_5p4G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_eDP_8p1G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_20b_QSGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_20b_SGMII.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_20b_USXGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_20b_XAUI.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_Hyperlink_5G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk19p2MHz_Hyperlink_10G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_eDP_1p62G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_eDP_2p7G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_eDP_2p16G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_eDP_2p43G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_eDP_3p24G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_eDP_4p32G.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_eDP_5p4G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_eDP_8p1G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_20b_QSGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_20b_SGMII.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_20b_USXGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_20b_XAUI.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_Hyperlink_5G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk24MHz_Hyperlink_10G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_eDP_1p62G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_eDP_2p7G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_eDP_2p16G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_eDP_2p43G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_eDP_3p24G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_eDP_4p32G.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_eDP_5p4G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_eDP_8p1G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_20b_QSGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_20b_SGMII.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_20b_USXGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_20b_XAUI.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_Hyperlink_5G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk25MHz_Hyperlink_10G.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_eDP_1p62G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_eDP_2p7G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_eDP_2p16G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_eDP_2p43G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_eDP_3p24G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_eDP_4p32G.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_eDP_5p4G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_eDP_8p1G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_20b_QSGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_20b_SGMII.c    
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_20b_USXGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_20b_XAUI.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_Hyperlink_5G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk26MHz_Hyperlink_10G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_20b_QSGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_20b_SGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_20b_USXGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_20b_XAUI.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_32b_PCIe.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_32b_USB.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_Hyperlink_5G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_Hyperlink_10G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk125MHz_20b_QSGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk125MHz_20b_SGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk125MHz_Hyperlink_5G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk125MHz_Hyperlink_10G.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk156p25MHz_20b_USXGMII.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk156p25MHz_20b_XAUI.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk100MHz_PCIe_156p25MHz_USXGMII_multilink.c
    SRCS_COMMON += csl_wiz16m_ct3_refclk156p25MHz_USXGMII_100MHz_SGMII_multilink.c	
    
    SRCDIR += src/ip/serdes_cd/V3
    INCDIR += . src/ip/serdes_cd/V3
  endif  
endif    
