# !/bin/bash

build_example () {
    local example_dir=$(dirname "$1")

    # jump into project root directry
    cd $example_dir

    touch sdkconfig.defaults
    cp sdkconfig.defaults sdkconfig.defaults.1

    # build exmaple with "libwolfssl_small_cert_verify.a"
    rm sdkconfig -rf
    make defconfig
    make -j4

    # build exmaple with "libwolfssl_small_cert_verify_debug.a"
    cp sdkconfig.defaults.1 sdkconfig.defaults
    echo "CONFIG_ESP_DEBUG_WOLFSSL=y\n" >> sdkconfig.defaults
    rm sdkconfig -rf
    make defconfig
    make -j4

    # build exmaple with "libwolfssl.a"
    cp sdkconfig.defaults.1 sdkconfig.defaults
    echo "CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY=" >> sdkconfig.defaults
    rm sdkconfig -rf
    make defconfig
    make -j4

    # build exmaple with "libwolfssl_debug.a"
    cp sdkconfig.defaults.1 sdkconfig.defaults
    echo "CONFIG_ESP_WOLFSSL_SMALL_CERT_VERIFY=" >> sdkconfig.defaults
    echo "CONFIG_ESP_DEBUG_WOLFSSL=y" >> sdkconfig.defaults
    rm sdkconfig -rf
    make defconfig
    make -j4

    cp sdkconfig.defaults.1 sdkconfig.defaults 
    rm sdkconfig.defaults.1
}

EXAMPLE_PATHS=$( find ${CI_PROJECT_DIR}/examples/ -type f -name Makefile | grep -v "/components/" | grep -v "/common_components/" | grep -v "/main/" | grep -v "/build_system/cmake/" | sort )

for FN in $EXAMPLE_PATHS
do
    build_example "$FN"
done
