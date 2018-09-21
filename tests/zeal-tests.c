#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "test.h"
#include "../zeal.h"

/* Expands into code that compares the current interpreter result to the given
 * expected value. Register a failed check if the values differ. */
#define ZEAL_CHECK_EVALUATION_RESULT(interpreter, expected_value)              \
  do                                                                           \
  {                                                                            \
    Value result = interpreter_get_result(interpreter);                        \
    if (!values_equal(result, expected_value))                                 \
    {                                                                          \
      printf("\n");                                                            \
      ZEAL_FAIL("Result differs from expected value");                         \
      printf("> Expected: ");                                                  \
      value_print(expected_value);                                             \
      printf("\n> Result:   ");                                                \
      value_print(result);                                                     \
      printf("\n");                                                            \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      ZEAL_PASS();                                                             \
    }                                                                          \
  } while (0);

/* Expands into code that evaluates an input string with an interpreter and
 * compares the result with an expected value. Register a failed check if the
 * values differ or a compiler error occurs. */
#define ZEAL_CHECK_EVALUATION(interpreter, input, expected_value)              \
  do                                                                           \
  {                                                                            \
    evaluate(interpreter, input);                                              \
    if (interpreter->compiler.error)                                           \
    {                                                                          \
      ZEAL_FAIL("Compiler error");                                             \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      ZEAL_CHECK_EVALUATION_RESULT(interpreter, expected_value);               \
    }                                                                          \
  } while (0);

static bool values_equal(Value v1, Value v2)
{
  return v1.type == v2.type && v1.integer == v2.integer;
}

static void show_test_results(TestContext ctx)
{
  if (!ctx.failed_checks)
    printf("OK.\n");
  else
    printf("(%d/%d) tests failed.\n", ctx.failed_checks, ctx.total_checks);
}

typedef void (*InterpreterTest)(Interpreter*);

static void run_interpreter_test(InterpreterTest test, const char* name)
{
  Interpreter interpreter;
  interpreter_init(&interpreter);

  TestContext ctx;
  test_use_context(&ctx);

  printf("Testing %s: ", name);
  test(&interpreter);
  show_test_results(ctx);
}

void test_integer_constants(Interpreter* interpreter)
{
  ZEAL_CHECK_EVALUATION(interpreter, "0", value_from_integer(0));
  ZEAL_CHECK_EVALUATION(interpreter, "1", value_from_integer(1));
  ZEAL_CHECK_EVALUATION(interpreter, "123", value_from_integer(123));
  ZEAL_CHECK_EVALUATION(interpreter, "9223372036854775807",
                        value_from_integer(9223372036854775807));
}

void test_basic_arithmetic(Interpreter* interpreter)
{
  ZEAL_CHECK_EVALUATION(interpreter, "1 + 2", value_from_integer(3));
  ZEAL_CHECK_EVALUATION(interpreter, "3 - 4", value_from_integer(-1));
  ZEAL_CHECK_EVALUATION(interpreter, "5 * 6", value_from_integer(30));
}

void test_grouped_expressions(Interpreter* interpreter)
{
  ZEAL_CHECK_EVALUATION(interpreter, "(1)", value_from_integer(1));
  ZEAL_CHECK_EVALUATION(interpreter, "((((1))))", value_from_integer(1));
  ZEAL_CHECK_EVALUATION(interpreter, "((((1+2)*3)-4)+5)",
                        value_from_integer(10));
  ZEAL_CHECK_EVALUATION(interpreter, "(((1 + 2) - (3 * 4)) + 5)",
                        value_from_integer(-4));
}

void test_arithmetic_precedence(Interpreter* interpreter)
{
  ZEAL_CHECK_EVALUATION(interpreter, "2 + 3 * 4", value_from_integer(14));
  ZEAL_CHECK_EVALUATION(interpreter, "2 * 3 + 4", value_from_integer(10));

  ZEAL_CHECK_EVALUATION(interpreter, "2 - 3 * 4", value_from_integer(-10));
  ZEAL_CHECK_EVALUATION(interpreter, "2 * 3 - 4", value_from_integer(2));

  ZEAL_CHECK_EVALUATION(interpreter, "2 + (3 * 4)", value_from_integer(14));
  ZEAL_CHECK_EVALUATION(interpreter, "(2 + 3) * 4", value_from_integer(20));

  ZEAL_CHECK_EVALUATION(interpreter, "(2 * 3) + 4", value_from_integer(10));
  ZEAL_CHECK_EVALUATION(interpreter, "2 * (3 + 4)", value_from_integer(14));
}

int32_t main(void)
{
  run_interpreter_test(test_integer_constants, "integer constants");
  run_interpreter_test(test_basic_arithmetic, "basic arithmetic");
  run_interpreter_test(test_arithmetic_precedence, "arithmetic precedence");
  run_interpreter_test(test_grouped_expressions, "grouped expressions");
  return 0;
}