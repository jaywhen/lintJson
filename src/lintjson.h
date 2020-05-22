#ifndef LINTJSON_H__
#define LINTJSON_H__



/* store data type*/
typedef enum {
    LINT_NULL, 
    LINT_FALSE, 
    LINT_TRUE, 
    LINT_NUMBER, 
    LINT_STRING, 
    LINT_ARRAY, 
    LINT_OBJECT 
} lint_type;

/* store all of this json text */
typedef struct 
{
    /*
     * now value have:
     * null/ false/ true/ number
     */
    lint_type type;
    union {
        struct { char* s; size_t len; }s; /* string */
        double n;                         /* number */
    }u;
    
}lint_value;


enum{
    LINT_PARSE_OK = 0,
    LINT_PARSE_EXPECT_VALUE,  
    LINT_PARSE_INVALID_VALUE, 
    LINT_PARSE_ROOT_NOT_SINGULAR,
    LINT_PARSE_NUMBER_TOO_BIG
};


#define lint_init(v) do{ (v)->type = LINT_NULL; } while(0)
#define lint_set_null(v) lint_free(v)

void lint_free(lint_value* v);

/* ---------------------functions--------------------- */
int lint_parse(lint_value* v, const char* json);

lint_type lint_get_type(const lint_value* v);

/* for parse boolean */
int lint_get_boolean(const lint_value* v);
void lint_set_boolean(lint_value* v, int b);

/* for parse number */
double lint_get_number(const lint_value* v);
void lint_set_number(lint_value* v, double n);

/* for parse string */
size_t lint_get_string_length(const lint_value* v);
const char* lint_get_string(const lint_value* v);
void lint_set_string(lint_value* v, const char* s, size_t len);

#endif  /* LINTJSON_H__ */
