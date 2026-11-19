/*!
 * \file            test-logger-api.c
 * \date            2025-07-10
 * \authors         Mirko Lana [mirko.lana@eagletrt.it]
 *
 * \brief           Liblog unit tests.
 */

#include "unity.h"
#include "fff.h"
DEFINE_FFF_GLOBALS;

#include "logger.h"
#include "logger-api.h"

extern bool prv_logger_api_level_is_valid(enum LoggerLevel level);
extern uint8_t prv_logger_api_bit_index(enum LoggerLevel level);
extern struct LoggerHandler logger_handler;

FAKE_VALUE_FUNC2(int, write_wrapper, const char *, size_t);
FAKE_VALUE_FUNC0(int, flush_wrapper);

void setUp(void) {
    logger_api_init(
        write_wrapper,
        flush_wrapper,
        LOGGER_ENABLE_ALL);
}

/*!
 * \defgroup        test_logger_api_level_is_valid Tests for level checking function.
 * \{
 */

void test_prv_logger_api_level_is_valid_with_right_values(void) {
    constexpr size_t size = 4U;
    const char *const failure_messages[size] = {
        "LOGGER_LEVEL_INFO should be valid!",
        "LOGGER_LEVEL_ERROR should be valid!",
        "LOGGER_LEVEL_WARNING should be valid!",
        "LOGGER_LEVEL_DEBUG should be valid!",
    };
    const bool results[size] = {
        prv_logger_api_level_is_valid(LOGGER_LEVEL_INFO),
        prv_logger_api_level_is_valid(LOGGER_LEVEL_ERROR),
        prv_logger_api_level_is_valid(LOGGER_LEVEL_WARNING),
        prv_logger_api_level_is_valid(LOGGER_LEVEL_DEBUG),
    };

    for (size_t i = 0U; i < size; ++i) {
        TEST_ASSERT_TRUE_MESSAGE(results[i], failure_messages[i]);
    }
}

void test_prv_logger_api_level_is_valid_with_non_power_of_two_bitmask(void) {
    constexpr enum LoggerLevel level = (enum LoggerLevel)0b0011;
    TEST_ASSERT_FALSE(prv_logger_api_level_is_valid(level));
}

void test_prv_logger_api_level_is_valid_with_large_bitmask_value(void) {
    constexpr enum LoggerLevel level = (enum LoggerLevel)0b10000;
    TEST_ASSERT_FALSE(prv_logger_api_level_is_valid(level));
}

/*! \} */

/*!
 * \defgroup        test_logger_api_bit_index Tests for level bitmask to index private function.
 * \{
 */

void test_prv_logger_api_bit_index_with_right_values(void) {
    constexpr size_t size = 4U;
    const uint8_t expected[size] = { 0, 1, 2, 3 };
    const uint8_t actual[size] = {
        prv_logger_api_bit_index(LOGGER_LEVEL_INFO),
        prv_logger_api_bit_index(LOGGER_LEVEL_ERROR),
        prv_logger_api_bit_index(LOGGER_LEVEL_WARNING),
        prv_logger_api_bit_index(LOGGER_LEVEL_DEBUG),
    };

    TEST_ASSERT_EQUAL_UINT8_ARRAY(expected, actual, size);
}

/*! \} */

/*!
 * \defgroup        test_logger_api_init Tests for initialization function.
 * \{
 */

