################################################################################
# \file download.mk
# \version 1.0
#
# \brief
# Download Trusted Firmware-M (TF-M) sources
#
################################################################################
# \copyright
# Copyright (c) 2024 Cypress Semiconductor Corporation (an Infineon company)
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

all: $(TFM_SRC_DIR) $(TFM_STAGES_DIR)/src

$(TFM_STAGES_DIR):
	@mkdir -pv $(TFM_STAGES_DIR)

$(TFM_SRC_DIR) $(TFM_STAGES_DIR)/src: | $(TFM_STAGES_DIR)
	@echo "=============================================================================="
	@echo "Checking out TF-M ..."
	@echo "=============================================================================="
	mkdir -pv $(TFM_SRC_DIR) && \
	cd $(TFM_SRC_DIR) && \
	git -c init.defaultBranch=master init . && \
	git config --local core.autocrlf false && \
	git config --local core.eol lf && \
	git remote rm origin 2>/dev/null; \
	git remote add origin $(TFM_GIT_URL) && \
	git fetch origin $(TFM_GIT_REF) && \
	git reset --hard FETCH_HEAD && \
	touch "$(TFM_STAGES_DIR)/src"
