///@file Asserts.h
#ifndef ASSERTS_H
#define ASSERTS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define my_assert(expr) if (!(expr))\
                        {\
                            printf ("ERROR: \"" #expr "\", file %s, function %s, line %d\n", \
                                        __FILE__, __func__, __LINE__);\
                            exit (1);\
                        }

#define CHECKED_(expr) if (!(expr))\
                            {\
                            COLOR_PRINT (RED, "ERROR, your error`s number is: <%d>\n",\
                                              "Information:\n",\
                                              "File: <%s>           Line: <%d>\n",\
                                              expr, __FILE__, __LINE__)}




#endif