# Amazon FreeRTOS (AFR) Wi-Fi Bluetooth Tester
This application integrates command console library including Wi-Fi iPerf and Bluetooth BLE functionality. Using this application, the user can characterize the Wi-Fi/BLE functionality and performance.

## Requirements
- [ModusToolboxÂ® software](https://www.cypress.com/products/modustoolbox-software-environment) v2.2
- Programming Language: C
- Supported Toolchains: Arm® GCC
- [ModusToolbox Amazon FreeRTOS SDK](https://github.com/cypresssemiconductorco/amazon-freertos)
- [Cypress Programmer](https://www.cypress.com/products/psoc-programming-solutions)
- Associated Parts: All [PSoCÂ® 6 MCU](http://www.cypress.com/PSoC6) parts with SDIO, CYW43012, CYW4343W

For CMake build, ensure that you have the following installed. This code example was tested with CMake version 3.15.3 and Ninja version 1.9.0.
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)

## Dependent assets
- [Command Console Library](http://github.com/cypresssemiconductorco/command-console)
- [Bluetooth FreeRTOS Library](http://github.com/cypresssemiconductorco/bluetooth-freertos)

## Supported kits
- [PSoC 6 Wi-Fi BT Prototyping Kit](https://www.cypress.com/CY8CPROTO-062-4343W) (CY8CPROTO-062-4343W) - Default target

- [PSoC 62S2 Wi-Fi BT Pioneer Kit](https://www.cypress.com/CY8CKIT-062S2-43012) (CY8CKIT-062S2-43012)

## Hardware setup
This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.

## Software setup
- Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en)

- [iPerf 2.0.13](https://sourceforge.net/projects/iperf2/files/) (Supported on Ubuntu, macOS and Windows)

## Using the Wi-Fi Bluetooth Tester

### First Steps:

The first three steps in this section are a one-time activity. These steps clone the Amazon FreeRTOS repository, check out the specified branch, and then finally clone this code example.

1. Clone the Amazon FreeRTOS from [GitHub](https://github.com/cypresssemiconductorco/amazon-freertos). The following command also clones all the submodules.

   ```
   git clone https://github.com/cypresssemiconductorco/amazon-freertos.git --recurse-submodules
   ```
2. Add command-console library to Amazon FreeRTOS project

   ```
   git submodule add git@devops-git.aus.cypress.com:repo-staging/command-console.git vendors/cypress/MTB/libraries/command-console
   ```
   *Note: currently command-console library is not integrated with Amazon FreeRTOS .git modules and hence this step is required.*

3. Go to the *<amazon-freertos>/projects/cypress* directory. Clone or download this code example, or copy it if you already have it.

   ```
   git clone https://github.com/cypresssemiconductorco/afr-wifi-bluetooth-tester.git
   ```

   Alternatively, clone the example outside the *amazon-freertos* directory, but make sure that `AFR_PATH` and `CY_AFR_ROOT` are set correctly in *CMakeLists.txt* and *Makefile* respectively. 
   
   If you use Eclipse IDE for ModusToolbox, you must clone the code example under the *\<amazon-freertos>/projects/cypress* directory because Eclipse project files use relative paths to link to the files under *\<amazon-freertos>*.

4. Go to the *<amazon-freertos>/projects/cypress/afr-wifi-bluetooth-tester* directory and Modify the macros WIFI_SSID and WIFI_KEY in main.c to match with those of the Wi-Fi network that you want to connect to.

### Using CMake Command Line Interface (CLI):

1. Go to the *\<amazon-freertos>/projects/cypress/afr-wifi-bluetooth-tester* folder.

2. Run the following command. This command will generate a build configuration under a new directory called *build* in the current directory:

   ```
   cmake -DVENDOR=cypress -DCOMPILER=arm-gcc -DBOARD=CY8CPROTO_062_4343W -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=../../../tools/cmake/toolchains/arm-gcc.cmake
   ```

3. After CMake has generated the configuration, run the following command to build the project:

   ```
   cmake --build build
   ```
   To clean and then build, use the following command:

   ```
   cmake --build build --clean-first
   ```

4. Program the *wifi-bluetooth-tester.elf* file generated under the *build* directory using Cypress Programmer.

   After programming, the application starts automatically. Observe the messages on the UART terminal, and wait for the device to make all the required connections.

5. The application will connect to the configured Wi-Fi Access Point and obtain the IP address. When the device is ready, `>` prompt will appear.

## Setting up iPerf on the host

1. Install [iPerf 2.0.13](https://sourceforge.net/projects/iperf2/files/) on the host.

2. Go to iPerf installation directory and launch the terminal (command prompt for Windows, terminal shell for macOS or Ubuntu).

## Setting up LE COC application on CYW920719B2Q40EVB-01 peer device

1. To run LE COC on the peer device you need to build [le_coc](https://github.com/cypresssemiconductorco/mtb-examples-CYW920719B2Q40EVB-01-btsdk-ble/tree/master/ble/le_coc) , that is part of ModusToolbox BTSDK.

2. Refer to [Building code examples](https://github.com/cypresssemiconductorco/mtb-examples-CYW920719B2Q40EVB-01-btsdk-ble#building-code-examples) section for build and installation instructions.

*Note: Ensure that the LE COC application is configured to run without pairing enabled, using the setting in client control.*

### Instructions to run iPerf (client and server) against a remote peer device
See [Running iPerf client and server against a remote peer device](http://github.com/cypresssemiconductorco/command-console##run-iperf-client-and-server-against-a-remote-peer-device)

### Instruction to run BLE commands against a remote peer device (such as LE CoC application on a CYW20719B2Q40EVB-01 kit)
See [Running BLE commands against a remote peer device](http://github.com/cypresssemiconductorco/command-console#run-ble-commands-against-a-remote-peer-device)

## Additional Information
- [Command Console Library API Reference Guide](https://cypresssemiconductorco.github.io/command-console/api_reference_manual/html/index.html)

- [AFR Wi-Fi Bluetooth Tester Version](./version.txt)

