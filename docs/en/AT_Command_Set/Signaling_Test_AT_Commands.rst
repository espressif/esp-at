.. _FACT-AT:

Signaling Test AT Commands
==========================================

:link_to_translation:`zh_CN:[中文]`

-  :ref:`AT+FACTPLCP <cmd-FACTPLCP>`: Send with long or short PLCP (Physical Layer Convergence Procedure)

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
