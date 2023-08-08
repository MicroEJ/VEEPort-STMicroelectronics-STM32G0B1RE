..
    Copyright 2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software..

.. |BOARD_NAME| replace:: STM32G0B1RE
.. |VEEPORT_VER| replace:: 1.0.0
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
     - ``.bss.soar``
     - ``.bss``
     - Internal SRAM
   * - MicroEJ Application threads stack blocks 
     - ``.bss.vm.stacks.java``
     - ``.ext_ram.bss``
     - Internal SRAM
   * - MicroEJ Core Engine internal heap 
     - ``ICETEA_HEAP``
     - ``.ext_ram.bss``
     - Internal SRAM
   * - MicroEJ Application heap 
     - ``_java_heap``
     - ``.ext_ram.bss``
     - Internal SRAM
   * - MicroEJ Application Immortal Heap 
     - ``_java_immortals``
     - ``.ext_ram.bss``
     - Internal SRAM
   * - MicroEJ Application resources 
     - ``.rodata.resources``
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
     - ``.text.soar``
     - ``.rodata``
     - Internal Flash

Information on MicroEJ memory sections can be found in ``xxx/stm32g0b1_threadx-bsp/projects/microej/EWARM/stm32g0b1xx_flash.icf``.

Please also refer to the MicroEJ docs website page available `here
<https://docs.microej.com/en/latest/PlatformDeveloperGuide/coreEngine.html#link>`__
for more details.

