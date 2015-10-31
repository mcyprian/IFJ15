/**
 * @file precedence_analysis.c
 * @author Michal Cyprian <xcypri01@stud.fit.vutbr.cz>
 *
 * @section DESCRIPTION
 *
 * Precedence syntax analysis of expresions in IFJ15 language
 */

#include <precedence_analysis.h>


const int precedence_table[NUM_OF_TOKENS][NUM_OF_TOKENS] = 
{//        id  ==   >   <   >=  <=  +   -   *   /   !=  )   (   $
 /* id */ { E,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  E,  R },
 /* == */ { S,  R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R },
 /* >  */ { S,  R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R },  
 /* <  */ { S,  R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R }, 
 /* >= */ { S,  R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R }, 
 /* <= */ { S,  R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R }, 
 /* +  */ { S,  R,  R,  R,  R,  R,  R,  R,  S,  S,  R,  R,  S,  R }, 
 /* -  */ { S,  R,  R,  R,  R,  R,  R,  R,  S,  S,  R,  R,  S,  R }, 
 /* *  */ { S,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  S,  R }, 
 /* /  */ { S,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  S,  R }, 
 /* != */ { S,  R,  R,  R,  R,  R,  S,  S,  S,  S,  R,  R,  S,  R }, 
 /* )  */ { E,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  R,  E,  R }, 
 /* (  */ { S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  H,  S,  E }, 
 /* $  */ { S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  S,  E,  S,  E }
};
