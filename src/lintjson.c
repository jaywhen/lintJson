#include "lintjson.h"
#include <stdio.h>
//#include <stdlib.h>
#include <assert.h>

#define EXPECT(c, ch) do{ assert(*c->json == (ch)); c->json++; } while(0)

typedef struct
{
    const char* json;
}lint_context;



/* ws = *(%x20 / %x09 / %x0A / %x0D) */
static void lint_parse_whitespace(lint_context* c){
    const char* *p = c->json;
    while(*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}


/* null = "null" */
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
    v->type = LINT_TRUE;
    return LINT_PARSE_OK;
}



static int lint_parse_value(lint_context* c, lint_value* v){
    switch (*c->json)
    {
    case 'n':  return lint_parse_null(c, v);
    case 't':  return lint_parse_true(c, v);
    case 'f':  return lint_parse_false(c, v);
    case '\0': return LINT_PARSE_EXPECT_VALUE;
    default:   return LINT_PARSE_INVALID_VALUE;
    }

}


/* 提示：这里应该是 JSON-text = ws value ws */
/* 以下实现没处理最后的 ws 和 LEPT_PARSE_ROOT_NOT_SINGULAR */
int lint_parse(lint_value* v, const char* json){
    lint_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LINT_NULL;
    lint_parse_whitespace(&c);
    int parse_stat = lint_parse_value(&c, v);
    if(parse_stat == LINT_PARSE_OK){
        //如果是忽略＂前引号的话
        lint_parse_whitespace(&c);
        return LINT_PARSE_OK;
    }else if(c.json != '\0'){
        return LINT_PARSE_ROOT_NOT_SINGULAR;
    }else{
        return parse_stat;
    }
}
