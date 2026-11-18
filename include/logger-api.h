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
 * \defgroup           logger_api_macros Convenience macros that wrap \ref logger_api_emit.
 *
 * \note            The macros include the source line number in each log message using
 *                  \p __LINE__, making it easier to locate where the message originated.
 * \{
 */

/*!
 * \brief           Emit an information log message.
 *
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 */
#define LOGGER_API_INFO(tag, fmt, ...) \
    (logger_api_emit(LOGGER_LEVEL_INFO, (tag), ":%d >> " fmt "\n\r", __LINE__, ##__VA_ARGS__))

/*!
 * \brief           Emit an error log message.
 *
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 */
#define LOGGER_API_ERROR(tag, fmt, ...) \
    (logger_api_emit(LOGGER_LEVEL_ERROR, (tag), ":%d >> " fmt "\n\r", __LINE__, ##__VA_ARGS__))

/*!
 * \brief           Emit a warning log message.
 *
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 */
#define LOGGER_API_WARNING(tag, fmt, ...) \
    (logger_api_emit(LOGGER_LEVEL_WARNING, (tag), ":%d >> " fmt "\n\r", __LINE__, ##__VA_ARGS__))

/*!
 * \brief           Emit a debugging log message.
 *
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 */
#define LOGGER_API_DEBUG(tag, fmt, ...) \
    (logger_api_emit(LOGGER_LEVEL_DEBUG, (tag), ":%d >> " fmt "\n\r", __LINE__, ##__VA_ARGS__))

/*! \} */

/*!
 * \brief           Initialize the logger.
 *
 * \param[in]       write Pointer to the write function callback.
 * \param[in]       flush Pointer to the flush function callback (optional).
 * \param[in]       verbosity Verbosity level, represented as a bit flag.
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if \p write is \c NULL.
 */
enum LoggerReturnCode logger_api_init(
    logger_write_callback write,
    logger_flush_callback flush,
    uint8_t verbosity);

/*!
 * \brief           Update the logger's verbosity level.
 *
 * \param[in]       verbosity New verbosity level, represented as a bit flag.
 */
void logger_api_set_verbosity(uint8_t verbosity);

/*!
 * \brief           Emit a log message.
 *
 * \param[in]       level Logging level
 * \param[in]       tag Tag string identifying the module.
 * \param[in]       fmt Format string
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized and if \p fmt or \p tag is \c NULL.
 * \retval          LOGGER_RC_BUFFER_SIZE_ERROR if the message exceeds \ref LOGGER_EMIT_BUFFER_SIZE.
 * \retval          LOGGER_RC_TRANSMISSION_ERROR otherwise.
 *
 * \attention       This function is not thread-safe yet!
 */
enum LoggerReturnCode logger_api_emit(
    enum LoggerLevel level,
    const char *tag,
    const char *fmt,
    ...);

#endif /*! LOGGER_API_H */
