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
    index_t token_index;           // Index to token string in dynamic_buffer
    int token_type;
    index_t token_next;            // Index to next token/expr in dynamic_structure
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
	IDENTIFIER, // 12                            }
	L_INT, // 13                                 } <RVALUE>
    L_DOUBLE, // 14                              }
	L_STRING, // 15                              }
    O_ASSIGN, // 16                  =
	O_LEFT_ARROW, // 17              <<
	O_RIGHT_ARROW, // 18             >>
	T_DOUBLE, // 19                        } <TYPE>
	T_INT, // 20                           }
	T_STRING, // 21                        }
    K_CIN, // 22                              } <KEY>
	K_COUT, // 23                             }
    K_DO,   // 24                             }
	K_ELSE, // 25                             }
	K_FOR, // 26                              }
	K_IF,	// 27                             }
	K_RETURN, // 28                           }
    K_WHILE,  // 29                           }
	UO_EXCLAMATION, // 30            !
	AUTO, // 31                      auto
	COMMA, // 32                     ,
	CLOSING_CURLY_BRACKET, // 33     }
	OPENING_CURLY_BRACKET, // 34     {
	SEMICOLON, // 35                 ;
    COMMENT, // 36
    LINE_COMMENT, // 37
    BLOCK_COMMENT, // 38
    BLOCK_COMMENT_END, // 39
    SCIENTIFIC, // 40
	START, // 41
	ERRORT, // 42
	EOFT, // 43
    SHIFT, // 44                      } precedence analysis
    RVALUE //45                       }
};


#endif // !TOKEN_H
