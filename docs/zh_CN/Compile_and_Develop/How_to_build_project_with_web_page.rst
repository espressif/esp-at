网页编译 ESP-AT 工程
=============================

:link_to_translation:`en:[English]`

本文档详细介绍了如何通过网页编译 ESP-AT 工程。当默认的 :doc:`官方发布的固件 <../AT_Binary_Lists/index>` 无法满足需求时，如您需要开启 :doc:`WebSocket 功能 <../AT_Command_Set/websocket_at_commands>` 并关闭 :ref:`mDNS 功能 <cmd-MDNS>`，那么就需要编译 ESP-AT 工程。通常推荐您 :doc:`本地搭建环境编译 ESP-AT 工程 <How_to_clone_project_and_compile_it>`，但如果您本地搭建环境编译有困难，则请参考本文档，通过网页编译 ESP-AT 工程。

.. attention::
  **网页编译的 AT 固件，需要您根据自己的产品自行测试验证功能。**

  **请保存好下载的固件以及下载链接**，用于后续可能的问题调试。

详细步骤
^^^^^^^^^^^^^

请根据下方详细步骤，完成 ESP-AT 工程的 Fork、环境配置、代码修改以及编译。

* :ref:`at-wc-github-login`
* :ref:`at-wc-project-fork`
* :ref:`at-wc-enable-github-action`
* :ref:`at-wc-config-secrets`
* :ref:`at-wc-commit-changes`
* :ref:`at-wc-action-compile`

.. _at-wc-github-login:

第一步：登录您的 GitHub 账号
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在开始之前，请先 `登录您的 GitHub 账号 <https://github.com/login>`_，因为编译和下载固件需要登录权限。

.. _at-wc-project-fork:

第二步：Fork ESP-AT 工程
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

访问 `ESP-AT 仓库 <https://github.com/espressif/esp-at>`_，点击页面右上角的 ``Fork``。

  .. figure:: ../../_static/web_compile/fork-s1.png
    :align: center
    :alt: 点击 Fork
    :figclass: align-center

    点击 Fork

点击 ``Create fork`` 复制仓库到自己的 GitHub 账号下。默认的 Fork 仅拷贝 master 分支。如果您需要基于指定的分支修改代码，请取消勾选 ``Copy the master branch only``。

  .. figure:: ../../_static/web_compile/fork-s2.png
    :align: center
    :alt: 创建 Fork
    :figclass: align-center

    创建 Fork

.. _at-wc-enable-github-action:

第三步：开启 GitHub Actions 功能
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

点击仓库下的 Actions 进入 GitHub Actions 页面。

  .. figure:: ../../_static/web_compile/github-action-enable-s1.png
    :align: center
    :alt: 点击 Actions
    :figclass: align-center

    点击 Actions

点击 ``I understand my workflows, go ahead and enable them`` 启用 GitHub Actions。

  .. figure:: ../../_static/web_compile/github-action-enable-s2.png
    :align: center
    :alt: 启用 GitHub Actions
    :figclass: align-center

    启用 GitHub Actions

启用 GitHub Actions 完成。

  .. figure:: ../../_static/web_compile/github-action-enable-s3.png
    :align: center
    :alt: 启用 GitHub Actions 完成
    :figclass: align-center

    启用 GitHub Actions 完成

.. _at-wc-config-secrets:

第四步：配置编译 ESP-AT 工程所需的密钥
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

如果您有 `Espressif OTA server <http://iot.espressif.cn>`_ 的账户和 OTA token，并且需要使用 :ref:`AT+CIUPDATE <cmd-UPDATE>` 命令升级 AT 固件，那么需要完成此步骤。否则，建议您禁用 CONFIG_AT_OTA_SUPPORT（有关更多详细信息，请参阅 :ref:`at-wc-commit-changes`）或者跳过此步骤。

点击仓库下的 ``Settings`` 进入设置页面。

  .. figure:: ../../_static/web_compile/cfg-ota-token-s1.png
    :align: center
    :alt: 点击 Settings
    :figclass: align-center

    点击 Settings

点击 ``Settings`` -> ``Secrets and variables`` -> ``Actions`` 进入 Action 配置页面。

  .. figure:: ../../_static/web_compile/cfg-ota-token-s2.png
    :align: center
    :alt: 进入 Actions 配置页面
    :figclass: align-center

    进入 Actions 配置页面

