# test\_uart\_echo

Test the uart write and read driver by sending and receiving data.

The pytest file is located under `src/ta/tests/test_uart_echo.py`.

To execute this test outside of the pytest framework two scripts are provided under `tools/`:
	- test\_uart\_serial.py -> uses serial device used for hardware
	- test\_uart\_tcp.py	-> uses tcp serial frontend for qemu virtual uart

For information how to run the scripts execute

```sh
./test_uart_serial.py --help
```
