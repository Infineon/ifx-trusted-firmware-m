# Infineon Customized Trusted Firmware-M for Secure Project

## Overview
Trusted Firmware-M (TF-M) implements the Secure Processing Environment (SPE)
for Arm Cortex-M based platforms. This aligns the reference implementation of the platform security architecture
with the PSA Certified guidelines. Thus, TF-M allows relevant chips and devices to become PSA Certified.

This library contains Trusted Firmware-M (TF-M) for secure projects. Use the ifx-trusted-firmware-m-ns
library to integrate TF-M services into a non-secure project (NSPE).

## Licensing
This software component is licensed under a mixture of the Apache License,
version 2 and the 3-Clause BSD License. See separate files to
determine which license applies. Note the licensing of the
following modules:
* [t_cose](https://github.com/Infineon/trusted-firmware-m/blob/master/src/lib/ext/t_cose/LICENSE)
* [qcbor](https://github.com/Infineon/trusted-firmware-m/blob/master/src/ext/qcbor/README.md)

## Usage of TF-M in the ModusToolbox™ tools package
### To build a TF-M secure image TF-M in a ModusToolbox™ environment:
1. Add the ifx-trusted-firmware-m library to the secure project.
2. Modify the memory configuration. See [Memory configuration](#memory-configuration).
3. Build a secure project.

> **_NOTE:_**  The main.c, linker script, startup, and other files provided by
a ModusToolbox™ secure project will not be included in the TF-M build,
files from the TF-M sources (ifx-trusted-firmware-m library) will be used instead.

### Usage of TF-M in non-secure applications
To use TF-M services in non-secure applications:
1. Add the ifx-trusted-firmware-m-ns library to the non-secure project. For more details, see the README.md of
   the ifx-trusted-firmware-m-ns library.

The code used to bind a non-secure project with TF-M is generated during the TF-M secure project build
in folder `TFM_INSTALL_PATH`.

## TF-M configuration
There are two types of configuration options:
1. ModusToolbox™ Makefile configuration options - set and used in
   ModusToolbox™ Makefile.
2. CMake configuration options - set in ModusToolbox™ Makefile using
   the `TFM_CONFIGURE_EXT_OPTIONS` variable and passed to the TF-M CMake
   configuration system.

### ModusToolbox™ Makefile configuration options
Optional variables to configure in TF-M Makefile:
1. `TFM_GIT_URL` - The optional location of the Git repo with TF-M sources. Use it if you created your own fork
    of the TF-M sources. The sources are cloned to folder _.tmp_ of the ifx-trusted-firmware-m library if
    `TFM_SRC_DIR` is not specified.
    * `TFM_GIT_REF` - The reference to the commit/branch/tag in the Git repo specified by
      `TFM_GIT_URL`.
2. `TFM_SRC_DIR` - The path to TF-M sources. The sources provided by the ifx-trusted-firmware-m library are used
   by default if neither `TFM_SRC_DIR` nor `TFM_GIT_URL` is provided.
3. The following variables can be used to specify custom locations for corresponding libraries:
    * `IFX_CORE_LIB_PATH`
    * `IFX_DEVICE_DB_LIB_PATH`
    * `IFX_MBEDTLS_ACCELERATION_LIB_PATH`
    * `IFX_PDL_LIB_PATH`
    * `MBEDCRYPTO_PATH`
4. `TFM_BUILD_DIR` - The location of the build directory.
5. `TFM_COMPILE_COMMANDS_PATH` - The optional location of `compile_commands.json`, which will
   be updated after CMake configuration.
6. `TFM_DEBUG_SYMBOLS` - Enables the debug info. It does not affect optimization flags.
7. `TFM_CMAKE_BUILD_TYPE` - The optional parameter to specify the CMake build type.
   See `CMAKE_BUILD_TYPE` in CMake documentation.
8. `CONFIG` - TF-M Makefile uses this variable to define `CMAKE_BUILD_TYPE`
   if `TFM_CMAKE_BUILD_TYPE` is not specified. Valid arguments:
    * `Debug` - The MinSizeRel configuration (`CMAKE_BUILD_TYPE=MinSizeRel`) with the debug info (TFM_DEBUG_SYMBOLS=ON)
       is used.
    * `Release` - The MinSizeRel configuration without the debug info (`CMAKE_BUILD_TYPE=MinSizeRel`) is used.
9. `TFM_TOOLS_CMAKE` - The path to CMake executable.
    * If `TFM_TOOLS_CMAKE` is not specified, `TFM_TOOLS_CMAKE_URL` can be
      specified to override the default URL for downloading CMake. In this case,
      CMake will be downloaded during the build process.
10. `TFM_INSTALL_PATH` - The optional path where the non-secure interface is installed. This is the parent folder.
    of the TF-M secure project by default.
11. `TFM_CONFIGURE_EXT_OPTIONS` - The additional options, which will be appended to
    setup CMake configuration. See below for details.

### CMake configuration options
`TFM_CONFIGURE_EXT_OPTIONS` make variable can be used to specify additional CMake options.
CMake configuration options:
1. `TFM_PROFILE` - TF-M profile.
2. `TFM_ISOLATION_LEVEL` - TF-M isolation level.
3. `IFX_MBEDTLS_ACCELERATION_ENABLED` - Whether to use the H/W crypto acceleration for Crypto partition
   provided by the cy-mbedtls-acceleration library. Ensure that the cy-mbedtls-acceleration library is
   added to the secure project to use it.
4. `IFX_MBEDTLS_CONFIG_PATH` - Path to optional ifx-mbedtls configuration header. It's included at
   the end of platform configuration header defined by `TFM_MBEDCRYPTO_PLATFORM_EXTRA_CONFIG_PATH`.
5. `IFX_PDL_SECURE_SERVICES` - Whether to provide a mechanism for non-secure
   code to control certain secure functions such as clocking, debug, and Deep
   Sleep.
6. `IFX_PROJECT_CONFIG_PATH` - Path to optional project configuration header included at the begining
   of config_tfm_target.h header. You can use it to override settings provided by profile
   configuration header.
7. Other CMake configuration options - For more details, refer to TF-M documentation.

### TF-M logging
To enable the TF-M logging:
1. Configure SCBx to UART mode in the Device Configurator and name it `IFX_TFM_SPM_UART`.
2. Set `IFX_UART_ENABLED` CMake option to ON.
3. `TFM_SPM_LOG_LEVEL` CMake option defines SPM (Secure Partition Manager) log level:
    * `TFM_SPM_LOG_LEVEL_DEBUG` - All log APIs output.
    * `TFM_SPM_LOG_LEVEL_INFO` - All log APIs output except SPMLOG_DBG and SPMLOG_DBGMSGVAL.
    * `TFM_SPM_LOG_LEVEL_ERROR` - Only SPMLOG_ERRMSG and SPMLOG_ERRMSGVAL APIs output.
    * `TFM_SPM_LOG_LEVEL_SILENCE` - All log APIs are suppressed.
4. `TFM_PARTITION_LOG_LEVEL` CMake option defines the secure partitions log level:
    * `TFM_PARTITION_LOG_LEVEL_DEBUG` - All log APIs output.
    * `TFM_PARTITION_LOG_LEVEL_INFO` - All log APIs output except LOG_DBGFMT.
    * `TFM_PARTITION_LOG_LEVEL_ERROR` - Only LOG_ERRFMT APIs output.
    * `TFM_PARTITION_LOG_LEVEL_SILENCE` - All log APIs are suppressed.

Only the SPM domain has access to the `IFX_TFM_SPM_UART` SCBx peripheral if the TF-M logging is enabled.
You can use the `ifx_platform_log_msg` function to write to `IFX_TFM_SPM_UART` from a secure partition or
a non-secure project.

### Further instructions
For the general TF-M documentation, refer to
[TF-M user guide](https://tf-m-user-guide.trustedfirmware.org/index.html).

## PSC3 TF-M specifics
### Memory configuration
TF-M uses the memory regions described in the table below. The rightmost column details the supported
types of memory and the recommended size for each block. Depending on the configuration, some of these
regions may not be required. See the notes below for details.
| Description                       | Region name            | PCS3               |
| --------------------------------- | ---------------------- | ------------------ |
| TF-M data section                 | TFM_DATA               | SRAM 48kB          |
| CM33 NSPE data section            | CM33_NS_DATA           | SRAM 16kB          |
| TF-M code region                  | TFM_IMAGE_EXECUTE      | FLASH 186kB        |
| Internal Trusted Storage data     | TFM_ITS                | FLASH 4kB          |
| Rollback counters                 | FLASH_NV_COUNTERS      | FLASH 2kB          |
| CM33 NSPE code region             | CM33_NS_IMAGE_EXECUTE  | FLASH 32kB         |
| CM33 NSPE secondary slot          | CM33_NS_SECONDARY_SLOT | FLASH 32kB         |

1.  The Flashboot starts the TF-M application.
2.  Additional data is used by the Flashboot, which is the image header.
    The size of execute regions includes a header. See the Flashboot documentation for more details.
3.  TFM_IMAGE_EXECUTE is used to specify the location of TF-M code.
4.  CM33_NS_IMAGE_EXECUTE is used to specify the location of the CM33 NSPE code.

> **_NOTE:_** The CM33_NS_SECONDARY_SLOT region is not used by TF-M, but used by Flashboot to update the NSPE image.

### NSPE for PSC3
The PSC3 platform supports NSPE on the Cortex-M33 core, it is always enabled.

For the correct use of NSPE, the client must initialize the NS interface by calling the `tfm_ns_interface_init`
function in the NSPE CM33 code **before** the first call to the PSA function.

Some of the peripheral registers (e.g. clock, power, protection configuration
registers, etc.) on PSC3 are protected as Secure access only by TFM, so the
Non-Secure code that access these registers (SystemInit, cybsp_init, etc.) must
not be executed (otherwise, device reset will be done because of the Secure
violation).

### PSC3 TF-M logging
Use SCB3 for `IFX_TFM_SPM_UART` because currently SCB3 is configured by default in the TF-M protection
settings.

### PSC3 device provisioning
The PSC3 platform requires specific setting to be provisioned during oem provisioning:
1. Set the boot configuration ID to BOOT_THREE_SLOTS.
```json
"boot": {
    "boot_cfg_id": {
        "description": "A behavior for BOOT_APP_LAYOUT (BOOT_SIMPLE_APP applicable to NORMAL_PROVISIONED only)",
        "applicable_conf": "BOOT_SIMPLE_APP, BOOT_ONE_SLOT, BOOT_TWO_SLOTS, BOOT_THREE_SLOTS, BOOT_IDLE",
        "value": "BOOT_THREE_SLOTS"
    }
}
```
2. Enable the L2 slot authentication.
```json
"boot_auth_l2_enable": {
    "description": "Indicates whether to enable L2 slot authentication by Boot FW",
    "value": true
}
```
3. Add the TFM_IMAGE_EXECUTE, CM33_NS_IMAGE_EXECUTE and CM33_NS_SECONDARY_SLOT memory regions to
the boot application layout option.
```json
"boot_app_layout": {
    "description": "The memory layout for the applications defined by BOOT_CFG_ID. 0x32000000 - 0x33FFFFFF for secure addresses; 0x22000000 - 0x23FFFFFF for non-secure addresses",
    "value": [
        {
            "address": "0x32000000",
            "size": "0x2E800" # 186kB
        },
        {
            "address": "0x22030000",
            "size": "0x8000" # 32kB
        },
        {
            "address": "0x22038000",
            "size": "0x8000" # 32kB
        }
    ]
}
```
> **_NOTE:_** The value of boot_app_layout represents the executable regions from the memory configuration,
in our case, these are the TFM_IMAGE_EXECUTE, CM33_NS_IMAGE_EXECUTE, and CM33_NS_SECONDARY_SLOT regions.

4. MPC configuration, thus TF-M is not allowed to change it.
```json
"mpc": {
    "mpc_struct_ram": {
        "description": "RAM MPC structures. The total number of RAM and Flash MPC structures must be less than 32",
        "value": [
            {
                "description": "TFM_DATA region. PC2 WR secure. Offset and size in KB. The values must be multiple of 2 (e.g. 0 KB, 2 KB, 4 KB, 8 KB, ...)",
                "offset": "0 KB",
                "size": "48 KB",
                "attr": {
                    "description": "The MPC attributes for this memory region",
                    "comment_1": "WRN allow write, read, and NS only access",
                    "comment_2": "-RN allow read, NS-only access",
                    "comment_3": "--- and --N, no access",
                    "comment_4": "WR- read and write access, but secure only",
                    "value": {
                        "pc0": "WR-",
                        "pc1": "WR-",
                        "pc2": "WR-",
                        "pc3": "WR-",
                        "pc4": "WR-",
                        "pc5": "WR-",
                        "pc6": "WR-",
                        "pc7": "WR-"
                    }
                }
            },
            {
                "description": "CM33_NS_DATA region. PC2 WR not secure. Offset and size in KB. The values must be multiple of 2 (e.g. 0 KB, 2 KB, 4 KB, 8 KB, ...)",
                "offset": "48 KB",
                "size": "16 KB",
                "attr": {
                    "description": "The MPC attributes for this memory region",
                    "comment_1": "WRN allow write, read, and NS only access",
                    "comment_2": "-RN allow read, NS-only access",
                    "comment_3": "--- and --N, no access",
                    "comment_4": "WR- read and write access, but secure only",
                    "value": {
                        "pc0": "WR-",
                        "pc1": "WR-",
                        "pc2": "WRN",
                        "pc3": "WR-",
                        "pc4": "WR-",
                        "pc5": "WR-",
                        "pc6": "WR-",
                        "pc7": "WR-"
                    }
                }
            }
        ]
    },
    "mpc_struct_flash": {
        "description": "Flash MPC structures. The total number of RAM and Flash MPC structures must be less than 32",
        "value": [
            {
                "description": "TFM_IMAGE_EXECUTE, TFM_ITS and FLASH_NV_COUNTERS regions. PC2 WR secure. Offset and size in KB. The values must be multiple of 2 (e.g. 0 KB, 2 KB, 4 KB, 8 KB, ...)",
                "offset": "0 KB",
                "size": "192 KB",
                "attr": {
                    "description": "The MPC attributes for this memory region",
                    "comment_1": "WRN allow read, write, and NS only access",
                    "comment_2": "-RN allow read, NS-only access",
                    "comment_3": "--- and --N, no access",
                    "comment_4": "WR- read and write access, but secure only",
                    "value": {
                        "pc0": "WR-",
                        "pc1": "WR-",
                        "pc2": "WR-",
                        "pc3": "WR-",
                        "pc4": "WR-",
                        "pc5": "WR-",
                        "pc6": "WR-",
                        "pc7": "WR-"
                    }
                }
            },
            {
                "description": "CM33_NS_IMAGE_EXECUTE and CM33_NS_SECONDARY_SLOT regions. PC2 WR is not secure. The offset and size are in KB. The values must be multiple of 2 (e.g. 0 KB, 2 KB, 4 KB, 8 KB, ...)",
                "offset": "192 KB",
                "size": "64 KB",
                "attr": {
                    "description": "The MPC attributes for this memory region",
                    "comment_1": "WRN allow read, write, and NS only access",
                    "comment_2": "-RN allow read, NS-only access",
                    "comment_3": "--- and --N, no access",
                    "comment_4": "WR- read and write access, but secure only",
                    "value": {
                        "pc0": "WR-",
                        "pc1": "WR-",
                        "pc2": "WRN",
                        "pc3": "WR-",
                        "pc4": "WR-",
                        "pc5": "WR-",
                        "pc6": "WR-",
                        "pc7": "WR-"
                    }
                }
            }
        ]
    }
}
```
> **_NOTE:_** The offsets and sizes in the MPC configuration are filled in according to the PCS3 memory configuration,
and must be recalculated if the PCS3 memory configuration is changed.

### Default configuration
TF-M is configured by default with the following settings:
| Description                        | Configuration option                   | PSC3           |
| ---------------------------------- | -------------------------------------- | -------------- |
| Profile                            | TFM_PROFILE                            | profile_medium |
| Crypto partition                   | TFM_PARTITION_CRYPTO                   | ON             |
| Use Crypto accelerator             | IFX_MBEDTLS_ACCELERATION_ENABLED       | ON             |
| Firmware Update partition          | TFM_PARTITION_FIRMWARE_UPDATE          | OFF            |
| Initial Attestation partition      | TFM_PARTITION_INITIAL_ATTESTATION      | ON             |
| Internal Trusted Storage partition | TFM_PARTITION_INTERNAL_TRUSTED_STORAGE | ON             |
| Platform partition                 | TFM_PARTITION_PLATFORM                 | ON             |
| Protected Storage partition        | TFM_PARTITION_PROTECTED_STORAGE        | OFF            |
| Isolation Level                    | TFM_ISOLATION_LEVEL                    | 2              |
| Fault Injection Hardening          | TFM_FIH_PROFILE                        | OFF            |

> **_NOTE:_** The Firmware Update service is not supported for the PSC3 device.

## Static code checkers

TF-M code is checked with MISRA-C and CERT-C rules checkers.

Following rules MISRA-C are ignored during the checks:
1. PSC3: 1.2; 1.5; 2.1; 2.3; 2.4; 2.5; 2.7; 3.1; 5.1; 5.6; 5.7; 5.8; 8.3; 8.4;
   8.5; 8.6; 8.7; 8.8; 8.9; 8.13; 8.15; 10.3; 10.5; 10.8; 11.1; 11.4; 11.5;
   11.9; 12.1; 14.4; 15.5; 16.1; 16.3; 16.6; 20.9; 21.1; 21.2; 21.15.

Following rules CERT-C are ignored during the checks:
1. PSC3: EXP32-C; EXP34-C; INT30-C; INT31-C; INT33-C; STR30-C.

Other MISRA-C and CERT-C rules may be suppressed for a specific lines - see TF-M
source code for more details.

## More information
Use the following links for more information:
* [Cypress Semiconductor Corporation (an Infineon company)](https://www.infineon.com)
* [Cypress Semiconductor Corporation (an Infineon company) GitHub](https://github.com/Infineon)
* [Trusted Firmware website](https://www.trustedfirmware.org)
* [TF-M project](https://www.trustedfirmware.org/projects/tf-m)
* [PSA API](https://github.com/ARM-software/psa-arch-tests/tree/master/api-specs)
* [ModusToolbox Software Environment, Quick Start Guide, Documentation, and Videos](https://www.infineon.com/cms/en/design-support/tools/sdk/modustoolbox-software)

---
© 2023-2025, Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation.
