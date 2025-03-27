################################################################################
# \file tfm_s.mk
# \version 1.0
#
# \brief
# Trusted Firmware-M (TF-M) secure image make file
#
################################################################################
# \copyright
# Copyright (c) 2023-2025 Cypress Semiconductor Corporation (an Infineon company)
# or an affiliate of Cypress Semiconductor Corporation. All rights reserved.
#
# SPDX-License-Identifier: Apache-2.0
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
################################################################################

ifeq ($(WHICHFILE),true)
$(info Processing $(lastword $(MAKEFILE_LIST)))
endif

# Makefile with common macros
include $(join $(dir $(lastword $(MAKEFILE_LIST))),common.mk)

# delete target files on error
.DELETE_ON_ERROR:

################################################################################
# Settings - allows to configure build behaviour
################################################################################

# List of files that are tracked to trigger configuration
TFM__CONFIG_CHECKSUM_FILE_LIST=

# Use tfm_config.cmake created by Edge Protect Configurator by default
TFM_EXTRA_CONFIG_PATH?=$(call TFM_PATH_MIXED,$(_MTB_TOOLS__MAKEFILE_DIR)/tfm_config/tfm_config.cmake)
ifneq ($(wildcard $(TFM_EXTRA_CONFIG_PATH)),)
TFM_CONFIGURE_OPTIONS+= -DTFM_EXTRA_CONFIG_PATH=$(TFM_EXTRA_CONFIG_PATH)
# Validate checksum of extra configuration file to trigger configuration
TFM__CONFIG_CHECKSUM_FILE_LIST+= $(TFM_EXTRA_CONFIG_PATH)
endif

ifneq ($(TFM_CMAKE_BUILD_TYPE),)
TFM_CONFIGURE_OPTIONS+= -DCMAKE_BUILD_TYPE=$(TFM_CMAKE_BUILD_TYPE)
else ifeq ($(CONFIG),Debug)
# MinSizeRel is used when MTB project is build in Debug mode to be able to
# fit into available memory. With that - ensure that debug symbols are included
TFM_CONFIGURE_OPTIONS+= -DCMAKE_BUILD_TYPE=MinSizeRel -DTFM_DEBUG_SYMBOLS=ON
else ifeq ($(CONFIG),Release)
TFM_CONFIGURE_OPTIONS+= -DCMAKE_BUILD_TYPE=MinSizeRel
else
$(error Please use TFM_CMAKE_BUILD_TYPE (Debug, Release, RelWithDebInfo or MinSizeRel) or CONFIG (Debug, Release) to specify build type!)
endif

# Enable build of secure image
TFM_CONFIGURE_OPTIONS+= -DS_BUILD:BOOL=ON

# Disable build of non-secure image
TFM_CONFIGURE_OPTIONS+= -DNS_BUILD:BOOL=OFF

# Tell cmake to generate compile_commands.json
TFM_CONFIGURE_OPTIONS+= -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON

# Install path
TFM_CONFIGURE_OPTIONS+= -DCMAKE_INSTALL_PREFIX:path=$(call TFM_PATH_MIXED,$(abspath $(TFM_INSTALL_PATH)))

ifndef TFM_BUILD_DIR
TFM_BUILD_DIR:=$(call TFM_PATH_MIXED,$(_MTB_TOOLS__MAKEFILE_DIR)/build/$(TARGET)/$(CONFIG))
endif
TFM_TOOLS_DIR:=$(call TFM_PATH_MIXED,$(TFM_TMP_DIR)/tools)

