# Install script for directory: C:/Users/Diogo/ncs/v1.9.1/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Zephyr-Kernel")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Users/Diogo/ncs/v1.9.1/toolchain/opt/bin/arm-none-eabi-objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/soc/arm/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/nrf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/trusted-firmware-m/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/cjson/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/cddl-gen/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/memfault-firmware-sdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/CANopenNode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/civetweb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/lz4/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/nrf_hw_models/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/tinycbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/tinycrypt/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/TraceRecorder/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/zscilib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/nrfxlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/modules/connectedhomeip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/Diogo/Documents/setr/repo/SETR_P2_G7/aula10-11/fifo/fifo/build/zephyr/cmake/reports/cmake_install.cmake")
endif()

