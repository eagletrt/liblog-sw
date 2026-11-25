# LIBLOG

This library implements a simple and easy-to-use logging system, suitable for
(almost) all platforms featuring a C-lang compiler.

## Usage

There are two main ways to use `liblog`:

1. By configuring its internal private handler
   ```c
   #include <stddef.h>
   #include <stdint.h>

   #include "logger.h"
   #include "logger-api.h"

     #define TAG "MAIN"

   // Maps to the platform's UART transmit routine
   static int uart_write(const char *msg, size_t size) {
       return (int)uart_send(msg, size); // e.g. HAL_UART_Transmit
   }

   int main(void) {
       logger_api_init(NULL, uart_write, NULL, LOGGER_ENABLE_ALL);

       LOGGER_API_INFO(NULL, TAG, "system boot");
       LOGGER_API_WARNING(NULL, TAG, "low battery voltage: %dmV", 3200);
       LOGGER_API_ERROR(NULL, TAG, "sensor %d unreachable", 2);

       while (true) {
           LOGGER_API_DEBUG(NULL, TAG, "heartbeat");
       }
   }
   ```
1. By configuring per peripheral handlers:
   ```c
   #include <stdio.h>
   #include <stddef.h>

   #include "logger.h"
   #include "logger-api.h"

   #define TAG "MAIN"

   static FILE *prv_log_file = NULL;
   static struct LoggerHandler prv_uart_handler = { 0 };
   static struct LoggerHandler prv_file_handler = { 0 };

   // Maps to the platform's UART transmit routine
   static int prv_uart_write(const char *msg, size_t size) {
       return uart_send(msg, size);
   }

   static int prv_file_write(const char *msg, size_t size) {
       return prv_log_file == NULL ? -1 : (int)fwrite(msg, 1U, size, prv_log_file);
   }

   static int prv_file_flush(void) {
       return prv_log_file == NULL ? -1 : fflush(prv_log_file);
   }

   int main(void) {
       prv_log_file = fopen("system.log", "a");
       if (prv_log_file == NULL) {
           return 1;
       }

       logger_api_init(&prv_uart_handler, prv_uart_write, NULL, LOGGER_ENABLE_ALL);
       logger_api_init(&prv_file_handler, prv_file_write, prv_file_flush, LOGGER_ENABLE_ALL);

       LOGGER_API_INFO(&prv_uart_handler, TAG, "system boot");
       LOGGER_API_WARNING(&prv_uart_handler, TAG, "battery low voltange: %dmV", 3200);

       LOGGER_API_INFO(&prv_file_handler, TAG, "system boot");
       LOGGER_API_ERROR(&prv_file_handler, TAG, "sensor %d unreachable", 2);

    		while (true) {
       LOGGER_API_DEBUG(&prv_uart_handler, TAG, "heartbeat");
    		}

       fclose(prv_log_file);
       prv_log_file = NULL;

       return 0;
   }
   ```

### Verbosity

Every log level is a bitmask. To enable a specific log level you have to set its
corrispoding bit to 1 and pass it to the handler by using the
`logger_api_set_verbosity` function. The verbosity level can be composed by
using the `|`/`^` operator or by using pre-defined `LOGGER_ENBALE_ALL` and
`LOGGER_DISABLE_ALL` constants.

For example, if you want to disable warning logs, you can use the `^` operator
with `LOGGER_ENABLE_ALL`:

```c
int main(void) {
	// ...

	uint8_t verbosity = LOGGER_ENABLE_ALL ^ LOGGER_LEVEL_WARNING;
	logger_api_set_verbosity(NULL, verbosity)

	// ...
}
```

Or, if you want to enable only informational and error logs, you can do as
follows:

```c
int main(void) {
	// ...

	uint8_t verbosity = LOGGER_LEVEL_INFO | LOGGER_LEVEL_ERROR;
	logger_api_set_verbosity(NULL, verbosity)

	// ...
}
```

## Acknowledgements

Thanks to:

- [chqrlie](https://stackoverflow.com/a/71539401) and
  [Sean Eron Anderson](https://graphics.stanford.edu/%7Eseander/bithacks.html#CountBitsSetParallel)
  for the algorithms that inspired the `prv_logger_api_bit_index`
  implementation.

## License

This libary is licensed under the
[GNU Affero General Public License v3.0](LICENSE).