ifeq ($(TOOLCHAIN),GCC_ARM)
# Setup GCC toolchain
TFM_TOOLCHAIN_FILE?=$(call TFM_PATH_MIXED,$(abspath $(TFM_SRC_DIR)/toolchain_GNUARM.cmake))
ifneq ($(CY_COMPILER_GCC_ARM_DIR),)
export TFM_TOOLCHAIN_PATH?=$(call TFM_PATH_SHELL,$(CY_COMPILER_GCC_ARM_DIR)/bin)
endif
else ifeq ($(TOOLCHAIN),ARM)
# Setup ARM toolchain
TFM_TOOLCHAIN_FILE?=$(call TFM_PATH_MIXED,$(abspath $(TFM_SRC_DIR)/toolchain_ARMCLANG.cmake))
ifneq ($(CY_COMPILER_ARM_DIR),)
export TFM_TOOLCHAIN_PATH?=$(call TFM_PATH_SHELL,$(CY_COMPILER_ARM_DIR)/bin)
endif
else ifeq ($(TOOLCHAIN),IAR)
# Setup IAR toolchain
TFM_TOOLCHAIN_FILE?=$(call TFM_PATH_MIXED,$(abspath $(TFM_SRC_DIR)/toolchain_IARARM.cmake))
ifneq ($(CY_COMPILER_IAR_DIR),)
export TFM_TOOLCHAIN_PATH?=$(call TFM_PATH_SHELL,$(CY_COMPILER_IAR_DIR)/bin)
endif
else
$(error "TF-M supports only GCC, ARMClang and IAR toolchains.")
endif

ifeq ($(TFM_TOOLCHAIN_PATH),)
ifneq ($(CY_CROSSPATH),)
export TFM_TOOLCHAIN_PATH?=$(call TFM_PATH_SHELL,$(CY_CROSSPATH)/bin)
endif
endif

ifneq ($(TFM_TOOLCHAIN_PATH),)
SET_TFM_TOOLCHAIN_PATH=PATH="$(call TFM_PATH_SHELL,$(TFM_TOOLCHAIN_PATH)):$$PATH" &&
endif

TFM_CONFIGURE_OPTIONS+= "-DTFM_TOOLCHAIN_FILE:FILEPATH=$(TFM_TOOLCHAIN_FILE)"


ifndef TFM_COMPILE_COMMANDS_PATH
TFM_COMPILE_COMMANDS_PATH:=$(call TFM_PATH_MIXED,$(_MTB_TOOLS__MAKEFILE_DIR)/build/compile_commands.json)
endif
TFM_COMPILE_COMMANDS_SRC_PATH:=$(call TFM_PATH_MIXED,$(TFM_BUILD_DIR)/compile_commands.json)

################################################################################
# Libraries
################################################################################

#
# Setup library for TF-M
# $(1) - TF-M library CMake configuration variable name
# $(2) - user provided optional variable name which defines path to library
# $(3) - MTB library manager variable name which defines path to library
#
define TFM_SETUP_MTB_LIBRARY
ifneq ($($(2)),)
# Library location provided by user in application Makefile
TFM_CONFIGURE_OPTIONS+= "-D$(1):STRING=$($(2))"
else ifneq ($($(3)),)
# Use library defined by MTB Library Manager
TFM_CONFIGURE_OPTIONS+= "-D$(1):STRING=$(call TFM_PATH_MIXED,$(_MTB_TOOLS__MAKEFILE_DIR)/$($(3)))"
else
# Use library provided by TF-M
endif
endef

# === core-lib ===
# Use IFX_CORE_LIB_PATH to specify custom core-lib library location
$(eval $(call TFM_SETUP_MTB_LIBRARY,IFX_CORE_LIB_PATH,IFX_CORE_LIB_PATH,SEARCH_core-lib))

# === device-db ===
# Use IFX_DEVICE_DB_LIB_PATH to specify custom device-db library location
$(eval $(call TFM_SETUP_MTB_LIBRARY,IFX_DEVICE_DB_LIB_PATH,IFX_DEVICE_DB_LIB_PATH,SEARCH_device-db))

# === MTB PDL CAT1 ===
# Use TFM_LIB_PDL to specify custom MTB PDL CAT1 library location
$(eval $(call TFM_SETUP_MTB_LIBRARY,IFX_PDL_LIB_PATH,IFX_PDL_LIB_PATH,SEARCH_mtb-pdl-cat1))

