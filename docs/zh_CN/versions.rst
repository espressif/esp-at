ESP-AT 版本简介
================

:link_to_translation:`en:[English]`

本文档主要介绍了 ESP-AT 的版本、如何选择版本、版本管理、支持期限、查看版本、和订阅 AT 发布等内容。

发布版本
---------

ESP-AT 在 GitHub 平台上的完整发布历史请见 `发布说明页面 <https://github.com/espressif/esp-at/releases>`_。您可以在该页面查看各个版本的 AT 固件、发布说明、配套文档及相应获取方式。

我该选择哪个版本？
----------------------

请阅读 :ref:`AT 软件方案选型 <at-solution-selection>`。

.. _versioning-scheme:

版本管理
-----------------

ESP-AT 发布固件管理
^^^^^^^^^^^^^^^^^^^^^^^^

ESP-AT 发布是针对芯片的，通常是指发布一个或者几个芯片的 AT 固件。ESP-AT :doc:`发布的 AT 固件 <AT_Binary_Lists/esp_at_binaries>` 所支持的 AT 命令集通常是向后兼容的。这意味着，您可以将新版本的 AT 固件更新至旧版本的设备中。

ESP-AT 发布的固件采用了和 `语义版本管理方法 <https://semver.org/lang/zh-CN/>`_ 类似的方式，即您可以从字面含义理解每个版本的差异。例如 ``v3.3.0.0``， ``v`` 代表版本，其后为版本号，版本号的格式如下：

::

    <major>.<minor>.<patch>.<custom>

其中：

- <major> 为主要版本。例如 ``v4.0.0.0`` 代表有重大更新，通常包括引入新的芯片支持、新特性、和问题修复。
- <minor> 为次要版本。例如 ``v3.3.0.0`` 代表有较大更新，通常包括新增特性、ESP-IDF 版本升级、和问题修复。
- <patch> 为修复版本，也叫 bugfix 版本。例如 ``v2.4.2.0`` 代表仅修复了一些问题，并不增加任何新特性。
- <custom> 为自定义版本。通常用于下游代理商、或定制项目的版本。

ESP-AT 发布分支管理
^^^^^^^^^^^^^^^^^^^^^^^^

ESP-AT 发布 AT 固件的同时，如果有主要版本或者次要版本的更新，会创建一个新的 `发布分支 <https://github.com/espressif/esp-at/branches/all?query=release>`_。

ESP-AT 发布的分支采用了和 `语义版本管理方法 <https://semver.org/lang/zh-CN/>`_ 类似的方式，即您可以从字面含义理解每个版本的差异。例如 ``release/v3.3.0.0``， ``release`` 代表着发布的分支， ``v`` 代表着发布分支的版本，其后为版本号，版本号的格式如下：

::

    <major>.<minor>.0.0

其中：

- <major> 为主要版本。例如 ``release/v4.0.0.0`` 代表有重大更新，通常包括引入新的芯片支持、新特性、和问题修复。
- <minor> 为次要版本。例如 ``release/v3.3.0.0`` 代表有较大更新，通常包括新增特性、ESP-IDF 版本升级、和问题修复。

通常情况下，待发布的 AT 固件会在发布分支上进行多轮测试，直到没有重大问题后，会发布 AT 固件并同步发布分支到 GitHub。

- 新的特性开发通常会在 master 分支上进行，不会合并到发布分支上。
- 问题修复通常会在 master 分支上进行，如果问题严重，会合并到发布分支上。

支持期限
---------------

ESP-AT 的每个主要版本和次要版本都有相应的支持期限。支持期限满后，版本停止更新维护，将不再提供支持。由于 ESP-AT 是基于 ESP-IDF 开发的项目，因此 ESP-AT 的支持期限是受限于 ESP-IDF 的支持期限。当前 ESP-AT 各版本信息如下：

.. list-table::
   :header-rows: 1
   :width: 100%

   * - AT 版本（发布时间）
     - 发布固件适用芯片
     - IDF 版本
     - IDF 支持期限截止
     - AT 新版本迭代计划
   * - **v4.0.0.0** (2023.12.29)
     - ESP32-C6
     - `v5.1.2 <https://github.com/espressif/esp-idf/releases/tag/v5.1.2>`_
     - 2025.12.30
     - 2025.7.30 - 2025.9.30
   * - **v3.4.0.0** (2024.6.7)
     -
       * ESP32
       * ESP32-S2
     - `v5.0.6 <https://github.com/espressif/esp-idf/releases/tag/v5.0.6>`_
     - 2025.5.29
     - 2024.12.29 - 2025.2.28
   * - **v3.3.0.0** (2024.5.9)
     -
       * ESP32-C2
       * ESP32-C3
     - `v5.0.6 <https://github.com/espressif/esp-idf/releases/tag/v5.0.6>`_
     - 2025.5.29
     - 2024.12.29 - 2025.2.28
   * - **v3.2.0.0** (2023.7.28)
     -
       * ESP32
       * ESP32-C3
     - ~v5.0.3 (`24b9d38 <https://github.com/espressif/esp-idf/commit/24b9d38>`_)
     - 2025.5.29
     -
       * ESP32 已发布新版本 v3.4.0.0
       * ESP32-C3 已发布新版本 v3.3.0.0

