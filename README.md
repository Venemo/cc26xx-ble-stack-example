# cc26xx-ble-stack-example

Example project that serves as a base for custom BLE stack projects.

The source files are directly copied from TI's BLE SDK, and as such, are subject to TI's copyright, TI's licensing, etc.
This repository merely exists for the reason of making it easier to create customized BLE SDK apps.

The author of this repository does NOT take ANY responsibility for ANYTHING in this repo.
For officially supported TI stuff, go to ti.com

# What this is and what it isn't

This repository doesn't contain any code that you couldn't find in TI's BLE SDK. It also doesn't contain any modifications to the TI BLE SDK, besides some minor changes that make them build on Linux.

However, it serves as a useful basis for a custom BLE stack project because:

The files in this repo are actual files in your file system, not links that point inside the BLE SDK. So you can use this independently of the SDK, meaning that you can edit the files without editing the files in the SDK.

## How to customize this example

* Edit `targetConfigs/CC2650F128.ccxml` to select the correct device you use (eg. CC2640)
* Edit the `Board.h` and `board.c` files according to your own project
* Edit the `main.c` and `simple_peripheral.h` / `simple_peripheral.c`
* Change `ble-user-config.h` to match your desires
* Decide which TI profiles you need and add / remove TI's profiles to / from `ble-stack/profiles`
* Create your own profiles, etc.

## How to flash

You can use Uniflash (which is cross-platform) or SmartRF (which seems to be Windows-only).

Flash the stack project first (this example works with `simple_peripheral_cc2650em_stack`), then this example project.  
You can also create a custom stack project similarly to the description on how to create this example, but that is out of scope for us for now.

## Troubleshooting

When you use the CC2640 and receive something like this:

```
Error connecting to the target:
(Error -241 @ 0x0)
A router subpath could not be accessed.
A security error has probably occurred.
Make sure your device is unlocked.
(Emulation package 6.0.628.1)
```

Open `targetConfigs/CC2650F128.ccxml` and change the target device from CC2650 to CC2640.

## How to reproduce this example project

* Set up the development environment
* Import the Simple Peripheral app/stack examples and build them
* Fix any issues that might arise.

NOTE: The example project will also use the Simple Peripheral example stack project (it is not such a big deal after all), so here we will only focus on creating a custom app project.

### Basic project structure

* Create an empty RTSC project for the CC26xx (CCSv7 will only let you create such a project for the CC2650). In the project creation wizard, select the TI compiler (a proper version) and the XDCtools version that came with your BLE SDK version, and also select the proper platform "ti.platforms.simplelink:CC2650F128"
* Right-click the project -> Properties -> Build -> XDCtools -> Advanced options -> Add `"${COMPILER_FLAGS}"` to Additional compiler options
* Create a `tools` directory under your project and add two subdirectories: `rtsc` and `linker`
* Create a `ble-sdk` directory under your project
* Copy BLE SDK file `src/common/cc26xx/kernel/cc2640/config/cc2640.cfg` to your project's `tools/rtsc/`
* Copy BLE SDK file `src/common/cc26xx/ccs/cc26xx_app.cmd` to your project's `tools/linker`

### Compiler and linker settings

* Right click on your project -> Properties
* Resource -> Linked resources -> add `CC26XXWARE` that points to `"${CCS_INSTALL_ROOT}/../../tirtos_cc13xx_cc26xx_2_20_01_08/products/cc26xxware_2_24_02_17393"`
* Build -> ARM Linker -> Advanced options -> Command file preprocessing -> Add the defines found in the BLE SDK file `examples/cc2650em/simple_peripheral/ccs/config/ccs_linker_defines.cmd`
* Build -> ARM Linker -> File search path -> change `${COM_TI_RTSC_TIRTOSCC13XX_CC26XX_LIBRARIES}` to `"${CC26XXWARE}/driverlib/bin/ccs/driverlib.lib"`
* Build -> ARM Linker -> File search path -> change `${COM_TI_RTSC_TIRTOSCC13XX_CC26XX_LIBRARY_PATH}` to `"${CC26XXWARE}/driverlib/bin/ccs"`
* Build -> ARM Compiler -> Predefined symbols -> Add the defines found in the BLE SDK file `examples/cc2650em/simple_peripheral/ccs/config/ccs_compiler_defines.bcfg`
* Build -> ARM Compiler -> Include options -> change `"${COM_TI_RTSC_TIRTOSCC13XX_CC26XX_INCLUDE_PATH}"` to `${CC26XXWARE}`

### BLE stack

* Right click on your project -> Properties
* Resource -> Linked resources -> add `BLESDK_SRC` that points to `"${PROJECT_LOC}/ble-stack"`
* Build -> ARM Compiler -> Include options

At this point, you should choose an example project (here, we used simple peripheral example).

Take a look at the include paths of your chosen example, and add them using the newly configured `BLESDK_SRC`.
For the simple peripheral, these are the following:

