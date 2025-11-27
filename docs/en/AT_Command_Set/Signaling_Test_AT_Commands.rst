.. _FACT-AT:

Signaling Test AT Commands
==========================================

:link_to_translation:`zh_CN:[中文]`

- :ref:`Introduction <cmd-signal-test-intro>`
-  :ref:`AT+FACTPLCP <cmd-FACTPLCP>`: Send with long or short PLCP (Physical Layer Convergence Procedure)

.. _cmd-signal-test-intro:

Introduction
------------

.. important::
  The default AT firmware supports all the AT commands mentioned on this page. If you don't need {IDF_TARGET_NAME} to support signaling test commands, you can compile the ESP-AT project by following the steps in :doc:`Compile ESP-AT Project Locally <../Compile_and_Develop/How_to_clone_project_and_compile_it>` documentation. In the project configuration during the fifth step, make the following selections:

  - Disable ``Component config`` > ``AT`` > ``AT signaling test command support``

.. _cmd-FACTPLCP:

:ref:`AT+FACTPLCP <FACT-AT>`: Send with Long or Short PLCP
--------------------------------------------------------------------------

Set Command
^^^^^^^^^^^

**Command:**

::

    AT+FACTPLCP=<enable>,<tx_with_long>

**Response:**

::

    OK

Parameters
^^^^^^^^^^

-  **<enable>**: Enable or disable manual configuration.

   -  0: Disable manual configuration. The default value for the parameter ``<tx_with_long>`` will be used.
   -  1: Enable manual configuration. The type of PLCP that AT sends depends on ``<tx_with_long>``.

-  **<tx_with_long>**: Send with long PLCP or short PLCP.

   -  0: Send with short PLCP (default).
   -  1: Send with long PLCP.
