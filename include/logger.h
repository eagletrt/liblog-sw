/*!
 * \file            logger.h
 * \date            2026-11-15
 * \authors         Mirko Lana [mirko.lana@eagletrt.it]
 *
 * \brief           Liblog data structures definitions.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <stdint.h>

/*!
 * \brief           Writes a formatted message through the logger.
 *
 * \param[in]       fmt Format string.
 *
 * \return          The number of bytes written on success, a negative value otherwise.
 *
 * \note            Remember that the logger has no clue about the underlyng implementation. If you use files or any
 *                  other type of output, manage it accordingly in the \ref logger_flush_callback implementation.
 */
typedef int (*logger_write_callback)(const char *fmt, ...);

/*!
 * \brief           Flushes pending logger output.
 *
 * \return          0 on success, a nonzero value otherwise.
 *
 * \note            Remember that the logger has no clue about the underlyng implementation. If you use files or any
 *                  other type of output, manage it accordingly in the \ref logger_write_callback implementation.
 */
typedef int (*logger_flush_callback)(void);

/*!
 * \brief           Enumeration with all possible return code of the library.
 */
enum LoggerReturnCode {
    LOGGER_RC_ERROR = -1,   /*!< An error occurred. */
    LOGGER_RC_OK,           /*!< Everything is fine. */
    LOGGER_RC_NULL_POINTER, /*!< Unexpected NULL pointer detected. */
    LOGGER_RC_INVALID_LEVEL /*!< Invalid logging level used. */
};

// clang-format off

/*!
 * \brief           Logging levels represented as bit flags.
 *
 * \note            Individual levels can be combied with bitwise OR to select multiple logging levels. Use
 *                  LOGGER_LEVEL_NONE to disable logging and LOGGER_LEVEL_ALL to enable all defined levels.
 */
enum LoggerLevel {
    LOGGER_LEVEL_NONE     = 0b0000, /*!< Disable all logging. */
    LOGGER_LEVEL_INFO     = 0b0001, /*!< Informational messages. */
    LOGGER_LEVEL_ERROR    = 0b0010, /*!< Error messages. */
    LOGGER_LEVEL_WARNING  = 0b0100, /*!< Warning messages. */
    LOGGER_LEVEL_DEBUG    = 0b1000, /*!< Debugging messages. */
    LOGGER_LEVEL_ALL      = 0b1111  /*!< Enable all defined logging levels. */
};

// clang-format on

/*!
 * \brief           Logger handler structure.
 */
struct LoggerHandler {
    logger_write_callback write; /*!< Write function callback. */
    logger_flush_callback flush; /*!< Flush function callback. */
    uint8_t verbosity;           /*!< Verbosity level, represented as a bit flag. */
};

#endif /*! LOGGER_H */