# === SE RT Services Utils ===
# Use IFX_SE_RT_SERVICES_UTILS_PATH to specify custom SE RT Services Utils library location
$(eval $(call TFM_SETUP_MTB_LIBRARY,IFX_SE_RT_SERVICES_UTILS_PATH,IFX_SE_RT_SERVICES_UTILS_PATH,SEARCH_se-rt-services-utils))

# === mbedtls ===
# Use TFM_LIB_MBEDTLS to specify custom mbedtls library location
$(eval $(call TFM_SETUP_MTB_LIBRARY,MBEDCRYPTO_PATH,TFM_LIB_MBEDTLS,SEARCH_ifx-mbedtls))

# === mbedtls acceleration ===
ifneq ($(SEARCH_cy-mbedtls-acceleration),)
# Use TFM_LIB_MBEDTLS to specify custom mbedtls acceleration library location
$(eval $(call TFM_SETUP_MTB_LIBRARY,IFX_MBEDTLS_ACCELERATION_LIB_PATH,IFX_MBEDTLS_ACCELERATION_LIB_PATH,SEARCH_cy-mbedtls-acceleration))
else
# Disable mbedtls acceleration library
TFM_CONFIGURE_OPTIONS+= -DIFX_MBEDTLS_ACCELERATION_ENABLED:BOOL=OFF
endif

TFM_CONFIGURE_OPTIONS+= $(TFM_CONFIGURE_EXT_OPTIONS)

################################################################################
# Tools location
################################################################################

ifeq ($(TFM_TOOLS_CMAKE),)
# Try to use system cmake instead of installing
TFM__TOOLS_CMAKE=$(shell which cmake 2>/dev/null)
ifneq ($(TFM__TOOLS_CMAKE),)
# cmake minimal version in format used to compare
TFM__TOOLS_CMAKE_MIN_VERSION=000300270007
TFM__TOOLS_CMAKE_VERSION:=$(shell ("$(TFM__TOOLS_CMAKE)" --version 2>/dev/null || echo "0.0.0") | grep -o -E [0-9]+.[0-9]+.[0-9]+ | awk -F. '{ printf("%04d%04d%04d\n", $$1,$$2,$$3); }')
TFM_TOOLS_CMAKE:=$(shell if [ $(TFM__TOOLS_CMAKE_VERSION) -ge $(TFM__TOOLS_CMAKE_MIN_VERSION) ]; then echo "$(TFM__TOOLS_CMAKE)"; fi)
endif
endif

################################################################################
# Local variables
################################################################################

# Is used to unzip archives (e.g. CMake archive)
TFM_PY_UNZIP="exec(\"import sys\nfrom zipfile import PyZipFile\nfor zip_file in sys.argv[1:]:\n    pzf = PyZipFile(zip_file)\n    pzf.extractall()\")"

ifeq ($(TFM_TOOLS_CMAKE),)
# Prepare variables to install CMake
TFM_TOOLS_CMAKE_INSTALL=true

ifneq (,$(findstring CYGWIN,$(TFM_OS)))
# ModusToolbox on Windows (with cygwin)
TFM_TOOLS_CMAKE_WIN_URL?=https://github.com/Kitware/CMake/releases/download/v3.27.7/cmake-3.27.7-windows-i386.zip
TFM_TOOLS_CMAKE_WIN_PATH?=cmake-3.27.7-windows-i386/bin/cmake.exe
TFM_TOOLS_CMAKE_WIN_INSTALL="$(TFM_PYTHON_PATH)" -c $(TFM_PY_UNZIP) "$(call TFM_PATH_MIXED,$<)"