点击 ``New repository secret`` 进入密钥创建页面。

  .. figure:: ../../_static/web_compile/cfg-ota-token-s3.png
    :align: center
    :alt: 创建密钥页面
    :figclass: align-center

    创建密钥页面

输入 Name 和 Secrets，点击 ``Add secret`` 创建一个新的密钥。此处的密钥即是 OTA token。

  .. figure:: ../../_static/web_compile/cfg-ota-token-s4.png
    :align: center
    :alt: 创建密钥页面
    :figclass: align-center

    创建密钥

  需要配置的可能的密钥名称如下：

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

如果需要更多的模组支持 AT 固件升级，请重复上面步骤，再次添加不同的密钥，所有密钥创建完成的页面如下。

  .. figure:: ../../_static/web_compile/cfg-ota-token-s5.png
    :align: center
    :alt: 创建密钥完成
    :figclass: align-center

    创建密钥完成

.. _at-wc-commit-changes:

第五步：使用 github.dev 编辑器修改和提交代码
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

第五步也可以参考 `github.dev 官方文档 <https://docs.github.com/zh/codespaces/the-githubdev-web-based-editor#%E6%89%93%E5%BC%80-githubdev-%E7%BC%96%E8%BE%91%E5%99%A8>`_，下面是示例过程。

5.1 打开 github.dev 编辑器
---------------------------------

回到仓库主页，选择要修改的分支名称。

  .. figure:: ../../_static/web_compile/commit-change-s1.png
    :align: center
    :alt: 选择分支
    :figclass: align-center

    选择分支

按键盘上点号 ``.``，使用 github.dev 编辑器打开分支代码。

  .. figure:: ../../_static/web_compile/commit-change-s2.png
    :align: center
    :alt: 打开分支代码
    :figclass: align-center

    打开分支代码

5.2 创建新分支
--------------------

在编辑器的底部，单击状态栏中的分支名称，在下拉菜单中，单击要切换到的分支或输入新分支的名称，然后单击 ``创建新分支``。

  .. figure:: ../../_static/web_compile/commit-change-s3.png
    :align: center
    :alt: 创建新分支
    :figclass: align-center
    :scale: 70%

    创建新分支（点击放大）

单击 ``Switch to Branch``，创建分支。

  .. figure:: ../../_static/web_compile/commit-change-s4.png
    :align: center
    :alt: 确认创建分支
    :figclass: align-center

    确认创建分支

分支创建完成。

  .. figure:: ../../_static/web_compile/commit-change-s5.png
    :align: center
    :alt: 分支创建完成
    :figclass: align-center

    分支创建完成

5.3 提交更改
---------------------

在 github.dev 编辑器中修改代码。例如 :doc:`开启 WebSocket 功能 <../AT_Command_Set/websocket_at_commands>` 并关闭 :ref:`mDNS 功能 <cmd-MDNS>`，则需要打开模组的配置文件 ``esp-at/module_config/<your_module_name>/sdkconfig.defaults`` 文件，增加如下行：

  ::

    CONFIG_AT_WS_COMMAND_SUPPORT=y
    CONFIG_AT_MDNS_COMMAND_SUPPORT=n

  .. figure:: ../../_static/web_compile/commit-change-s6.png
    :align: center
    :alt: 增加 WebSocket 命令支持，禁用 mDNS 功能
    :figclass: align-center
    :scale: 70%

    增加 WebSocket 命令支持，禁用 mDNS 功能（点击放大）

在活动栏中，单击 ``源代码管理`` 视图。单击已更改文件旁边的加号 ``+`` 暂存修改。

  .. figure:: ../../_static/web_compile/commit-change-s7.png
    :align: center
    :alt: 暂存修改
    :figclass: align-center
    :scale: 70%

    暂存修改（点击放大）

输入提交消息，描述您所做的更改。点击 ``Commit & Push`` 提交。

  .. figure:: ../../_static/web_compile/commit-change-s8.png
    :align: center
    :alt: 提交修改
    :figclass: align-center
    :scale: 70%

    提交修改（点击放大）

.. _at-wc-action-compile:

第六步：GitHub Actions 编译 AT 固件
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

上述步骤完成之后，会自动触发 GitHub Actions 编译您的 ESP-AT 固件。请参考 :doc:`如何从 GitHub 下载最新临时版本 AT 固件 <How_to_download_the_latest_temporary_version_of_AT_from_github>` 文档，下载您所需要的 AT 固件（注意：文档里的步骤都是基于您自己账号下的 esp-at 仓库进行的，而不是 https://github.com/espressif/esp-at 仓库）。
