 #ifndef LINTJSON_H__
 #define LINTJSON_H__

typedef enum {
    LINT_NULL, LINT_FALSE, LINT_TRUE, LINT_NUMBER, 
    LINT_STRING, LINT_ARRAY, LINT_OBJECT 
} lint_type;

enum{
    LINT_PARSE_OK = 0,
    LINT_PARSE_EXPECT_VALUE,  //json只含空白
    LINT_PARSE_INVALID_VALUE, //值后空白还有其他字符
    LINT_PARSE_ROOT_NOT_SINGULAR
};


typedef struct 
{
    lint_type type;
}lint_value;



/* functions */
int lint_parse(lint_value* v, const char* json);
lint_type lint_get_type(const lint_value* v);






 #endif  /* LINTJSON_H__ */
