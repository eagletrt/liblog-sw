/*!
 * \file            logger-api.h
 * \date            2026-11-15
 * \authors         Mirko Lana [mirko.lana@eagletrt.it]
 *
 * \brief           Liblog APIs.
 */

#ifndef LOGGER_API_H
#define LOGGER_API_H

#include <stdint.h>

#include "logger.h"

/*!
 * \brief           Initialize the logger.
 *
 * \param[in]       write A pointer to the write function callback.
 * \param[in]       flush A pointer to the flush function callback.
 * \param[in]       verbosity Verbosity level, represented as a bit flag.
 *
 * \retval          LOGGER_RC_OK on success.
 * \retval          LOGGER_RC_NULL_POINTER if \p write or \p flush is NULL.
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
 * \retval          LOGGER_RC_NULL_POINTER if the logger isn't initialized or \p fmt is NULL.
 * \retval          LOGGER_RC_INVALID_LEVEL if the logging level is invalid.
 * \retval          LOGGER_RC_ERROR otherwise.
 *
 * \pre             \p level must be one of \c LOGGER_LEVEL_INFO, \c LOGGER_LEVEL_DEBUG, \c LOGGING_LEVEL_DEBUG and \c LOGGER_LEVEL_ERROR.
 */
enum LoggerReturnCode logger_api_emit(
    enum LoggerLevel level,
    const char *tag,
    const char *fmt,
    ...);

#endif /*! LOGGER_API_H */
