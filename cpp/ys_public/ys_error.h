#ifndef YS_ERROR_H 
#define YS_ERROR_H 

#include <stdio.h>
#include <stdlib.h>

#define ERR_EXIT(m) \
        do \
        { \
            perror(m); \
            exit(EXIT_FAILURE); \
        } while (0);
        

#endif	// YS_ERROR_H