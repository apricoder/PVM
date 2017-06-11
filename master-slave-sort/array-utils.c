#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* format_int_array(int array[], int size) {

	int joined_size = 3;	// '[' + ' ' + ... ']';
	char* joined;
	char* c_array[size];

    int i;
    for (i = 0; i < size; i++)
    {   
        char c[sizeof(int)];    
        snprintf(c, sizeof(int), "%d", array[i]);

        c_array[i] = malloc(sizeof(c)); 
        strcpy(c_array[i], c); 

        joined_size += sizeof(c) + 1;	// + sizeof(' ');
    }  

    joined = malloc(joined_size + 1);
    strcpy(joined, "[ ");    
    for (i = 0; i < size; i++) {
    	strcat(joined, c_array[i]);
    	strcat(joined, " ");
    }
    strcat(joined, "]");

    return joined;
}

void sort_int_array(int array[], int size) {
    int i, j;
    for (i = 0; i < size - 1; i++) {
        int min_ind = i;
        for (j = i + 1; j < size; j++) {
            if (array[j] < array[min_ind]) {
                min_ind = j;
            }
        }
        int tmp = array[i];
        array[i] = array[min_ind];
        array[min_ind] = tmp;
    }
}

// int main(int argc, char** argv) {

// 	int array[argc];

// 	int size = argc - 1 ;
//     int* data  = (int*)calloc( size, sizeof(int) ) ;

//     int i;
//     for(i = 0; i < size; i++) {        
//         sscanf(argv[i + 1], "%d", &data[i]);               
//     }

//     sort_int_array(data, size);
//     printf("Array: %s\n", format_int_array(data, size));

// 	return 0;
// }