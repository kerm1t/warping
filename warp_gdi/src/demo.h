#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100
#define STR_ENDS_WITH(S, E) (strcmp(S + strlen(S) - (sizeof(E)-1), E) == 0)
#define USE_BITBLT