如何从 GitHub 下载最新临时版本 AT 固件
=========================================================================

:link_to_translation:`en:[English]`

由于 ESP-AT 在 GitHub 上启用 CI（持续集成），因此每次代码被推送到 GitHub 都会生成 ESP-AT 固件的临时版本。

.. attention::
  **您所下载的最新临时版本 AT 固件仅用于测试目的。若需用于量产，请务必依据您产品的实际应用场景进行全面的功能验证与测试。** 此外，当前固件不支持官方的 OTA 功能 (:ref:`AT+CIUPDATE <cmd-UPDATE>`)。

  **请保存好下载的固件以及下载链接**，用于后续可能的问题调试。

以下步骤指导您如何从 GitHub 下载最新临时版本 AT 固件。

#. 登录您的 GitHub 账号

   在开始之前，**请先登录您的 GitHub 账号**，因为下载固件需要登录权限。

#. 打开网页 https://github.com/espressif/esp-at

   .. figure:: ../../_static/download_temp_version/download-temp-version-github.png
    :align: center
    :figclass: align-center
    :alt: ESP-AT GitHub 官方页面

    ESP-AT GitHub 官方页面

#. 点击 "Actions" 进入 "Actions" 页面

   .. figure:: ../../_static/download_temp_version/download-temp-version-click-actions.png
    :align: center
    :figclass: align-center
    :alt: 点击 Actions

    点击 Actions

   .. figure:: ../../_static/download_temp_version/download-temp-version-actions-page.png
    :align: center
    :figclass: align-center
    :alt: Actions 页面

    Actions 页面

#. 点击 "Branch" 选择指定分支

    默认为 master 分支。如果您想下载指定分支的临时固件，点击 "Branch" 进入指定分支的 workflow 页面。

    .. figure:: ../../_static/download_temp_version/download-temp-version-click-branch-workflow.png
      :align: center
      :figclass: align-center
      :alt: 点击 Branch

      点击 Branch

#. 点击最新的 workflow 进入 workflow 页面

   .. figure:: ../../_static/download_temp_version/download-temp-version-click-workflow.png
    :align: center
    :figclass: align-center
    :alt: 点击最新的 Workflow

    点击最新的 Workflow

   .. figure:: ../../_static/download_temp_version/download-temp-version-workflow.png
    :align: center
    :figclass: align-center
    :alt: 最新的 Workflow 页面

    最新的 Workflow 页面

#. 将页面滚动到最后，在 ``Artifacts`` 页面中选择相对应的模组

   .. figure:: ../../_static/download_temp_version/download-temp-version-artifacts.png
    :align: center
    :figclass: align-center
    :alt: Artifacts 页面

    Artifacts 页面

#. 点击下载模组的最新临时版本 AT 固件

   .. note::
    如果您在 ``Artifacts`` 页面中没有找到对应的模组，请参考 :doc:`esp-at_firmware_differences` 选择类似固件进行下载。