```
"${BLESTACK_SRC}/inc"
"${BLESTACK_SRC}/rom"
"${BLESTACK_SRC}/icall/inc"
"${BLESTACK_SRC}/common/cc26xx"
"${BLESTACK_SRC}/components/heapmgr"
"${BLESTACK_SRC}/controller/cc26xx/inc"
"${BLESTACK_SRC}/components/hal/src/target/_common"
"${BLESTACK_SRC}/components/hal/src/target/_common/cc26xx"
"${BLESTACK_SRC}/components/hal/src/inc"
"${BLESTACK_SRC}/components/osal/src/inc"
"${BLESTACK_SRC}/components/services/src/sdata"
"${BLESTACK_SRC}/components/services/src/saddr"
"${BLESTACK_SRC}/components/icall/src/inc"
"${BLESTACK_SRC}/components/npi/src"
"${BLESTACK_SRC}/profiles/roles"
"${BLESTACK_SRC}/profiles/roles/cc26xx"
"${BLESTACK_SRC}/profiles/dev_info"
"${BLESTACK_SRC}/profiles/simple_profile"
"${BLESTACK_SRC}/examples/simple_peripheral/cc26xx/app"
"${BLESTACK_SRC}/boards/CC2650DK_7ID"
```

* Build -> ARM Compiler -> Predefined symbols

Take a look at the defined symbols of your chosen example, and copy them to your project.
For the simple peripheral, these are the following.

```
CC2650EM_7ID
CC26XX
BOARD_DISPLAY_EXCLUDE_UART
HEAPMGR_SIZE=0
ICALL_MAX_NUM_ENTITIES=6
ICALL_MAX_NUM_TASKS=3
POWER_SAVING
USE_ICALL
xBOARD_DISPLAY_EXCLUDE_LCD
xDisplay_DISABLE_ALL
xdc_runtime_Assert_DISABLE_ALL
xdc_runtime_Log_DISABLE_ALL
```

* Copy the files required by your chosen example from the BLE stack sources to your project's `ble-stack` directory.

NOTE: the physical structure of the BLE stack is different from the structure it appears in the example projects. You will need to use the physical paths.

After this, the project should build successfully.

### Starting customizations

* Create a `source` directory under your project, whose purpose is to hold your custom code. Move files from `ble-stack/examples/simple_peripheral/cc26xx/app` to this folder.
* Remove TI's `Board.h` and board-related stuff, and add your own

# How to use the BLE Stack on Linux

## Setting up the development environment

NOTE: TI expects you to install its stuff into `~/ti`, so the `${TI_PRODUCTS_DIR}` variable will
always be set to `~/ti` regardless of where you installed it. This variable unfortunately cannot be
changed, you you might need to workaround it if you didn't install into `~/ti`. See [this E2E forum thread](https://e2e.ti.com/support/development_tools/code_composer_studio/f/81/t/480411) for more details.

NOTE: I prefer to install it into `~/dev/ti`, but this is your choice. It will still create `~/ti` regardless of where you installed it.

NOTE: Unfortunately, you will need to patch the BLE Stack (see below), but fortunately the patched SDK will also work on Windows.

* Install CCSv7 to `~/dev/ti` -> it will create a `ccsv7` folder and an `xdctools_(version)` folder
* Delete `~/dev/ti/xdctools*`
* Install the BLE Stack using `wine ble_sdk_(version)_setup.exe` to the default location
* Check `~/.wine/drive_c/ti` for the TI-RTOS version
* Download same TI-RTOS Linux version from TI
* Install said TI-RTOS version to `~/dev/ti` -> it will create a `tirtos_cc13xx_cc26xx_(version)` folder and an `xdctools_(version)_core` folder
* Copy `~/.wine/drive_c/ti/simplelink` to `~/dev/ti`
* Patch the BLE SDK using the [convert script](https://github.com/jcormier/TI_BLE_CC2650_Linux_Convert) which a helpful user extracted from [this E2E forum thread](https://e2e.ti.com/support/wireless_connectivity/bluetooth_low_energy/f/538/p/412962/1911528#1911528) - you might need to adjust it for more recent SDK and TIRTOS versions
* Launch CCS
* It will recognize the newly installed xdctools and tirtos versions - accept to install them if it offers
* Go to Help -> Install new software
* Install the latest version of the TI ARM compiler

NOTE: you can no longer install the BLE SDK using the resource explorer, but that is not an issue; you can still import and build examples from it.

## Importing and building example projects

* Launch CCS
* Click on Import project
* Browse to `~/dev/ti/simplelink/ble_sdk_(version)/examples/(kit)/(example)`
* Select both the app and stack projects
* Tick "Copy projects into workspace"
* Import the projects
* Right-click the project -> Properties
* Go to CCS General
* Set the compiler version to the latest
* Hit OK
* Right-click the project -> Build Project
* Build might fail at the first attempt because of the incorrect includes (maybe they were cached), fix this using Clean Project, then clicking on Build Project again

On older SDK versions, you might need to:

* Right-click the project -> Properties
* Go to Resource -> Linked resources
* Change `${TI_PRODUCTS_DIR}` to `${CCS_INSTALL_ROOT}/..`

NOTE: Sometimes CCS will create a `cc26x0f128.cmd` without asking you. When this happens, you will get a linker error. To fix that error, simply delete this file. See [this E2E forum thread](https://e2e.ti.com/support/embedded/tirtos/f/355/t/464872) for more details.

