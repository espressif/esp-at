# Examples

## Establishing Wi-Fi or Ethernet Connection

### About the `example_connect()` Function

Protocols examples use a simple helper function, `example_connect()`, to establish Wi-Fi or Ethernet connection. This function is implemented in [examples/common_components/protocol_examples/common/connect.c](../common_components/protocol_examples_common/connect.c), and has a very simple behavior: block until connection is established and IP address is obtained, then return. This function is used to reduce the amount of boilerplate and to keep the example code focused on the protocol or library being demonstrated.

The simple `example_connect()` function does not handle timeouts, does not gracefully handle various error conditions, and is only suited for use in examples. When developing real applications, this helper function needs to be replaced with full Wi-Fi / Ethernet connection handling code. Such code can be found in [examples/wifi/getting_started/](../wifi/getting_started) and [examples/ethernet/basic/](../ethernet/basic) examples.

### Configuring the Example

To configure the example to use Wi-Fi or Ethernet connection, open the project configuration menu (`idf.py menuconfig`) and navigate to "Example Connection Configuration" menu. Select either "Wi-Fi" or "Ethernet" in the "Connect using" choice.

When connecting using Wi-Fi, enter SSID and password of your Wi-Fi access point into the corresponding fields. If connecting to an open Wi-Fi network, keep the password field empty.

When connecting using Ethernet, set up PHY type and configuration in the provided fields. If using Ethernet for the first time, it is recommended to start with the [Ethernet example readme](../ethernet/basic/README.md), which contains instructions for connecting and configuring the PHY. Once Ethernet example obtains IP address successfully, proceed to the protocols example and set the same configuration options.

### Disabling IPv6

By default, `example_connect()` function waits until Wi-Fi or Ethernet connection is established, and IPv4 address and IPv6 link-local address are obtained. In network environments where IPv6 link-local address cannot be obtained, disable "Obtain IPv6 link-local address" option found in "Example Connection Configuration" menu.

### Building the example
- execute `idf.py build` or `make` in the terminal (inside project directory).

### Flash and monitor
- execute `idf.py flash monitor` or `make flash monitor` to flash the binary onto the chip and monitor the results

### Options (Debugging and more)
- Please refer README file in Parent directory esp-wolfssl
