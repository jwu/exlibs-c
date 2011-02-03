// ======================================================================================
// File         : str_op.c
// Author       : Wu Jie 
// Last Change  : 10/12/2010 | 13:56:03 PM | Tuesday,October
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

#include "exsdk.h"
#include "str_op.h"

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

static const char __white_space_list[] = {
    (L' '),
    (L'\t'),
    (L'\r'),
    (L'\n'),
};

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

int ex_str_split_into_array( ex_array_t *_outList, const char *_token, const char *_text )
{
    const char *string_to_parse = _text;
    const char *string_to_split = string_to_parse;
    const char *rest_string = NULL;

    int token_len = strlen(_token);
    int sub_str_len = 0;
    uint parse_str_len = (token_len == 1) ? EX_UINT32_MAX : strlen(string_to_parse);

    // stop parsing when we get the end of the str
    while ( *string_to_parse != 0 && parse_str_len > 0 ) {
        sub_str_len += 1;

        // check if split
        if ( ex_str_nicmp( string_to_parse, _token, token_len ) == 0 ) {
            if ( sub_str_len > 0 ) {
                char *sub_str = (char *)ex_malloc ( sub_str_len );
                strncpy(sub_str, string_to_split, sub_str_len-1 );
                sub_str[sub_str_len-1] = '\0';
                ex_array_append ( _outList, &sub_str );
            }
            else {
                char *sub_str = (char *)ex_malloc (2);
                sub_str = "";
                ex_array_append ( _outList, &sub_str );
            }

            // reset the split value (skip token,too)
            string_to_split = string_to_parse + token_len;
            rest_string = string_to_split; 
            sub_str_len = 0;

            //
            parse_str_len -= token_len;
            string_to_parse += token_len;
        }
        // go to next check and increase split length
        else {
            parse_str_len -= 1;
            string_to_parse += 1;
        }
    }

    if ( rest_string != NULL ) {
        int len = strlen(rest_string);
        char *sub_str = (char *)ex_malloc(len+1);
        strncpy(sub_str, rest_string, len );
        sub_str[len] = '\0';
        ex_array_append ( _outList, &sub_str );
    }

    return (int)ex_array_count(_outList);
}