TFM_TOOLS_CMAKE_URL=$(TFM_TOOLS_CMAKE_WIN_URL)
TFM_TOOLS_CMAKE_INSTALL_CMD=$(TFM_TOOLS_CMAKE_WIN_INSTALL)
TFM_TOOLS_CMAKE:=$(call TFM_PATH_MIXED,$(TFM_TOOLS_DIR)/$(TFM_TOOLS_CMAKE_WIN_PATH))

else ifneq (,$(findstring Darwin,$(TFM_OS)))
# ModusToolbox on OS X
TFM_TOOLS_CMAKE_OSX_URL?=https://github.com/Kitware/CMake/releases/download/v3.27.7/cmake-3.27.7-macos-universal.tar.gz
TFM_TOOLS_CMAKE_OSX_PATH?=cmake-3.27.7-macos-universal/CMake.app/Contents/bin/cmake
TFM_TOOLS_CMAKE_OSX_INSTALL=tar -zxf "$<"

TFM_TOOLS_CMAKE_URL=$(TFM_TOOLS_CMAKE_OSX_URL)
TFM_TOOLS_CMAKE_INSTALL_CMD=$(TFM_TOOLS_CMAKE_OSX_INSTALL)
TFM_TOOLS_CMAKE:=$(call TFM_PATH_MIXED,$(TFM_TOOLS_DIR)/$(TFM_TOOLS_CMAKE_OSX_PATH))

else ifneq (,$(findstring Linux,$(TFM_OS)))
# ModusToolbox on Linux
TFM_TOOLS_CMAKE_LINUX_URL?=https://github.com/Kitware/CMake/releases/download/v3.27.7/cmake-3.27.7-linux-x86_64.tar.gz
TFM_TOOLS_CMAKE_LINUX_PATH?=cmake-3.27.7-linux-x86_64/bin/cmake
TFM_TOOLS_CMAKE_LINUX_INSTALL=tar -zxf "$<"

TFM_TOOLS_CMAKE_URL=$(TFM_TOOLS_CMAKE_LINUX_URL)
TFM_TOOLS_CMAKE_INSTALL_CMD=$(TFM_TOOLS_CMAKE_LINUX_INSTALL)
TFM_TOOLS_CMAKE:=$(call TFM_PATH_MIXED,$(TFM_TOOLS_DIR)/$(TFM_TOOLS_CMAKE_LINUX_PATH))

else
$(error "TF-M build system supports only Windows, Linux and OS X.")
endif
TFM_TOOLS_CMAKE_ARCHIVE=$(call TFM_PATH_MIXED,$(TFM_TMP_DIR)/downloads/$(shell basename "$(TFM_TOOLS_CMAKE_URL)"))
endif

TFM_TOOLS_WGET?=$(shell which wget 2>/dev/null)
ifneq ($(TFM_TOOLS_WGET),)
TFM_TOOLS_DOWNLOAD_CMD=$(TFM_TOOLS_WGET) -O "$1" $2
else
TFM_TOOLS_CURL?=$(shell which curl 2>/dev/null)
ifneq ($(TFM_TOOLS_CURL),)
TFM_TOOLS_DOWNLOAD_CMD=$(TFM_TOOLS_CURL) -o "$1" -L $2
else
$(error No wget or curl found, please install one of these tools or provide path to one and repeat make getlibs)
endif
endif


################################################################################
# Targets
################################################################################

