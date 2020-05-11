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
    double n;
}lint_value;


enum{
    LINT_PARSE_OK = 0,
    LINT_PARSE_EXPECT_VALUE,  
    LINT_PARSE_INVALID_VALUE, 
    LINT_PARSE_ROOT_NOT_SINGULAR,
    LINT_PARSE_NUMBER_TOO_BIG
};




/* functions */
int lint_parse(lint_value* v, const char* json);
lint_type lint_get_type(const lint_value* v);
double lint_get_number(const lint_value* v);

 #endif  /* LINTJSON_H__ */
