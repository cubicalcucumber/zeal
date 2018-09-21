#include <stdarg.h>
#include <stdio.h>

#include "test.h"
#include "../utils.h"

/* Used to track test results if not NULL. */
static TestContext* context;

/* Register a passed check. */
static void pass_check() { context->total_checks += 1; }

/* Register a failed check. */
static void fail_check()
{
  context->failed_checks += 1;
  context->total_checks += 1;
}

/* Register a failed check and print a given message. */
static void fail_with_message(const char* file, uint32_t line, const char* msg,
                              va_list args)
{
  if (context)
    fail_check();
  printf("Check failed (%s:%d): ", file, line);
  vprintf(msg, args);
  puts(".");
}

void test_reset_context(TestContext* context)
{
  if (context)
  {
    context->total_checks = 0;
    context->failed_checks = 0;
  }
}

void test_use_context(TestContext* extern_context)
{
  context = extern_context;
  test_reset_context(context);
}

void zeal_pass()
{
  pass_check();
}

void zeal_fail(const char* file, uint32_t line, const char* msg, ...)
{
  va_list args;
  va_start(args, msg);
  fail_with_message(file, line, msg, args);
  va_end(args);
}

void zeal_check(const char* file, uint32_t line, bool condition,
                const char* msg, ...)
{
  if (!condition)
  {
    va_list args;
    va_start(args, msg);
    fail_with_message(file, line, msg, args);
    va_end(args);
    return;
  }

  if (context)
    pass_check();
}