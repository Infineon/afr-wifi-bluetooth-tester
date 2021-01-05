################################################################################
# \file afr.mk
# \version 1.0
#
# \brief
# Configuration file for adding/removing aFR files. Modify this file
# to suit the application needs.
#
################################################################################
# \copyright
# Copyright 2019 Cypress Semiconductor Corporation
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

# LPA Changes

CY_APP_LIB_DIR=$(CY_AFR_ROOT)/vendors/cypress/MTB/libraries/command-console
CY_TEST_LIB_DIR=$(CY_AFR_ROOT)/vendors/cypress/MTB/libraries
CY_AFR_APP_SRC=
CY_AFR_APP_CONFIG=$(CY_APP_LIB_DIR)/TESTS/unit-tests/afr/config_files

SOURCES+=\
    $(wildcard ${CY_APP_LIB_DIR}/source/command_console/command_console.c)\
    $(wildcard ${CY_APP_LIB_DIR}/source/command_console/COMPONENT_PSOC6HAL/*.cpp)\
    $(wildcard ${CY_APP_LIB_DIR}/source/iperf/compat/*.c)\
    $(wildcard ${CY_APP_LIB_DIR}/source/iperf/src/*.c)\
    $(wildcard ${CY_APP_LIB_DIR}/source/iperf/src/*.cpp)\
    $(wildcard ${CY_APP_LIB_DIR}/source/iperf/rtos/COMPONENT_AFR/*.c)\
    $(wildcard ${CY_APP_LIB_DIR}/source/iperf/rtos/COMPONENT_AFR/*.cpp)\
    $(wildcard ${CY_APP_LIB_DIR}/source/bluetooth_utility/COMPONENT_WICED_BLE/*.c)\

INCLUDES+=\
    ${CY_APP_LIB_DIR}/include\
    ${CY_APP_LIB_DIR}/source/command_console/COMPONENT_PSOC6HAL\
    ${CY_APP_LIB_DIR}/source/iperf/include\
    ${CY_APP_LIB_DIR}/source/iperf/rtos/COMPONENT_AFR\
    ${CY_APP_LIB_DIR}/source/iperf_utility\
    ${CY_APP_LIB_DIR}/source/bluetooth_utility/COMPONENT_WICED_BLE\
    ${CY_TEST_LIB_DIR}/connectivity-utilities\
    ${CY_TEST_LIB_DIR}/connectivity-utilities/network\
    ${CY_TEST_LIB_DIR}/connectivity-utilities/cy_string\
    ${CY_TEST_LIB_DIR}/bluetooth/wiced_include\
    $(CY_AFR_APP_CONFIG)\
