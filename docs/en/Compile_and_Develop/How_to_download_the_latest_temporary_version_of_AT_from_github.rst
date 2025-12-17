How to Download the Latest Temporary Version of AT Firmware from GitHub
=========================================================================

:link_to_translation:`zh_CN:[中文]`

As ESP-AT enables CI (Continuous Integration) on GitHub, temporary versions of ESP-AT firmware is generated every time when the code is pushed to GitHub.

.. attention::
  **The latest temporary version of AT firmware you download is for testing purposes only. For mass production, please make sure to conduct comprehensive function verification and testing based on the actual application scenarios of your product.** In addition, the current firmware does not support the official OTA function (:ref:`AT+CIUPDATE <cmd-UPDATE>`).

  **Please save the firmware and download link**, for possible issue debugging in the future.

The following steps guide you on how to download the latest temporary version of AT firmware from GitHub.

#. Sign in your GitHub account

   Before you start, **please sign in your GitHub account**, as you need login permission to download firmware. 

#. Open the website https://github.com/espressif/esp-at

   .. figure:: ../../_static/download_temp_version/download-temp-version-github.png
    :align: center
    :figclass: align-center
    :alt: ESP-AT GitHub Home Page

    ESP-AT GitHub Home Page

#. Click "Actions" to enter the "Actions" page

   .. figure:: ../../_static/download_temp_version/download-temp-version-click-actions.png
    :align: center
    :figclass: align-center
    :alt: Click Actions

    Click Actions

   .. figure:: ../../_static/download_temp_version/download-temp-version-actions-page.png
    :align: center
    :figclass: align-center
    :alt: Actions Page

    Actions Page

#. Click the branch and choose the specified branch

   Default branch is master. If you want to download the temporary firmware of the specified branch, click "Branch" to enter the workflow page of the specified branch.

   .. figure:: ../../_static/download_temp_version/download-temp-version-click-branch-workflow.png
    :align: center
    :figclass: align-center
    :alt: Click the Branch

    Click the Branch

#. Click the latest workflow and enter the latest workflow page

   .. figure:: ../../_static/download_temp_version/download-temp-version-click-workflow.png
    :align: center
    :figclass: align-center
    :alt: Click the Latest Workflow

    Click the Latest Workflow

   .. figure:: ../../_static/download_temp_version/download-temp-version-workflow.png
    :align: center
    :figclass: align-center
    :alt: Latest Workflow Page

    Latest Workflow Page

#. Scroll the page to the end, select the corresponding module on the ``Artifacts`` page

   .. figure:: ../../_static/download_temp_version/download-temp-version-artifacts.png
    :align: center
    :figclass: align-center
    :alt: Artifacts Page

    Artifacts Page

#. Click to download the temporary version of AT firmware for modules

   .. note::
    If you do not find the firmware of the corresponding module on the ``Artifacts`` page, you can refer to :doc:`esp-at_firmware_differences` to select a similar firmware to download.
