..
    Copyright 2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software..

.. |BOARD_NAME| replace:: STM32G0B1RE
.. |VEEPORT_VER| replace:: 1.0.1
.. |VEEPORT| replace:: VEE Port
.. |MANUFACTURER| replace:: STMicroelectronics
.. |MICROEJ_ARCHITECTURE_VERSION| replace:: 8.0.0

========================================================
|VEEPORT| Release Notes for |MANUFACTURER| |BOARD_NAME|
========================================================

Description
===========

This is the release notes of the |VEEPORT| for |BOARD_NAME|.

Versions
========

|VEEPORT|
---------

|VEEPORT_VER|

Dependencies
------------

This |VEEPORT| contains the following dependencies:

.. list-table::
   :header-rows: 1
   
   * - Dependency Name
     - Version
   * - Architecture (flopi0PI33)
     - |MICROEJ_ARCHITECTURE_VERSION|

Please refer to the |VEEPORT| `module description file <./stm32g0b1_threadx-configuration/module.ivy>`_ 
for more details.

Board Support Package
---------------------

- BSP provider and name: |MANUFACTURER| (``STM32CubeG0``)
- BSP version: 1.6.1

Please refer to the |MANUFACTURER| (``STM32CubeG0``) git repository
available `here
<https://github.com/STMicroelectronics/STM32CubeG0>`__.

Third Party Software
--------------------

Third party software used in BSP can be found `here
<https://github.com/STMicroelectronics/x-cube-azrtos-g0>`__. Here
is a list of the most important ones:

.. list-table::
   :widths: 3 3 3

   * - RTOS 
     - Azure RTOS
     - 1.1.0

Features
========

- N/A


MISRA Compliance
================

This VEE Port has a list of components that are MISRA-compliant (MISRA C:2012) with some noted exception.
Below is the list of ``folders that have been verified``:

- microej/core/
- microej/main/

Among the folders verified, below is the list of ``files that have not been verified``:

- microej/core/src/LLMJVM_ThreadX.c
- microej/core/src/microej_main.c
- microej/core/src/LLBSP_generic.c
- microej/core/inc/misra_2004_conf.h
- microej/main/src/system_stm32g0xx.c
- microej/main/src/stm32g0xx_hal_msp.c

It has been verified with Cppcheck v2.10. Here is the list of deviations from MISRA standard:

+------------+-----------+----------------------------------------------------------------------+
| Deviation  | Category  | Justification                                                        |
+============+===========+======================================================================+
| Rule 2.5   | Advisory  | A macro can be defined at API level and not used by the application  |
|            |           |                                                                      |
| Rule 8.4   | Required  | Missing compatible declarations, cannot be seen during analysis.     |
|            |           |                                                                      |
| Rule 8.7   | Advisory  | External linkage, cannot be seen during analysis.                    |
|            |           |                                                                      |
| Rule 8.9   | Advisory  | TGlobal variable necessary to do only one init.                      |
|            |           |                                                                      |
| Rule 10.1  | Required  | This use case enter in the rule exception of MISRA (false positive)  |
|            |           |                                                                      |
| Rule 11.4  | Advisory  | Used when coding BSP C source code (drivers, etc.)                   |
|            |           |                                                                      |
| Rule 17.7  | Required  | The use of returned values of debug traces is not necessary.         |
|            |           |                                                                      |
| Rule 17.8  | Advisory  | Can be useful when designing C library                               |
|            |           |                                                                      |
| Rule 21.2  | Required  | fputc reserved identifier is overridden on purpose.                  |
|            |           |                                                                      |
| Rule 21.6  | Required  | Used for printf usage                                                |
+------------+-----------+----------------------------------------------------------------------+

Known Issues/Limitations
========================

Known issues:

- N/A

Limitations:

- N/A


|VEEPORT| Memory Layout
=======================

Memory Sections
---------------

IAR linker configuration files provide memory section information and the memory layout. They can be found in the following directory: 
``xxx/stm32g0b1_threadx-bsp/projects/microej/EWARM``

The memory layout is stored in ``stm32g0b1xx_flash.icf``.

Memory Layout
-------------

.. list-table::
   :header-rows: 1
   
   * - Section Content
     - Section Source
     - Section Destination
     - Memory Type
   * - MicroEJ Application static
     - ``.bss.microej.statics``
     - ``.bss``
     - Internal SRAM
   * - MicroEJ Application threads stack blocks 
     - ``.bss.microej.stacks``
     - ``.ext_ram.bss``
     - Internal SRAM
   * - MicroEJ Core Engine internal heap 
     - ``.bss.microej.runtime``
     - ``.ext_ram.bss``
     - Internal SRAM
   * - MicroEJ Application heap 
     - ``.bss.microej.heap``
     - ``.ext_ram.bss``
     - Internal SRAM
   * - MicroEJ Application Immortal Heap 
     - ``.bss.microej.immortals``
     - ``.ext_ram.bss``
     - Internal SRAM
   * - MicroEJ Application resources 
     - ``.rodata.microej.resource.*``
     - ``.rodata``
     - Internal Flash
   * - MicroEJ System Applications code and resources 
     - ``.rodata.soar.features``
     - ``.rodata``
     - Internal Flash
   * - MicroEJ System Application statics 
     - ``.bss.features.installed``
     - ``.ext_ram.bss``
     - N/A
   * - MicroEJ Shielded Plug data 
     - ``.shieldedplug``
     - ``.rodata``
     - N/A
   * - MicroEJ Application and Library code 
     - ``.rodata.microej.soar``
     - ``.rodata``
     - Internal Flash

Information on MicroEJ memory sections can be found in ``xxx/stm32g0b1_threadx-bsp/projects/microej/EWARM/stm32g0b1xx_flash.icf``.

Please also refer to the MicroEJ docs website page available `here
<https://docs.microej.com/en/latest/PlatformDeveloperGuide/coreEngine.html#link>`__
for more details.

