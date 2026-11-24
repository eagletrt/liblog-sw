/*!
 * \file            logger-api.h
 * \date            2026-11-15
 * \authors         Mirko Lana [mirko.lana@eagletrt.it]
 *
 * \brief           Liblog APIs implementation.
 */

#ifndef LOGGER_API_H
#define LOGGER_API_H

#include <stdint.h>

#include "logger.h"

/*!
 * \defgroup        logger_api_macros Convenience macros that wrap \ref logger_api_emit.
 *
 * \note            The macros include the source line number in each log message using
 *                  \p __LINE__, making it easier to locate where the message originated.
 * \{
 */

/*!
 * \brief           Emit an information log message.
 *
 * \details         Emit an informational log message by using the passed
 *                  \c logger_handler. If \c logger_handler is \p NULL, the
 *                  internal default one will be used instead.
 *
 * \param[in]       logger_handler Logger handler structure.
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 */
#define LOGGER_API_INFO(logger_handler, tag, fmt, ...) \
    (int_logger_api_emit((logger_handler), LOGGER_LEVEL_INFO, (tag), ":%d >> " fmt "\n\r", __LINE__, ##__VA_ARGS__))

/*!
 * \brief           Emit an error log message.
 *
 * \details         Emit an error log message by using the passed
 *                  \c logger_handler. If \c logger_handler is \p NULL, the
 *                  internal default one will be used instead.
 *
 * \param[in]       logger_handler Logger handler structure.
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 */
#define LOGGER_API_ERROR(logger_handler, tag, fmt, ...) \
    (int_logger_api_emit((logger_handler), LOGGER_LEVEL_ERROR, (tag), ":%d >> " fmt "\n\r", __LINE__, ##__VA_ARGS__))

/*!
 * \brief           Emit a warning log message.
 *
 * \param[in]       logger_handler Logger handler structure.
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \details         Emit a warning log message by using the passed
 *                  \c logger_handler. If \c logger_handler is \p NULL, the
 *                  internal default one will be used instead.
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 */
#define LOGGER_API_WARNING(logger_handler, tag, fmt, ...) \
    (int_logger_api_emit((logger_handler), LOGGER_LEVEL_WARNING, (tag), ":%d >> " fmt "\n\r", __LINE__, ##__VA_ARGS__))

/*!
 * \brief           Emit a debugging log message.
 *
 * \details         Emit a debugging log message by using the passed
 *                  \c logger_handler. If \c logger_handler is \p NULL, the
 *                  internal default one will be used instead.
 *
 * \param[in]       logger_handler Logger handler structure.
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 */
#define LOGGER_API_DEBUG(logger_handler, tag, fmt, ...) \
    (int_logger_api_emit((logger_handler), LOGGER_LEVEL_DEBUG, (tag), ":%d >> " fmt "\n\r", __LINE__, ##__VA_ARGS__))

/*! \} */

/*!
 * \brief           Initialize the logger.
 *
 * \details         Initialize the passed \c logger_handler strcuture. If
 *                  \c logger_handler is \p NULL, the internal default one is
 *                  used instead.
 *
 * \param[out]      logger_handler Logger handler structure to initialize.
 * \param[in]       write Pointer to the write function callback.
 * \param[in]       flush Pointer to the flush function callback (optional).
 * \param[in]       verbosity Verbosity level, represented as a bit flag.
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if \p write is \c NULL.
 */
enum LoggerReturnCode logger_api_init(
    struct LoggerHandler *logger_handler,
    logger_write_callback write,
    logger_flush_callback flush,
    uint8_t verbosity);

/*!
 * \brief           Update the logger's verbosity level.
 *
 * \details         Set the verbosity level to the passed \c logger_handler
 *                  strcuture. If \c logger_handler is \p NULL, the internal
 *                  default one is used instead.
 *
 * \param[out]      logger_handler Logger handler structure.
 * \param[in]       verbosity New verbosity level, represented as a bit flag.
 */
void logger_api_set_verbosity(
    struct LoggerHandler *logger_handler,
    uint8_t verbosity);

/*!
 * \brief           Emit a log message.
 *
 * \details         Emit a generic log message by using the passed
 *                  \c logger_handler. If \c logger_handler is \p NULL, the
 *                  internal default one will be used instead.
 *
 * \param[in]       logger_handler Logger handler structure.
 * \param[in]       level Logging level
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 *
 * \note            Internal use only.
 * \attention       This function is not thread-safe!
 */
enum LoggerReturnCode int_logger_api_emit(
    const struct LoggerHandler *logger_handler,
    enum LoggerLevel level,
    const char *tag,
    const char *fmt,
    ...);

#endif /*! LOGGER_API_H */
