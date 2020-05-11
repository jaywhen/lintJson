#include "lintjson.h"
#include <stdlib.h> /* NULL  ||ps: NULL are also included in <stdio.h> || strtod() */
#include <assert.h> /* assert() */

#define EXPECT(c, ch) do{ assert(*c->json == (ch)); c->json++; } while(0)

typedef struct
{
    const char* json;
}lint_context;



/* ws = *(%x20 / %x09 / %x0A / %x0D) */
static void lint_parse_whitespace(lint_context* c){
    const char* p = c->json;
    while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
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

static int lint_parse_literal(lint_context* c, lint_value* v, const char* literal, lint_type type) {
    /* to use size_t in array*/
    size_t i;
    EXPECT(c, literal[0]);
    for(i=0; literal[i+1]; ++i)
        if(c->json[i] != literal[i+1])
            return LINT_PARSE_INVALID_VALUE;
    c->json += i;
    v->type = type;
    return LINT_PARSE_OK;
}


static int lint_parse_number(lint_context* c, lint_value* v) {
    char* end;
    /* \TODO validate number */
    v->n = strtod(c->json, &end);
    /* only have string: like: "number" : string */
    if(c->json == end)
        return LINT_PARSE_INVALID_VALUE;
    c->json = end;
    v->type = LINT_NUMBER;
    return LINT_PARSE_OK;
}

static int lint_parse_value(lint_context* c, lint_value* v){
    switch (*c->json)
    {
    case 'n':  return lint_parse_literal(c, v, "null", LINT_NULL);
    case 't':  return lint_parse_literal(c, v, "true", LINT_TRUE);
    case 'f':  return lint_parse_literal(c, v, "false", LINT_FALSE);
    default:   return lint_parse_number(c, v);
    case '\0': return LINT_PARSE_EXPECT_VALUE;
    }

}




int lint_parse(lint_value* v, const char* json){

    lint_context c;
    int ret;
    assert(v != NULL);
    c.json = json;
    v->type = LINT_NULL;
    lint_parse_whitespace(&c);
    
    ret = lint_parse_value(&c, v);

    if(ret == LINT_PARSE_OK){
        
        lint_parse_whitespace(&c);
        if(*c.json != '\0')
            return ret = LINT_PARSE_ROOT_NOT_SINGULAR;
        
    }
    return ret;
}

lint_type lint_get_type(const lint_value* v){
    assert(v != NULL);
    return v->type;
}

double lint_get_number(const lint_value* v){
    assert(v != NULL && v->type == LINT_NUMBER);
    return v->n;
}