# TF-M should be downloaded for secure build to use device configuration makefiles bundled in sources.
# Non-secure build uses configuration installed during secure build.
ifneq ($(filter $(MAKECMDGOALS),tfm-build),)
ifeq ($(TFM_DOWNLOAD_SRC),true)
TFM_DOWNLOAD_SRC=false
ifeq ($(wildcard $(TFM_STAGES_DIR)/src),)
# No src stage file - download
TFM_DOWNLOAD_SRC=true
endif
ifeq ($(wildcard $(TFM_SRC_DIR)),)
# No src directory - download
TFM_DOWNLOAD_SRC=true
endif
ifeq ($(TFM_DOWNLOAD_SRC),true)
# Download of TF-M sources
$(info ==============================================================================)
$(info Downloading TF-M ...)
$(info ==============================================================================)
$(info TFM_SRC_DIR $(TFM_SRC_DIR))
$(info TFM_GIT_URL $(TFM_GIT_URL))
$(info TFM_GIT_REF $(TFM_GIT_REF))
$(info TFM_STAGES_DIR $(TFM_STAGES_DIR))
_ := $(shell $(MAKE) TFM_SRC_DIR=$(TFM_SRC_DIR) TFM_STAGES_DIR=$(TFM_STAGES_DIR) TFM_GIT_URL=$(TFM_GIT_URL) TFM_GIT_REF=$(TFM_GIT_REF) -f $(join $(dir $(lastword $(MAKEFILE_LIST))),download.mk))
ifeq ($(wildcard $(TFM_STAGES_DIR)/src),)
$(error Download of TF-M sources failed)
endif # ($(wildcard $(TFM_STAGES_DIR)/src),)
endif # ($(TFM_DOWNLOAD_SRC),true)
endif # ($(TFM_DOWNLOAD_SRC),true)
endif # ($(filter $(MAKECMDGOALS),tfm-build),)

# Create dirs
$(TFM_TMP_DIR):
	@mkdir -pv $(TFM_TMP_DIR)

$(TFM_TMP_DIR)/downloads:
	mkdir -p $(TFM_TMP_DIR)/downloads

$(TFM_SRC_DIR):
	@mkdir -pv $(TFM_SRC_DIR)

$(TFM_TOOLS_DIR):
	@mkdir -pv $(TFM_TOOLS_DIR)

$(TFM_STAGES_DIR):
	@mkdir -pv $(TFM_STAGES_DIR)

ifneq ($(TFM_BUILD_DIR),)
$(TFM_BUILD_DIR): |tfm-create-build-dir

.PHONY: tfm-create-build-dir
tfm-create-build-dir:
	@mkdir -pv $(TFM_BUILD_DIR)

# Clean
.PHONY: tfm-clean
TFM_CLEAN_LIST+= $(TFM_INSTALL_PATH)/bin $(TFM_INSTALL_PATH)/cmake $(TFM_INSTALL_PATH)/config $(TFM_INSTALL_PATH)/interface $(TFM_INSTALL_PATH)/platform $(TFM_INSTALL_PATH)/CMakeLists.txt
tfm-clean:
	@echo "=============================================================================="
	@echo "Clean TF-M ..."
	@echo "=============================================================================="
	rm -rf $(TFM_CLEAN_LIST)

clean_proj: tfm-clean

.PHONY: tfm-configure-clean
tfm-configure-clean:
	$(MAKE) clean_proj

# Configure TF-M
.PHONY: tfm-configure

ifneq ($(TFM__CONFIG_CHECKSUM_FILE_LIST),)
# Validate content of additional configuration files
TFM__CONFIG_CHECKSUM_VALIDATION_STATUS=$(shell md5sum --check --quiet --status $(TFM_BUILD_DIR)/.tfm-configure-checksum 2>/dev/null && echo "" || echo ":::FAILED:::")
TFM__CONFIG_CHECKSUM_VALIDATION=md5sum $(TFM__CONFIG_CHECKSUM_FILE_LIST) >$(TFM_BUILD_DIR)/.tfm-configure-checksum &&
endif

