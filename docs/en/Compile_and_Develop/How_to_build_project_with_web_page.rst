Compile ESP-AT Project on the GitHub Webpage
============================================

:link_to_translation:`zh_CN:[中文]`

This document provides a detailed guide on how to compile the ESP-AT project through a web page. When the default :doc:`officially released firmware <../AT_Binary_Lists/index>` does not satisfy your requirements, for example, you want to enable :doc:`WebSocket functionality <../AT_Command_Set/websocket_at_commands>` and disable :ref:`mDNS functionality <cmd-MDNS>`, you will need to compile the ESP-AT project. Usually we recommend you to :doc:`Compile ESP-AT Project Locally <How_to_clone_project_and_compile_it>`. However, if you have difficulties building the project locally, please refer to this document to compile the ESP-AT project through the webpage.

.. attention::
  **The AT firmware compiled from webpages needs to be tested and verified for functionality based on your own product.**

  **Please save the firmware and download link**, for possible issue debugging in the future.

Detailed Steps
^^^^^^^^^^^^^^

Please follow the detailed steps below to complete the Fork, environment configuration, code modification, and compilation of the ESP-AT project.

* :ref:`at-wc-github-login`
* :ref:`at-wc-project-fork`
* :ref:`at-wc-enable-github-action`
* :ref:`at-wc-config-secrets`
* :ref:`at-wc-commit-changes`
* :ref:`at-wc-action-compile`

.. _at-wc-github-login:

Step 1. Log in to your GitHub account
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Before starting, please `log in to your GitHub account <https://github.com/login>`_ as compile and firmware download require login credentials.

.. _at-wc-project-fork:

Step 2. Fork the ESP-AT project
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Visit the `ESP-AT repository <https://github.com/espressif/esp-at>`_ and click ``Fork`` in the upper right corner of the page.

  .. figure:: ../../_static/web_compile/fork-s1.png
    :align: center
    :alt: Click Fork
    :figclass: align-center

    Click Fork

Click ``Create fork`` to copy the repository to your GitHub account. The default Fork only copies the master branch. If you need to modify the code based on a specific branch, uncheck ``Copy the master branch only.``

  .. figure:: ../../_static/web_compile/fork-s2.png
    :align: center
    :alt: Create Fork
    :figclass: align-center

    Create Fork

.. _at-wc-enable-github-action:

Step 3. Enable GitHub Actions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Under the repository, click ``Actions`` to enter the GitHub Actions page.

  .. figure:: ../../_static/web_compile/github-action-enable-s1.png
    :align: center
    :alt: Click Actions
    :figclass: align-center

    Click Actions

Click ``I understand my workflows, go ahead and enable them`` to enable GitHub Actions.

  .. figure:: ../../_static/web_compile/github-action-enable-s2.png
    :align: center
    :alt: Enable GitHub Actions
    :figclass: align-center

    Enable GitHub Actions

GitHub Actions is now enabled.

  .. figure:: ../../_static/web_compile/github-action-enable-s3.png
    :align: center
    :alt: GitHub Actions Enabled Successfully
    :figclass: align-center

    GitHub Actions Enabled Successfully

.. _at-wc-config-secrets:

Step 4. Configure the secrets required to compile the ESP-AT project
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

If you have an account and an OTA token of `the Espressif OTA server <http://iot.espressif.cn>`_, and you need to upgrade the AT firmware by using the :ref:`AT+CIUPDATE <cmd-UPDATE>` command, you need to complete this step. Otherwise, it is recommended to disable CONFIG_AT_OTA_SUPPORT (see :ref:`at-wc-commit-changes` for details) or skip this step.

Under the repository, click ``Setting`` to enter the settings page.

  .. figure:: ../../_static/web_compile/cfg-ota-token-s1.png
    :align: center
    :alt: Click on Settings
    :figclass: align-center

    Click on Settings

Click on ``Settings`` -> ``Secrets and variables`` -> ``Actions`` to access the Action configuration page.

  .. figure:: ../../_static/web_compile/cfg-ota-token-s2.png
    :align: center
    :alt: Enter Actions Configuration Page
    :figclass: align-center

    Enter Actions Configuration Page

Click ``New repository secret`` to enter the secret creation page.

  .. figure:: ../../_static/web_compile/cfg-ota-token-s3.png
    :align: center
    :alt: Create secret Page
    :figclass: align-center

    Create Secret Page

