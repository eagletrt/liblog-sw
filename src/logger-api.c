/*!
 * \file            logger-api.c
 * \date            2026-11-15
 * \authors         Mirko Lana [mirko.lana@eagletrt.it]
 *
 * \brief           Liblog APIs implementation.
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "logger.h"
#include "logger-api.h"

#include "eagletrt.h"

EAGLETRT_STATIC struct LoggerHandler logger_handler;

/*!
 * \brief           Dummmy flush function.
 */
EAGLETRT_STATIC int prv_logger_api_dummy_flush(void) {
    return 0;
}

/*!
 * \brief           Check if the given level bitmask is valid or not.
 *
 * \details         The function checks if it's a power of two and less than or
 *                  equal \ref LOGGER_LEVEL_DEBUG.
 *
 * \param[in]       level Bitmask representing the level.
 *
 * \return          True if the level bitmask is valid, false otherwise.
 */
EAGLETRT_STATIC_INLINE bool prv_logger_api_level_is_valid(enum LoggerLevel level) {
    return ((level & (level - 1)) == 0U) && (level <= LOGGER_LEVEL_DEBUG);
}

/*!
 * \brief           Convert a level bitmask into the corresponding index.
 *
 * \details         The function decrements by 1 the bitmask, then counts the
 *                  set bits in the result using parallel bit-counting steps.
 *
 * \param[in]       level Bitmask representing the level.
 *
 * \return          The corresponding index.
 *
 * \par             References:
 * \htmlonly
 * <ul>
 *   <li>
 *     <a href="https://stackoverflow.com/a/71539401">
 *       https://stackoverflow.com/a/71539401
 *     </a>
 *   </li>
 *   <li>
 *     <a href="https://graphics.stanford.edu/%7Eseander/bithacks.html#CountBitsSetParallel">
 *       https://graphics.stanford.edu/%7Eseander/bithacks.html#CountBitsSetParallel
 *     </a>
 *   </li>
 * </ul>
 * \endhtmlonly
 *
 * \note            I personally think that using this type of solution is
 *                  better than using a switch-case statement or replacing it
 *                  entirely with levels used as indexes. Level bitmasks gives
 *                  us a flexible way to manage the logs filtering by enabling
 *                  and disabling specific verobosity levels when necessary.
 *                  ~Mirko
 */
EAGLETRT_STATIC_INLINE uint8_t prv_logger_api_bit_index(enum LoggerLevel level) {
    uint8_t bitmask = level - 1;
    bitmask = bitmask - ((bitmask >> 1) & 0x55U);
    return (bitmask & 0x33U) + ((bitmask >> 2) & 0x33U);
}

enum LoggerReturnCode logger_api_init(
    logger_write_callback write,
    logger_flush_callback flush,
    uint8_t verbosity) {
    if (write == NULL) {
        return LOGGER_RC_NULL_POINTER;
    }

    logger_handler.write = write;
    logger_handler.flush = flush == NULL ? prv_logger_api_dummy_flush : flush;
    logger_handler.verbosity = verbosity;

    return LOGGER_RC_OK;
}

void logger_api_set_verbosity(uint8_t verbosity) {
    logger_handler.verbosity = verbosity;
}

enum LoggerReturnCode logger_api_emit(
    enum LoggerLevel level,
    const char *tag,
    const char *fmt,
    ...) {
    static const char *const MESSAGE_HEADERS[] = {
        "INFO",
        "ERROR",
        "WARN",
        "DEBUG",
    };
    static char emit_buffer[LOGGER_EMIT_BUFFER_SIZE];

    if (logger_handler.write == NULL || tag == NULL || fmt == NULL) {
        return LOGGER_RC_NULL_POINTER;
    }

    if (!prv_logger_api_level_is_valid(level) || tag[0] == '\0') {
        return LOGGER_RC_INVALID_ARGUMENT;
    }

    if (!(logger_handler.verbosity & level)) {
        return LOGGER_RC_OK;
    }

    memset(emit_buffer, 0, LOGGER_EMIT_BUFFER_SIZE);
    const int offset = snprintf(
        emit_buffer,
        LOGGER_EMIT_BUFFER_SIZE,
        "[%-5s] @%s",
        MESSAGE_HEADERS[prv_logger_api_bit_index(level)],
        tag);
    if (offset < 0 || offset >= (const int)LOGGER_EMIT_BUFFER_SIZE) {
        return LOGGER_RC_BUFFER_SIZE_ERROR;
    }

    va_list args;
    va_start(args, fmt);

    int result = vsnprintf(
        emit_buffer + offset,
        LOGGER_EMIT_BUFFER_SIZE,
        fmt,
        args);
    if (result < 0) {
        return LOGGER_RC_TRANSMISSION_ERROR;
    }

    va_end(args);

    result = logger_handler.write(emit_buffer, offset + result);
    return (result >= 0 && !logger_handler.flush())
               ? LOGGER_RC_OK
               : LOGGER_RC_TRANSMISSION_ERROR;
}
