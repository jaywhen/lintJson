#include "lintjson.h"
#include <stdlib.h> /* NULL ; strtod() ; size_t ; free ; malloc */
#include <assert.h> /* assert() */
#include <errno.h>  /* errno, ERANGE */
#include <math.h>   /* HUGE_VAL */
#include <string.h> /* memcpy */

#define EXPECT(c, ch) do{ assert(*c->json == (ch)); c->json++; } while (0)
#define ISDIGIT(ch) ((ch) >= '0' && (ch) <= '9')
#define ISDIGIT1TO9(ch) ((ch) >= '1' && (ch) <= '9')

typedef struct
{
    const char* json;
    char* stack;
    size_t size, top;
} lint_context;

/* ws = *(%x20 / %x09 / %x0A / %x0D) */
static void lint_parse_whitespace(lint_context* c)
{
    const char* p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/* null = "null" */

/*
 *
static int lint_parse_null(lint_context* c, lint_value* v){
    EXPECT(c, 'n');
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LINT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LINT_NULL;
    return LINT_PARSE_OK;
}

static int lint_parse_true(lint_context* c, lint_value* v){
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e')
        return LINT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LINT_TRUE;
    return LINT_PARSE_OK;
}

static int lint_parse_false(lint_context* c, lint_value* v){
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e')
        return LINT_PARSE_INVALID_VALUE;
    c->json += 4;
    v->type = LINT_FALSE;
    return LINT_PARSE_OK;
}
 *
 */
/* combine/merge above functions */

static int lint_parse_literal(lint_context* c, lint_value* v, const char* literal, lint_type type)
{
    /* to use size_t in array*/
    size_t i;
    EXPECT(c, literal[0]);
    for (i = 0; literal[i + 1]; ++i)
        if (c->json[i] != literal[i + 1])
            return LINT_PARSE_INVALID_VALUE;
    c->json += i;
    v->type = type;
    return LINT_PARSE_OK;
}

static int lint_parse_number(lint_context* c, lint_value* v)
{
    const char* p = c->json;
    /* \TODO validate number 
     * ---------------------
     * number = [ "-" ] int [ frac ] [ exp ]
     * int = "0" / digit1-9 *digit
     * frac = "." 1*digit
     * exp = ("e" / "E") ["-" / "+"] 1*digit 
     * ---------------------
     */
    /* '-' */
    if (*p == '-')
        p++;

    /* int */
    if (*p == '0')
        p++; /* is '0'  skip it */
    else
    {
        if (!ISDIGIT1TO9(*p))
            return LINT_PARSE_INVALID_VALUE;

        for (p++; ISDIGIT(*p); p++)
            ;
    }

    /* decimal */
    if (*p == '.')
    {
        p++;
        if (!ISDIGIT(*p))
            return LINT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++)
            ;
    }

    /* index number(exp) */

    if (*p == 'e' || *p == 'E')
    {
        p++;
        if (*p == '-' || *p == '+')
            ++p;
        if (!ISDIGIT(*p))
            return LINT_PARSE_INVALID_VALUE;
        for (++p; ISDIGIT(*p); ++p)
            ;
    }

    errno = 0;
    v->u.n = strtod(c->json, NULL);
    /* only have string: like: "number" : string */
    if (errno == ERANGE && (v->u.n == HUGE_VAL || v->u.n == -HUGE_VAL))
        return LINT_PARSE_NUMBER_TOO_BIG;
    v->type = LINT_NUMBER;
    c->json = p;
    return LINT_PARSE_OK;
}

static int lint_parse_value(lint_context* c, lint_value* v)
{
    switch (*c->json)
    {
    case 'n':
        return lint_parse_literal(c, v, "null", LINT_NULL);
    case 't':
        return lint_parse_literal(c, v, "true", LINT_TRUE);
    case 'f':
        return lint_parse_literal(c, v, "false", LINT_FALSE);
    default:
        return lint_parse_number(c, v);
    case '\0':
        return LINT_PARSE_EXPECT_VALUE;
    }
}

int lint_parse(lint_value* v, const char* json)
{

    lint_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    c.stack = NULL;
    c.size = c.top = 0;
    lint_init(v); /* v->type = LINT_NULL; */
    lint_parse_whitespace(&c);

    ret = lint_parse_value(&c, v);

    if (ret == LINT_PARSE_OK)
    {

        lint_parse_whitespace(&c);
        if (*c.json != '\0') {
                v->type = LINT_NULL;
                ret = LINT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    assert(c.top == 0);
    free(c.stack);
    return ret;
}

lint_type lint_get_type(const lint_value* v)
{
    assert(v != NULL);
    return v->type;
}

double lint_get_number(const lint_value* v)
{
    assert(v != NULL && v->type == LINT_NUMBER);
    return v->u.n;
}

/* free */
void lint_free(lint_value* v) {
    assert(v != NULL);
    if (v->type == LINT_STRING)
        free(v->u.s.s);
    
    /* avoid repeated free */
    v->type = LINT_NULL;
}


/* string's set && get */
void lint_set_string(lint_value* v, const char* s, size_t len) {
    assert(v != NULL && (s != NULL || len == 0));
    /* copy */
    lint_free(v);
    v->u.s.s = (char*)malloc(len + 1);
    memcpy(v->u.s.s, s, len);

    /* set */
    v->u.s.s[len] = '\0'; /* cuz array's index is from 0 to ... */
    v->u.s.len = len;
    v->type = LINT_STRING;
}

size_t lint_get_string_length(const lint_value* v) {
    assert(v != NULL && v->type == LINT_STRING);
    return v->u.s.len;
}

const char* lint_get_string(const lint_value* v) {
    assert(v != NULL && v->type == LINT_STRING);
    return v->u.s.s;
}