**ESP-AT 各发布版本支持期限如下：**

.. figure:: ../_static/at-support-periods.png
   :align: center
   :alt: ESP-AT 版本支持期限
   :figclass: align-center

   ESP-AT 版本支持期限

- 支持期限的服务期

    通常为从 AT 发布该芯片的 AT 固件开始，到计划发布该芯片下一个 AT 版本为止。下一个 AT 版本的发布时间通常在该芯片有重大问题需要修复，或者对应的 `ESP-IDF 支持期限 <https://github.com/espressif/esp-idf/blob/master/README_CN.md#esp-idf-%E7%89%88%E6%9C%AC%E6%94%AF%E6%8C%81%E6%9C%9F%E9%99%90>`_ 结束前几个月（AT 发布说明中有介绍该芯片对应的 ESP-IDF 版本）。

- 支持期限的维护期

    通常为从服务期结束后，到该芯片对应的 `ESP-IDF 支持期限 <https://github.com/espressif/esp-idf/blob/master/README_CN.md#esp-idf-%E7%89%88%E6%9C%AC%E6%94%AF%E6%8C%81%E6%9C%9F%E9%99%90>`_ 结束（AT 发布说明中有介绍该芯片对应的 ESP-IDF 版本）。例如，ESP-IDF v5.0 的支持期限到 2025 年 5 月 29 日，那么 ESP-AT v3.0 ~ v3.3 的维护期限也到 2025 年 5 月 29 日。

一般而言：

- 一旦 AT 发布新的版本，则旧版本的支持期限的服务期结束，进入支持期限的维护期。

    例如，AT 发布了 v3.3.0.0 版本（针对 ESP32-C2 和 ESP32-C3 芯片），那么 ESP32-C3 的 v3.2.0.0 版本的支持期限的服务期结束，进入支持期限的维护期；ESP32-C2 的 v3.1.0.0 版本的支持期限的服务期结束，进入支持期限的维护期。

- 如您有 GitHub 账号，请 :ref:`sub-at-release` ，GitHub 将会在新版本发布的时候通知您。当您所使用的 AT 固件有 Bugfix 版本发布时，请做好升级至该 Bugfix 版本的规划。
- 请确保您所使用的版本停止更新维护前，已做好升级至新版本的规划。
- 在支持期限内意味着 ESP-AT 团队将继续在 GitHub 的发布分支上进行重要 bug 修复、安全修复等，并根据需要定期发布新的 Bugfix 版本。

查看当前 AT 固件版本
----------------------------

请发送 :ref:`AT+GMR <cmd-GMR>` 命令查看 AT 固件版本信息，参考 :ref:`AT+GMR <cmd-GMR>` 命令下的参数说明了解更多信息。

.. _sub-at-release:

订阅 AT 版本发布
--------------------

* :ref:`at-sr-github-login`
* :ref:`at-sr-watch-custom`
* :ref:`at-sr-release-apply`

.. _at-sr-github-login:

第一步：登录您的 GitHub 账号
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

在开始之前，请先 `登录您的 GitHub 账号 <https://github.com/login>`_，因为订阅发布需要登录权限。

.. _at-sr-watch-custom:

第二步：选择定制化的通知
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

访问 `ESP-AT 仓库 <https://github.com/espressif/esp-at>`_，点击页面右上角的 ``Watch``，再点击 ``Custom``。

  .. figure:: ../_static/at-sub-release-custom.png
    :align: center
    :alt:
    :figclass: align-center
    :scale: 70%

    定制化通知（点击放大）

.. _at-sr-release-apply:

第三步：定制发布应用
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

勾选 ``Releases`` 并点击 ``Apply``。

  .. figure:: ../_static/at-sub-release-apply.png
    :align: center
    :alt:
    :figclass: align-center
    :scale: 70%

    定制发布应用（点击放大）

这样就完成了订阅 AT 发布的操作。当有新的 AT 版本发布时，您将会收到 GitHub 的通知。
