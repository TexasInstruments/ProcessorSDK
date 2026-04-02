# Power Analysis Tools

## Introduction

The tools empower users to accurately configure and monitor Phase-lock loop (PLL) and Power-and-sleep-controller (PSC) based on specific use-cases. By selecting which IP and Module to enable or disable and configuring the frequency at which they operate, users can significantly impact power consumption.

![dm_power_config_tool](dm_power_config_tool/internal/power_tools.drawio.svg)

## JTAG Power Analysis Tool

Refer [userguide](jtag_power_analysis_tool/doc/user_guide.md)

## DM Power Configuration Tool

Refer [userguide](dm_power_config_tool/userguide.md)

## SoC Compatibility Matrix

### Tool Support by SoC

| Tool                     | J784S4 | J722S | J721S2 | J721E | J7200 |
| ------------------------ | :----: | :---: | :----: | :---: | :---: |
| DM Power Config Tool     |   ✓    |   ✓   |   ✓    |   ✗   |   ✗   |
| JTAG Power Analysis Tool |   ✓    |   ✓   |   ✓    |   ✓   |   ✓   |

### JTAG Power Analysis Tool Features by SoC

| Feature    | J784S4 | J722S | J721S2 | J721E | J7200 |
| ---------- | :----: | :---: | :----: | :---: | :---: |
| PET        |   ✓    |   ✓   |   ✓    |   ✗   |   ✗   |
| PLL        |   ✓    |   ✓   |   ✓    |   ✓   |   ✓   |
| PSC        |   ✓    |   ✓   |   ✓    |   ✓   |   ✓   |
| CLOCK-TREE |   ✓    |   ✓   |   ✓    |   ✓   |   ✓   |

✓ = Supported
✗ = Not Supported
