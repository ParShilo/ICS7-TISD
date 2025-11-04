#ifndef ERR_DEF_H__
#define ERR_DEF_H__

#define MAX_LEN_FIRST_LINE 35
#define MAX_LEN_SECOND_LINE 40
#define MAX_LEN_MANTISSA 75
#define MAX_LEN_RESULT 40
#define MAX_LEN_EXPONENT 5
#define MAX_INPUT_LEN 100

enum
{
    OK = 0,
    ERROR_INPUT,
    ERROR_LONG_MANTISSA,
    ERROR_LONG_EXPONENTA,
    ERROR_EMPTY,
    ERROR_OVERFLOW,
    ERROR_UNDERFLOW
};

typedef struct
{
    char sign_mantissa;
    char mantissa[MAX_INPUT_LEN + 1];
    char sign_exponent;
    int exponent;    
} number;

#endif // #ifndef ERR_DEF_H__