Enter ``Name`` and ``Secrets``, and click ``Add secret`` to create a new secret. This secret is the OTA token.

  .. figure:: ../../_static/web_compile/cfg-ota-token-s4.png
    :align: center
    :alt: Create Secret
    :figclass: align-center

    Create Secret

  All the possible secret names that need to be configured are as follows:

    ::

      AT_OTA_TOKEN_WROOM32
      AT_OTA_TOKEN_WROVER32
      AT_OTA_TOKEN_ESP32_MINI_1
      AT_OTA_TOKEN_ESP32_PICO_D4
      AT_OTA_TOKEN_ESP32_SOLO_1
      AT_OTA_TOKEN_ESP32C3_MINI
      ESP32C2_2MB_TOKEN
      ESP32C2_4MB_TOKEN
      ESP32C6_4MB_TOKEN

If you need to support AT firmware upgrades for more modules, please repeat the above steps and add different keys again. After creating all the secrets, you will have the following page.

  .. figure:: ../../_static/web_compile/cfg-ota-token-s5.png
    :align: center
    :alt: Create Secrets Completed
    :figclass: align-center

    Create Secrets Completed

.. _at-wc-commit-changes:

Step 5. Use the github.dev editor to modify and submit the code
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For how to use the github.dev editor, please refer to the official `github.dev document <https://docs.github.com/en/codespaces/the-githubdev-web-based-editor>`_. The example process is shown below.

5.1 Open the github.dev editor
-------------------------------

Go back to the repository homepage and select the branch name that you want to modify.

  .. figure:: ../../_static/web_compile/commit-change-s1.png
    :align: center
    :alt: Select Branch
    :figclass: align-center

    Select Branch

Press the ``.`` key on the keyboard to open the branch code using the github.dev editor.

  .. figure:: ../../_static/web_compile/commit-change-s2.png
    :align: center
    :alt: Open Branch Code
    :figclass: align-center

    Open Branch Code

5.2 Create a new branch
-----------------------

At the bottom of the editor, click the branch name in the status bar. In the dropdown, enter the name for a new branch and click ``Create new branch``.

  .. figure:: ../../_static/web_compile/commit-change-s3.png
    :align: center
    :alt: Create New Branch
    :figclass: align-center
    :scale: 70%

    Create New Branch (Click to Enlarge)

Click ``Switch to Branch`` to create the branch.

  .. figure:: ../../_static/web_compile/commit-change-s4.png
    :align: center
    :alt: Confirm Branch Creation
    :figclass: align-center

    Confirm Branch Creation

The branch has been created.

  .. figure:: ../../_static/web_compile/commit-change-s5.png
    :align: center
    :alt: Branch Created
    :figclass: align-center

    Branch Created

5.3 Commit changes
---------------------

Modify the code in the github.dev editor. For example, to enable :doc:`WebSocket functionality <../AT_Command_Set/websocket_at_commands>` and disable :ref:`mDNS functionality <cmd-MDNS>`, open the configuration file ``esp-at/module_config/<your_module_name>/sdkconfig.defaults`` and add the following lines:

  ::

    CONFIG_AT_WS_COMMAND_SUPPORT=y
    CONFIG_AT_MDNS_COMMAND_SUPPORT=n

  .. figure:: ../../_static/web_compile/commit-change-s6.png
    :align: center
    :alt: Add WebSocket Command Support and Disable mDNS Functionality
    :figclass: align-center
    :scale: 70%

    Add WebSocket Command Support and Disable mDNS Functionality (Click to Enlarge)

In the activity bar, click the ``Source Control`` view. Click the plus sign ``+`` next to the file that has been modified to stage the changes.

  .. figure:: ../../_static/web_compile/commit-change-s7.png
    :align: center
    :alt: Stage Changes
    :figclass: align-center
    :scale: 70%

    Stage Changes (Click to Enlarge)

Enter a commit message to describe the changes you have made. Click ``Commit & Push`` to submit the changes.

  .. figure:: ../../_static/web_compile/commit-change-s8.png
    :align: center
    :alt: Submit Changes
    :figclass: align-center
    :scale: 70%

    Submit Changes (Click to Enlarge)

.. _at-wc-action-compile:

Step 6. Compile the AT firmware using GitHub Actions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

After you complete the above steps, GitHub Actions will automatically trigger the compilation of your ESP-AT firmware. After compilation, please refer to :doc:`How to Download the Latest Temporary Version of AT Firmware from GitHub <How_to_download_the_latest_temporary_version_of_AT_from_github>` to download the AT firmware you need. Note that the firmware is generated based on your own esp-at project, not the https://github.com/espressif/esp-at project.
