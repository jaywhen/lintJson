#include "lintjson.h"
#include <stdio.h>

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
    test_count++;\
    if (equality)\
        test_pass++;\
    else {\
        fprintf(stderr, "%s:%d: expect: "format" actual: "format"\n", __FILE__, __LINE__, expect, actual);\
        main_ret = 1;\
    }\
} while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect)==(actual), expect, actual, "%d")
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect)==(actual), expect, actual, "%.17g")



static void test_parse_null() {
    lint_value v;
    v.type = LINT_FALSE;
    EXPECT_EQ_INT(LINT_PARSE_OK, lint_parse(&v, "null"));
    EXPECT_EQ_INT(LINT_NULL, lint_get_type(&v));
}

static void test_parse_true() {
    lint_value v;
    v.type = LINT_FALSE;
    EXPECT_EQ_INT(LINT_PARSE_OK, lint_parse(&v, "true"));
    EXPECT_EQ_INT(LINT_TRUE, lint_get_type(&v));
}

static void test_parse_false() {
    lint_value v;
    v.type = LINT_TRUE;
    EXPECT_EQ_INT(LINT_PARSE_OK, lint_parse(&v, "false"));
    EXPECT_EQ_INT(LINT_FALSE, lint_get_type(&v));
}

#define TEST_NUMBER(expect, json)\
    do {\
        lint_value v;\
        EXPECT_EQ_INT(LINT_PARSE_OK, lint_parse(&v, json));\
        EXPECT_EQ_INT(LINT_NUMBER, lint_get_type(&v));\
        EXPECT_EQ_DOUBLE(expect, lint_get_number(&v));\
    } while(0)

static void test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */

    /* wikipedia.org: Double-precision_floating-point_format */
    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

#define TEST_ERROR(error, json)\
    do {\
        lint_value v;\
        v.type = LINT_FALSE;\
        EXPECT_EQ_INT(error, lint_parse(&v, json));\
        EXPECT_EQ_INT(LINT_NULL, lint_get_type(&v));\
    } while(0)


static void test_parse_expect_value() {
    TEST_ERROR(LINT_PARSE_EXPECT_VALUE, "");
    TEST_ERROR(LINT_PARSE_EXPECT_VALUE, " ");
}

static void test_parse_invalid_value() {
    /*
     *
    lint_value v;
    v.type = LINT_FALSE;
    EXPECT_EQ_INT(LINT_PARSE_INVALID_VALUE, lint_parse(&v, "nul"));
    EXPECT_EQ_INT(LINT_NULL, lint_get_type(&v));

    v.type = LINT_FALSE;
    EXPECT_EQ_INT(LINT_PARSE_INVALID_VALUE, lint_parse(&v, "?"));
    EXPECT_EQ_INT(LINT_NULL, lint_get_type(&v));
     *
     */

    #if 1
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "nul");
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "?");
    #endif


    /**/
    /* invalid number */
    #if 1
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, ".123");

    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "1.");
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "23.");

    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "WHOIAM");
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "HP:1000");
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "nan");
    TEST_ERROR(LINT_PARSE_INVALID_VALUE, "NAN");
    #endif

}

static void test_parse_root_not_singular() {

    /*  
     *
     * 
    lint_value v;
    v.type = LINT_FALSE;
    EXPECT_EQ_INT(LINT_PARSE_ROOT_NOT_SINGULAR, lint_parse(&v, "null x"));
    EXPECT_EQ_INT(LINT_NULL, lint_get_type(&v));
    */

    TEST_ERROR(LINT_PARSE_ROOT_NOT_SINGULAR, "null x");
    
    /* need to add some */
    #if 1
    TEST_ERROR(LINT_PARSE_ROOT_NOT_SINGULAR, "0123");
    TEST_ERROR(LINT_PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(LINT_PARSE_ROOT_NOT_SINGULAR, "0x123");
    #endif
    
}

static void test_parse_number_too_big() {
    /* add some? */
    #if 1
    TEST_ERROR(LINT_PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(LINT_PARSE_NUMBER_TOO_BIG, "-1e309");
    #endif
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_number_too_big();
}

int main() {
    test_parse();
    printf("%d / %d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}