tfm-configure $(TFM_BUILD_DIR)/.tfm-configure: $(TFM_STAGES_DIR)/tools-pyreq | $(call TFM_WRAP_SPACE,$(TFM_TOOLS_CMAKE)) $(TFM_BUILD_DIR)
	@echo "=============================================================================="
	@echo "Configuring TF-M ..."
	@echo "=============================================================================="
	@mkdir -pv $(TFM_BUILD_DIR)/.tmp
	. $(TFM_TOOLS_ACTIVATE_PYENV) && \
	rm -rf $(TFM_CLEAN_LIST) && \
	$(SET_TFM_TOOLCHAIN_PATH) \
	$(TFM__CONFIG_CHECKSUM_VALIDATION) \
	echo "LAST_TFM_CONFIGURE_OPTIONS=$(call TFM_WRAP_DOUBLE_QUOTE,$(TFM_CONFIGURE_OPTIONS))" >"$(TFM_BUILD_DIR)/.tfm-configure.mk" && \
	"$(TFM_TOOLS_CMAKE)" $(TFM_CONFIGURE_OPTIONS) -G "Unix Makefiles" -S "$(call TFM_PATH_MIXED,${TFM_SRC_DIR})" -B $(TFM_BUILD_DIR) && \
	install $(TFM_COMPILE_COMMANDS_SRC_PATH) $(TFM_COMPILE_COMMANDS_PATH) && \
	touch "$(TFM_BUILD_DIR)/.tfm-configure"

tfm-configure: tfm-configure-clean

# Build TF-M
.PHONY: tfm-make
# Validate configuration flags
-include $(TFM_BUILD_DIR)/.tfm-configure.mk

ifneq ($(LAST_TFM_CONFIGURE_OPTIONS),$(TFM_CONFIGURE_OPTIONS)$(TFM__CONFIG_CHECKSUM_VALIDATION_STATUS))
# There are changes in configuration flags, force configuration with new settings
tfm-make $(TFM_BUILD_DIR)/.tfm-make: tfm-configure
else
# There are no changes in configuration flags, reconfigure only if $(TFM_BUILD_DIR)/.tfm-configure is missed
tfm-make $(TFM_BUILD_DIR)/.tfm-make: $(TFM_BUILD_DIR)/.tfm-configure | $(TFM_BUILD_DIR)
endif
	@echo "=============================================================================="
	@echo "Building TF-M SPE ..."
	@echo "=============================================================================="
	. $(TFM_TOOLS_ACTIVATE_PYENV) && \
	cd $(TFM_BUILD_DIR) && \
	$(SET_TFM_TOOLCHAIN_PATH) \
	$(MAKE) install && \
	install $(TFM_COMPILE_COMMANDS_SRC_PATH) $(TFM_COMPILE_COMMANDS_PATH) && \
	mkdir -p $(MTB_TOOLS__OUTPUT_CONFIG_DIR) && \
	install $(TFM_BUILD_DIR)/bin/tfm_s.elf $(MTB_TOOLS__OUTPUT_CONFIG_DIR)/$(APPNAME).elf && \
	install $(TFM_BUILD_DIR)/bin/tfm_s.hex $(MTB_TOOLS__OUTPUT_CONFIG_DIR)/$(APPNAME).hex && \
	touch "$(TFM_BUILD_DIR)/.tfm-make"

.PHONY: tfm-project-hex
tfm-project-hex: tfm-make
	mkdir -p $(TFM_BUILD_PROJECT_HEX_DIR) && \
	install $(MTB_TOOLS__OUTPUT_CONFIG_DIR)/$(APPNAME).hex $(TFM_BUILD_PROJECT_HEX_DIR)/$(APPNAME).hex && \
	echo $(call TFM_PATH_MIXED,$(TFM_BUILD_DIR)/bin/tfm_s.hex) >$(TFM_BUILD_PROJECT_HEX_DIR)/$(APPNAME).hex.d;

.PHONY: tfm-build
tfm-build: tfm-make tfm-project-hex
	@echo "=============================================================================="
	@echo "Building TF-M SPE ... Done"
	@echo "=============================================================================="

ifneq ($(POSTBUILD),)
.PHONY: tfm-postbuild
tfm-project-hex: tfm-postbuild
tfm-postbuild: tfm-make
	$(POSTBUILD)
endif