void test_logger_api_init_with_right_values(void) {
    const enum LoggerReturnCode expected = LOGGER_RC_OK;
    const enum LoggerReturnCode actual = logger_api_init(
        write_wrapper,
        flush_wrapper,
        LOGGER_ENABLE_ALL);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_init_with_null_write_callback(void) {
    const enum LoggerReturnCode expected = LOGGER_RC_NULL_POINTER;
    const enum LoggerReturnCode actual = logger_api_init(
        NULL,
        flush_wrapper,
        LOGGER_ENABLE_ALL);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_init_with_null_flush_callback(void) {
    const enum LoggerReturnCode expected = LOGGER_RC_OK;
    const enum LoggerReturnCode actual = logger_api_init(
        write_wrapper,
        NULL,
        LOGGER_ENABLE_ALL);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

/*! } */

/*!
 * \defgroup        test_logger_api_emit Tests for emit function.
 * \{
 */

void test_logger_api_emit_with_right_values(void) {
    const char *tag = "TEST";
    const char *fmt = "format string";
    const enum LoggerReturnCode expected = LOGGER_RC_OK;
    const enum LoggerReturnCode actual = logger_api_emit(
        LOGGER_LEVEL_INFO, tag, fmt);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_emit_with_null_writer(void) {
    const char *tag = "TEST";
    const char *fmt = "format string";
    logger_handler.write = NULL;
    const enum LoggerReturnCode expected = LOGGER_RC_NULL_POINTER;
    const enum LoggerReturnCode actual = logger_api_emit(
        LOGGER_LEVEL_INFO, tag, fmt);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_emit_with_null_tag(void) {
    const char *tag = NULL;
    const char *fmt = "format string";
    const enum LoggerReturnCode expected = LOGGER_RC_NULL_POINTER;
    const enum LoggerReturnCode actual = logger_api_emit(
        LOGGER_LEVEL_INFO, tag, fmt);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_emit_with_null_fmt(void) {
    const char *tag = "TEST";
    const char *fmt = NULL;
    const enum LoggerReturnCode expected = LOGGER_RC_NULL_POINTER;
    const enum LoggerReturnCode actual = logger_api_emit(
        LOGGER_LEVEL_INFO, tag, fmt);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_emit_with_empty_tag(void) {
    const char *tag = "";
    const char *fmt = "format string";
    const enum LoggerReturnCode expected = LOGGER_RC_INVALID_ARGUMENT;
    const enum LoggerReturnCode actual = logger_api_emit(
        LOGGER_LEVEL_INFO, tag, fmt);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_emit_with_invalid_level(void) {
    const char *tag = "TEST";
    const char *fmt = "format string";
    const enum LoggerReturnCode expected = LOGGER_RC_INVALID_ARGUMENT;
    const enum LoggerReturnCode actual = logger_api_emit(
        (enum LoggerLevel)0b0011, tag, fmt);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_emit_with_write_failure(void) {
    const char *tag = "TEST";
    const char *fmt = "format string";
    const enum LoggerReturnCode expected = LOGGER_RC_TRANSMISSION_ERROR;
    write_wrapper_fake.return_val = -1;
    const enum LoggerReturnCode actual = logger_api_emit(
        LOGGER_LEVEL_INFO, tag, fmt);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

void test_logger_api_emit_with_long_format_string(void) {
    const char *tag = "TEST";
    const char *fmt =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do canis";
    const enum LoggerReturnCode expected = LOGGER_RC_TRANSMISSION_ERROR;
    write_wrapper_fake.return_val = -1;
    const enum LoggerReturnCode actual = logger_api_emit(
        LOGGER_LEVEL_INFO, tag, fmt);

    TEST_ASSERT_EQUAL_INT(expected, actual);
}

/*! \} */

int main(void) {
    UNITY_BEGIN();

    /*!
     * \addgroup        test_logger_api_level_is_valid Run level checking tests.
     * \{
     */

    RUN_TEST(test_prv_logger_api_level_is_valid_with_right_values);
    RUN_TEST(test_prv_logger_api_level_is_valid_with_non_power_of_two_bitmask);
    RUN_TEST(test_prv_logger_api_level_is_valid_with_large_bitmask_value);

    /*! \} */

    /*!
     * \addgroup        test_logger_api_bit_index Run level bitmask to index tests.
     * \{
     */

    RUN_TEST(test_prv_logger_api_bit_index_with_right_values);

    /*! \} */

    /*!
     * \addgroup        test_logger_api_init Run itialization tests.
     * \{
     */

    RUN_TEST(test_logger_api_init_with_right_values);
    RUN_TEST(test_logger_api_init_with_null_write_callback);
    RUN_TEST(test_logger_api_init_with_null_flush_callback);

    /*! \} */

    /*!
     * \addgroup        test_logger_api_emit Run emit function tests.
     * \{
     */

    RUN_TEST(test_logger_api_emit_with_right_values);
    RUN_TEST(test_logger_api_emit_with_null_writer);
    RUN_TEST(test_logger_api_emit_with_null_tag);
    RUN_TEST(test_logger_api_emit_with_null_fmt);
    RUN_TEST(test_logger_api_emit_with_empty_tag);
    RUN_TEST(test_logger_api_emit_with_invalid_level);
    RUN_TEST(test_logger_api_emit_with_write_failure);
    RUN_TEST(test_logger_api_emit_with_long_format_string);

    /*! \} */

    UNITY_END();
}
