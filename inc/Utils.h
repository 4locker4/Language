/// @file Utils.h

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "Asserts.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define RESET   "\x1b[0m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MANGETA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define STRANGE "\x1b[1;35;7m"

#define COLOR_PRINT(color, ...)  {printf (color __VA_ARGS__); printf (RESET);}              ///< Colorful print

#define COLOR_FPRINT(file, color, ...) {fprintf (file, color __VA_ARGS__); fprintf (file, RESET);}

const double EPSILON = 1e-10;                                                               ///< Error constant double

bool   IsZero                   (const double val);                                         ///< Double error check
bool   DoubleComparison         (const double firstNum, const double secondNum);            ///< Checking equality double
void   BufferCleaner            ();                                                         ///< Buffer cleaner
bool   SkipSpaces               ();                                                         ///< Skip spaces and tabs after User`s text, returns first char
int    StrCounter               (const char * fileFirstElement);                            ///< Count quantity of tests
void   TestArrayReturner        (int fileSize, char * fileFirstElement);                    ///< Put values into array of struct (test data array)
char * FileToStr                (const char * file_directory, size_t * file_size);          ///< Return string with all file`s text
size_t ReadFileSize             (const char * file_directory);                              ///< Return fili size
int    Factorial                (int num);                                                  ///< Return num factorial
char * DeletSeveralSpacesInRow  (char * text, size_t text_size);                            ///< Return string without double spaceses


#endif ///not UTILS_H