endif   # ifneq ($(TFM_BUILD_DIR),)

################################################################################
# Tools
################################################################################

# === Prepare CMake ===
ifneq ($(TFM_TOOLS_CMAKE_INSTALL),)
# Install CMake
$(TFM_TOOLS_CMAKE_ARCHIVE): | $(TFM_TMP_DIR)/downloads
	@echo "=============================================================================="
	@echo "Downloading CMake ..."
	@echo "=============================================================================="
	$(call TFM_TOOLS_DOWNLOAD_CMD,$(TFM_TOOLS_CMAKE_ARCHIVE),$(TFM_TOOLS_CMAKE_URL))

$(TFM_TOOLS_CMAKE): $(TFM_TOOLS_CMAKE_ARCHIVE) | $(TFM_TOOLS_DIR) $(TFM_STAGES_DIR)
	@echo "=============================================================================="
	@echo "Unpacking CMake ..."
	@echo "=============================================================================="
	cd $(TFM_TOOLS_DIR) && \
	$(TFM_TOOLS_CMAKE_INSTALL_CMD) && \
	touch $(TFM_TOOLS_CMAKE) \
	|| (rm -f $(TFM_TOOLS_CMAKE_ARCHIVE); false)

.PHONY: tfm-tools-cmake
tfm-tools-cmake: $(TFM_TOOLS_CMAKE)
else
# Use system provided CMake
.PHONY: tfm-tools-cmake
tfm-tools-cmake:
	@echo "Use cmake : $(TFM_TOOLS_CMAKE)"
endif


# === Python requirements within pyenv ===
ifneq (,$(findstring CYGWIN,$(TFM_OS)))
TFM_TOOLS_ACTIVATE_PYENV=$(TFM_TOOLS_DIR)/tfm-pyenv/Scripts/activate
# Convert Windows line endings to Unix
TFM_PREPARE_PYENV=dos2unix $(TFM_TOOLS_ACTIVATE_PYENV) &&
else
TFM_TOOLS_ACTIVATE_PYENV=$(TFM_TOOLS_DIR)/tfm-pyenv/bin/activate
endif

$(TFM_STAGES_DIR)/tools-pyenv: | $(TFM_TOOLS_DIR) $(TFM_STAGES_DIR)
	@echo "=============================================================================="
	@echo "Creating pyenv for TF-M prerequisites"
	@echo "=============================================================================="
	rm -rf "$(call TFM_PATH_MIXED,$(TFM_TOOLS_DIR)/tfm-pyenv)" && \
	"$(TFM_PYTHON_PATH)" -m venv "$(call TFM_PATH_MIXED,$(TFM_TOOLS_DIR)/tfm-pyenv)" && \
	$(TFM_PREPARE_PYENV) \
	touch "$@"

$(TFM_STAGES_DIR)/tools-pyreq: $(TFM_STAGES_DIR)/tools-pyenv | $(TFM_TOOLS_DIR) $(TFM_STAGES_DIR)
	@echo "=============================================================================="
	@echo "Installing TF-M Python prerequisites"
	@echo "=============================================================================="
# Skip installation of requirements.txt
# There are lot of tools which are used for documentation only
#	pip install -r "$(call TFM_PATH_MIXED,$(TFM_SRC_DIR)/tools/requirements.txt)"
	. $(TFM_TOOLS_ACTIVATE_PYENV) && \
	pip install --disable-pip-version-check Jinja2 PyYAML cbor\>=1.0.0 && \
	touch "$@"

.PHONY: tfm-tools-pyenv
tfm-tools-pyenv: $(TFM_STAGES_DIR)/tools-pyenv $(TFM_STAGES_DIR)/tools-pyreq

# === Tools ===
.PHONY: tfm-tools
tfm-tools: tfm-tools-cmake tfm-tools-pyenv
	@echo "TF-M Tools installed"


################################################################################
# getlibs
################################################################################

tfm-download: tfm-tools
