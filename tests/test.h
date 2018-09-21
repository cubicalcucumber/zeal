#ifndef zeal_test_h
#define zeal_test_h

#include <inttypes.h>
#include <stdbool.h>

/* A test context may be used to keep track of the number of total and failed
 * checks performed in a test. */
typedef struct
{
  uint32_t total_checks;
  uint32_t failed_checks;
} TestContext;

/* Use a test context for tracking test results of future tests. Disable by
 * passing NULL. */
void test_use_context(TestContext* context);

/* Reset the number of total and failed checks inside the given test context. */
void test_reset_context(TestContext* context);

/* Check the given condition. If the condition is false, fail with the given
 * message. */
#define ZEAL_CHECK(condition, ...)                                             \
  zeal_check(__FILE__, __LINE__, condition, __VA_ARGS__)

/* Fail a check with a given message. */
#define ZEAL_FAIL(...) zeal_fail(__FILE__, __LINE__, __VA_ARGS__)

/* Pass a check. */
#define ZEAL_PASS() zeal_pass()

void zeal_pass();

void zeal_check(const char* file, uint32_t line, bool condition,
                const char* msg, ...);

void zeal_fail(const char* file, uint32_t line, const char* msg, ...);

#endif