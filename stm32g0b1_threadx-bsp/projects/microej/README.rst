.. 
    Copyright 2023 MicroEJ Corp. All rights reserved.
    Use of this source code is governed by a BSD-style license that can be found with this software.

.. |BOARD_NAME| replace:: STM32G0B1RE
.. |VEEPORT| replace:: VEE Port
.. |RTOS| replace:: Azure RTOS (ThreadX)
.. |BSP_NAME| replace:: STM32CubeG0

.. _README: ./../../../README.rst

================
|BOARD_NAME| BSP
================

This project contains the BSP sources of the |VEEPORT| for the
|BOARD_NAME|. The BSP project is based on |BSP_NAME| .

This document does not describe how to setup the |VEEPORT|. Please
refer to the `README`_ for that.

Build & Run Scripts
---------------------

In the directory ``project/microej/scripts/`` are scripts that can be
used to build and flash the BSP.  The ``.bat`` and ``.sh`` scripts are
meant to run in a Windows and Linux environment respectively.

- The ``build*`` scripts are used to compile and link the BSP with a
  MicroEJ Application to produce a MicroEJ Firmware
  (``application.out``) that can be flashed on a device.

  The ``build*`` scripts work out of the box, assuming the toolchain is
  installed in the default path.

- The ``run*`` scripts are used to flash a MicroEJ Firmware
  (``application.out``) on a device.

The following environment variables are customizable:  

- ``IAREW_INSTALLATION_DIR``: The install directory of the IAR toolchain.

See the comments for each variables in the scripts for a detailed
explanation.

The environment variables can be defined globally by the user or in
the ``set_local_env*`` scripts.  When the ``.bat`` (``.sh``) scripts
are executed, the ``set_local_env.bat`` (``set_local_env.sh``) script
is executed if it exists.  Create and configure these files to
customize the environment locally.  Template files are provided as
example, see ``set_local_env.bat.tpl`` and ``set_local_env.sh.tpl``.

Customize BSP
-------------


Microjvm task
~~~~~~~~~~~~~

There are several macros available to customize the C task of the microjvm in the file ``stm32g0b1_threadx-bsp\projects\microej\main\src\main.c``:

- ``MICROJVM_STACK_SIZE``: indicates the amount of memory allocated for the microjvm stack.
- ``JAVA_TASK_PRIORITY``: indicates priority of the microjvm C task.


Main Traces
~~~~~~~~~~~

In the file ``stm32g0b1_threadx-bsp\projects\microej\main\inc\main.h``, uncomment the macro ``MICROEJ_MAIN_DEBUG`` to display traces in the main function.
It's enabled by default.

Please use ``MICROEJ_MAIN_TRACE`` to print traces from the file ``stm32g0b1_threadx-bsp\projects\microej\main\src\main.c``.



Launch Coremark
~~~~~~~~~~~~~~~

In the file ``stm32g0b1_threadx-bsp\projects\microej\main\inc\main.h``, uncomment the macro ``VALIDATION_BUILD`` in order to launch coremark benchmark.
Note that the java application cannot start when this macro is enabled.

Flash the Board
---------------

The ``run*`` scripts can also be used to flash the device with the
MicroEJ Firmware.

The board can also be flashed using the IAR IDE. Once the IAR IDE is open with the workspace ``stm32g0b1_threadx-bsp\projects\microej\EWARM\application.eww``, click on ``Project > Download > Download active application``.

Debugging with |BOARD_NAME|
---------------------------

A debug session can be started using IAR IDE. Once the IAR IDE is open with the workspace ``stm32g0b1_threadx-bsp\projects\microej\EWARM\application.eww``,

- Right click on the project and select ``Options...``.
- Then go to the ``C/C++ Compiler`` tab and set the ``Optimizations > Level`` to ``None``.
- After that, rebuild the firmware and click on ``Project > Download and Debug``.

Dependencies
------------

The following dependencies are included as part of the BSP:

- name="bsp-microej-main" rev="2.0.0"
- name="mjvm-threadx" rev="1.1.0"
- name="bsp-generic" rev="1.1.0"

