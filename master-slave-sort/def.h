#include "pvm3.h"
#include "stdlib.h"
#include "stdio.h"
#include "array-utils.c"

#define GROUP_NAME "SORTING_GROUP"

#define SLAVE_NAME "slave-sort"
#define SLAVE_COUNT 5
#define SLAVE_ARRAY_SIZE 3

#define MAX_NUMBER 100

#define MSG_SORTING_BETWEEN_SLAVES_STATUS 201
#define MSG_SORT_LOCAL_ARRAYS 202
#define MSG_SORTED_LOCAL_ARRAY 203
#define MSG_SLAVE_STATUS 204
#define MSG_MIN 205
#define MSG_MAX 206