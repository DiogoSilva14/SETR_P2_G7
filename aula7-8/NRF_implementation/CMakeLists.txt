# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(vending_machine)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    )

target_sources(app PRIVATE src/device_drivers.c src/start_vending_machine.c src/state_machine.c src/vending_machine.c)
