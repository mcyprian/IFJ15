/**
 * @file token.h
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz> Tomas Scavnicky <xscavn00@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Header file of module token.c, including typedef of general TToken struct.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <datatypes.h>
#include <dynamic_structure_buffer.h>


typedef struct {
    index_t next;                  // Index to next token/expr in dynamic_structure
    index_t token_index;           // Index to token string in dynamic_buffer
    int token_type;
    int original_type;
} TToken;

enum token_types
{
	O_EQUALS, // 0                   ==
	O_G, // 1                        >
	O_L, // 2                        <
    O_GE, // 3                       >=
    O_LE, // 4                       <=
    O_PLUS,  // 5                    +
	O_MINUS, // 6                    -
    O_MUL,   // 7                    *
    O_DIV,   // 8                    /
    O_NE,  // 9                      !=
	CLOSING_BRACKET, // 10           )
    OPENING_BRACKET, // 11           (
    END_OF_EXPR,     // 12
	IDENTIFIER, // 13                            }
	L_INT, // 14                                 } <RVALUE>
    L_DOUBLE, // 15                              }
	L_STRING, // 16                              }
    O_ASSIGN, // 17                  =
	O_LEFT_ARROW, // 18              <<
	O_RIGHT_ARROW, // 19             >>
	T_DOUBLE, // 20                        } <TYPE>
	T_INT, // 21                           }
	T_STRING, // 22                        }
    K_CIN, // 23                              } <KEY>
	K_COUT, // 24                             }
    K_DO,   // 25                             }
	K_ELSE, // 26                             }
	K_FOR, // 27                              }
	K_IF,	// 28                             }
	K_RETURN, // 29                           }
    K_WHILE,  // 30                           }
	UO_EXCLAMATION, // 31            !
	AUTO, // 32                      auto
	COMMA, // 33                     ,
	CLOSING_CURLY_BRACKET, // 34     }
	OPENING_CURLY_BRACKET, // 35     {
	SEMICOLON, // 36                 ;
    COMMENT, // 37
    LINE_COMMENT, // 38
    BLOCK_COMMENT, // 39
    BLOCK_COMMENT_END, // 40
    SCIENTIFIC, // 41
	START, // 42
	ERRORT, // 43
	EOFT, // 44
    SHIFT, // 45                      } precedence analysis
    RVALUE, //46                      }
    INTERNAL_ERROR_TOKEN // 47
};

#define catch_token_internal_error(var, value, token, index)            \
    do {                                                                \
        if ((var) == (value)) {                                         \
            token->token_type = INTERNAL_ERROR_TOKEN;					\
            return(index);                                    			\
        }                                                               \
    } while (0)

#endif // !TOKEN_H
