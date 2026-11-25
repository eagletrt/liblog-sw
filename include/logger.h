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
#include <stddef.h>

#define LOGGER_DISABLE_ALL (0b0000U)  /*!< Disable all logging levels. */
#define LOGGER_ENABLE_ALL (0b1111U)   /*!< Enable all logging levels. */

#ifndef LOGGER_EMIT_BUFFER_SIZE 
    #define LOGGER_EMIT_BUFFER_SIZE (64U) /*!< The emit buffer capacity. */
#endif /*! LOGGER_EMIT_BUFFER_SIZE */

/*!
 * \brief           Writes a formatted message through the logger.
 *
 * \param[in]       msg Logging message string.
 * \param[in]       size \p msg buffer size.
 *
 * \return          The number of bytes written on success, a negative value otherwise.
 *
 * \note            Remember that the logger has no clue about the underlyng
 *                  implementation. If you use files or any other type of
 *                  output, manage it accordingly in the
 *                  \ref logger_flush_callback implementation.
 */
typedef int (*logger_write_callback)(const char *msg, size_t size);

/*!
 * \brief           Flushes pending logger output.
 *
 * \return          0 on success, a nonzero value otherwise.
 *
 * \note            Remember that the logger has no clue about the underlyng
 *                  implementation. If you use files or any other type of
 *                  output, manage it accordingly in the
 *                  \ref logger_write_callback implementation.
 */
typedef int (*logger_flush_callback)(void);

/*!
 * \brief           Enumeration with all possible return code of the library.
 */
enum LoggerReturnCode {
    LOGGER_RC_OK,                 /*!< Everything is fine. */
    LOGGER_RC_NULL_POINTER,       /*!< Unexpected \c NULL pointer detected. */
    LOGGER_RC_BUFFER_SIZE_ERROR,  /*!< Buffer size limit exeeded. */
    LOGGER_RC_TRANSMISSION_ERROR, /*!< Transmission failed. */
    LOGGER_RC_INVALID_ARGUMENT,   /*!< Invalid argument passed to a function. */
};

// clang-format off

/*!
 * \brief           Logging levels represented as bit flags.
 *
 * \note            Individual levels can be combied with bitwise OR to select
 *                  multiple logging levels, for example:
 * \code
 * // Enables informational, error and debugging messages.
 * uint8_t verbosity = LOGGER_LEVEL_INFO | LOGGER_LEVEL_ERROR | LOGGER_LEVEL_DEBUG;
 * // Disables debugging messages
 * verbosity ^= LOGGER_LEVEL_DEBUG;
 * \endcode
 */
enum LoggerLevel {
    LOGGER_LEVEL_INFO    = 0b0001U, /*!< Informational messages. */
    LOGGER_LEVEL_ERROR   = 0b0010U, /*!< Error messages. */
    LOGGER_LEVEL_WARNING = 0b0100U, /*!< Warning messages. */
    LOGGER_LEVEL_DEBUG   = 0b1000U, /*!< Debugging messages. */
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
