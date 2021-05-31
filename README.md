# Amazon FreeRTOS Tester - Wi-Fi Bluetooth&reg; Console

This application integrates the command console Library including Wi-Fi iPerf and Bluetooth® Low Energy functionality. You can use this application to characterize the Wi-Fi/Bluetooth® LE functionality and performance.

Please refer command console library's [Readme.md](https://github.com/cypresssemiconductorco/command-console/tree/latest-v2.X#readme) for more information.

## Requirements

- [ModusToolbox&trade; software](https://www.cypress.com/products/modustoolbox-software-environment)

- Programming Language: C

- Supported Toolchains: Arm® GCC
- [ModusToolbox&trade; Amazon FreeRTOS SDK](https://github.com/cypresssemiconductorco/amazon-freertos)
- [Cypress Programmer](https://www.cypress.com/products/psoc-programming-solutions)
- Associated Parts: All [PSoC&trade; 6 MCU](http://www.cypress.com/PSoC6) parts with SDIO, [PSoC™ 6 MCU with AIROC™ CYW43012 Wi-Fi & Bluetooth® combo chip](https://www.cypress.com/documentation/datasheets/cyw43012-single-chip-ultra-low-power-ieee-80211n-compliant-ieee-80211ac), [PSoC™ 6 MCU with AIROC™ CYW4343W Wi-Fi & Bluetooth® combo chip](https://www.cypress.com/documentation/datasheets/cyw4343w-single-chip-80211-bgn-macbasebandradio-bluetooth-51)

For CMake build, ensure that you have the following installed. This code example was tested with CMake version 3.15.3 and Ninja version 1.9.0.
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)

## Supported kits

- [PSoC&trade; 6 Wi-Fi Bluetooth&reg; prototyping kit (CY8CPROTO-062-4343W) - Default target](https://www.cypress.com/CY8CPROTO-062-4343W)

- [PSoC&trade; 62S2 Wi-Fi Bluetooth&reg; pioneer kit (CY8CKIT-062S2-43012)](https://www.cypress.com/CY8CKIT-062S2-43012)

## Hardware setup

This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly.


## Software setup

- Install a terminal emulator if you don't have one. Instructions in this document use [Tera Term](https://ttssh2.osdn.jp/index.html.en)

- Setting up iPerf on the Host

  - Install [iPerf 2.0.13](https://sourceforge.net/projects/iperf2/files/) on the host.

  - Go to the iPerf installation directory and launch the terminal (command prompt for Windows, terminal shell for macOS or Ubuntu).

- Setting up the LE CoC application on [CYW920719B2Q40EVB-01](%) peer device

  - Build the [le_coc](https://github.com/cypresssemiconductorco/mtb-examples-CYW920719B2Q40EVB-01-btsdk-ble/tree/master/ble/le_coc) application to run LE connection-oriented channel (CoC) on the peer device. This is part of the ModusToolbox BTSDK.

  - See [Building code examples](https://github.com/cypresssemiconductorco/mtb-examples-CYW920719B2Q40EVB-01-btsdk-ble#building-code-examples) for build and installation instructions.

  **Note:** Ensure that the LE CoC application is configured to run without pairing enabled, using the client control setting.


## Using the _Amazon FreeRTOS Tester - Wi-Fi Bluetooth® Console_

### First steps:

The first three steps in this section are a one-time activity. These steps clone the Amazon FreeRTOS repository, check out the specified branch, and then finally clone this code example.

1. Clone the Amazon FreeRTOS from [GitHub](https://github.com/cypresssemiconductorco/amazon-freertos). The following command also clones all the submodules:

   ```
   git clone https://github.com/cypresssemiconductorco/amazon-freertos.git --recurse-submodules
   ```
2. Add command-console library to Amazon FreeRTOS project:

   ```
   git submodule add https://github.com/cypresssemiconductorco/command-console.git vendors/cypress/MTB/libraries/command-console
   ```
   **Note:** Currently, command-console library is not integrated with Amazon FreeRTOS .git modules and therefore, this step is mandatory.

3. Go to the *<amazon-freertos>/projects/cypress* directory. Clone or download this code example, or copy it if you already have it:

   ```
   git clone https://github.com/cypresssemiconductorco/afr-wifi-bluetooth-tester.git
   ```

   Alternatively, clone the example outside the *amazon-freertos* directory, but make sure that `AFR_PATH` and `CY_AFR_ROOT` are set correctly in *CMakeLists.txt*.

   If you use Eclipse IDE for ModusToolbox&trade;, you must clone the code example under the *\<amazon-freertos>/projects/cypress* directory because Eclipse project files use relative paths to link to the files under *\<amazon-freertos>*.

4. Go to the *\<amazon-freertos>/projects/cypress/afr-wifi-bluetooth-tester* directory and run the following command. This command will generate a build configuration under a new directory called *build* in the current directory:

   ```
   cmake -DVENDOR=cypress -DCOMPILER=arm-gcc -DBOARD=CY8CPROTO_062_4343W -S . -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=../../../tools/cmake/toolchains/arm-gcc.cmake
   ```
5. Go to the *<amazon-freertos>/projects/cypress/afr-wifi-bluetooth-tester* directory and modify the `WIFI_SSID` and `WIFI_KEY` macros in *main.c* to match with those of the Wi-Fi network that you want to connect to.

6. Update command-console to specific release tag to be compatible with `amazon-freertos`. Go to the *<amazon-freertos>/vendors/cypress/MTB/libraries/command-console* directory and check out the release tag `2.0.0.845` using the following command:

   ```
   git checkout release-v2.0.0
   ```


### Using CMake command-line interface (CLI):

1. Go to the *\<amazon-freertos>/projects/cypress/afr-wifi-bluetooth-tester* folder.

2. After CMake has generated the configuration, run the following command to build the project:

   ```
   cmake --build build
   ```
   To clean and then build, use the following command:

   ```
   cmake --build build --clean-first
   ```

3. Program the *wifi-bluetooth-tester.elf* file generated under the *build* directory using Cypress Programmer.

   After programming, the application starts automatically. Observe the messages on the UART terminal, and wait for the device to make the required connections.

4. The application connects to the configured Wi-Fi access point (AP) and obtains the IP address. When the device is ready, the `>` prompt appears.

5. Run iPerf commands (client and server) against a remote peer device

   - See [Running iPerf client and server against a remote peer device](http://github.com/cypresssemiconductorco/command-console/tree/latest-v2.X#run-iperf-client-and-server-against-a-remote-peer-device).


6. Run Bluetooth® LE commands against a remote peer device.

   - A remote peer device can be the LE CoC application on a [CYW20719B2Q40EVB-01](%) kit.

   - See [Running Bluetooth® LE commands against a remote peer device](https://github.com/cypresssemiconductorco/command-console/tree/latest-v2.X#run-ble-commands-against-a-remote-peer-device).

## Debugging

You can debug the example to step through the code. In the IDE, use the **\<Application Name> Debug (KitProg3)** configuration in the **Quick Panel**. For more details, see the "Program and Debug" section in the Eclipse IDE for ModusToolbox User Guide: *{ModusToolbox install directory}/ide_{version}/docs/mt_ide_user_guide.pdf*.

**Note:** **(Only while debugging)** On the CM4 CPU, some code in `main()` may execute before the debugger halts at the beginning of `main()`. This means that some code executes twice - before the debugger stops execution, and again after the debugger resets the program counter to the beginning of `main()`. See [KBA231071](https://community.cypress.com/docs/DOC-21143) to learn about this and for the workaround.

## Design and implementation

This application integrates the command console library including Wi-Fi iPerf and Bluetooth® LE functionality.
The command-console library is initialized which allows you to test the Wi-Fi, Bluetooth®, and iPerf commands through the command line.

### Resources and settings

**Table 1. Application resources**

| Resource  |  Alias/object     |    Purpose     |
| :------- | :------------    | :------------ |
| UART (HAL)|cy_retarget_io_uart_obj| UART HAL object used by Retarget-IO for debug UART port  |

## Related resources

| Application notes                                            |                                                              |
| :----------------------------------------------------------- | :----------------------------------------------------------- |
| [AN228571](https://www.cypress.com/AN228571) – Getting started with PSoC&trade; 6 MCU on ModusToolbox&trade; | Describes PSoC&trade; 6 MCU devices and how to build your first application with ModusToolbox&trade; |
| [AN228571](https://www.cypress.com/AN228571) – Getting started with PSoC&trade; 6 MCU on ModusToolbox&trade; | Describes PSoC&trade; 6 MCU devices and how to build your first application with ModusToolbox&trade; |
| [AN221774](https://www.cypress.com/AN221774) – Getting started with PSoC&trade; 6 MCU on PSoC&trade; Creator&trade; | Describes PSoC&trade; 6 MCU devices and how to build your first application with PSoC&trade; Creator&trade; |
| [AN210781](https://www.cypress.com/AN210781) – Getting started with PSoC&trade; 6 MCU with Bluetooth® Low Energy connectivity on PSoC&trade; Creator&trade; | Describes PSoC&trade; 6 MCU with Bluetooth® LE connectivity devices and how to build your first application with PSoC&trade; Creator&trade; |
| [AN215656](https://www.cypress.com/AN215656) – PSoC&trade; 6 MCU: dual-CPU system design | Describes the dual-CPU architecture in PSoC&trade; 6 MCU, and shows how to build a simple dual-CPU design |
| **Code examples**                                            |                                                              |
| [Using ModusToolbox&trade;](https://github.com/cypresssemiconductorco/Code-Examples-for-ModusToolbox-Software) | [Using PSoC&trade; Creator&trade;](https://www.cypress.com/documentation/code-examples/psoc-6-mcu-code-examples) |
| **Device documentation**                                     |                                                              |
| [PSoC&trade; 6 MCU datasheets](https://www.cypress.com/search/all?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A575&f[2]=field_related_products%3A114026) | [PSoC&trade; 6 technical reference manuals](https://www.cypress.com/search/all/PSoC%206%20Technical%20Reference%20Manual?f[0]=meta_type%3Atechnical_documents&f[1]=resource_meta_type%3A583) |
| **Development kits**                                         | Buy at www.cypress.com                                       |
| [CY8CPROTO-062-4343W](https://www.cypress.com/CY8CPROTO-062-4343W) PSoC&trade; 6 Wi-Fi Bluetooth® prototyping kit | [CY8CKIT-062S2-43012](https://www.cypress.com/CY8CKIT-062S2-43012) PSoC&trade; 62S2 Wi-Fi Bluetooth® pioneer kit |
| **Libraries**                                                 |                                                              |
| PSoC&trade; 6 peripheral driver library (PDL) and docs                    | [psoc6pdl](https://github.com/cypresssemiconductorco/psoc6pdl) on GitHub |
| Hardware abstraction layer (HAL) library and docs          | [psoc6hal](https://github.com/cypresssemiconductorco/psoc6hal) on GitHub |
| Retarget-IO - A utility library to retarget the standard input/output (STDIO) messages to a UART port | [retarget-io](https://github.com/cypresssemiconductorco/retarget-io) on GitHub |
| **Middleware**                                               |                                                              |
| Command console library                                      | [Command console library](https://github.com/cypresssemiconductorco/command-console/tree/latest-v2.X) on GitHub |
| Links to all PSoC&trade; 6 MCU middleware                           | [psoc6-middleware](https://github.com/cypresssemiconductorco/psoc6-middleware) on GitHub |
| **Tools**                                                    |                                                              |
| [Eclipse IDE for ModusToolbox&trade;](https://www.cypress.com/modustoolbox)     | The cross-platform, Eclipse-based IDE for IoT designers that supports application configuration and development targeting converged MCU and wireless systems.             |
| [PSoC&trade; Creator&trade;](https://www.cypress.com/products/psoc-creator-integrated-design-environment-ide) | The Cypress IDE for PSoC&trade; and FM0+ MCU development.            |

## Other resources

- [Command console library](http://github.com/cypresssemiconductorco/command-console)

- [Bluetooth® FreeRTOS library](http://github.com/cypresssemiconductorco/bluetooth-freertos)

Cypress provides a wealth of data at www.cypress.com to help you select the right device, and quickly and effectively integrate it into your design.

For PSoC&trade; 6 MCU devices, see [How to design with PSoC&trade; 6 MCU - KBA223067](https://community.cypress.com/docs/DOC-14644) in the Cypress community.

## Document history

Document title: Amazon FreeRTOS Tester - Wi-Fi Bluetooth® Console

| Version | Description of change |
| ------- | --------------------- |
| 1.0.0   | New code example      |
| 1.0.1   | Documentation updates |

-----------------------------------------------------

All other trademarks or registered trademarks referenced herein are the property of their respective owners.

![banner](images/ifx-cy-banner.png)

-----------------------------------------------------

© Cypress Semiconductor Corporation, 2020-2021. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br>
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product.  CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications.  To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document.  Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device.  You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device.  Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br>
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolBox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries.  For a more complete list of Cypress trademarks, visit cypress.com.  Other names and brands may be claimed as property of their respective owners.
