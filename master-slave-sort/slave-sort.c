#include "def.h"

int find_min_ind(int array[], int length);
int find_max_ind(int array[], int length);
int sorting_between_slaves_not_ended();

int main() {

	int my_tid = pvm_mytid();

	// join group
	int my_gid = pvm_joingroup(GROUP_NAME);	
	pvm_barrier(GROUP_NAME, SLAVE_COUNT);
		
	// generate array
	int array[SLAVE_ARRAY_SIZE];
	srand(my_tid);
	int i;
	for (i = 0; i < SLAVE_ARRAY_SIZE; i++) {
		array[i] = rand() % MAX_NUMBER;
	}
	
	printf("\nSlave %d: array %s initial\n"
		, my_gid, format_int_array(array, SLAVE_ARRAY_SIZE)); 
	

	do {
		// 1 = no exchanges
		// 0 = some exchange with neighbour
		int slave_ready = 1;
		
		int min_ind = find_min_ind(array, SLAVE_ARRAY_SIZE);
		int min = array[min_ind];
		
		int max_ind = find_max_ind(array, SLAVE_ARRAY_SIZE);
		int max = array[max_ind];

		int left_neighbour_tid;
		int right_neighbour_tid;
		
		// send min to left neighbor
		if (my_gid > 0) {
			left_neighbour_tid = pvm_gettid(GROUP_NAME, my_gid-1);

			pvm_initsend(PvmDataDefault);
			pvm_pkint(&min, 1, 1);
			pvm_send(left_neighbour_tid, MSG_MIN);
		}

		// send max to right neighbor
		if (my_gid < pvm_gsize(GROUP_NAME) - 1) {
			right_neighbour_tid = pvm_gettid(GROUP_NAME, my_gid+1);

			pvm_initsend(PvmDataDefault);
			pvm_pkint(&max, 1, 1);
			pvm_send(right_neighbour_tid, MSG_MAX);
		}

		// receive max from left neighbour
		if (my_gid > 0) {
			int left_neighbour_max;
			pvm_recv(left_neighbour_tid, MSG_MAX);
			pvm_upkint(&left_neighbour_max, 1, 1);

			if (min < left_neighbour_max) {

				printf(
					"Slave %d: array %s %d => %d\n"
					, my_gid, format_int_array(array, SLAVE_ARRAY_SIZE)
					, min, left_neighbour_max); 

				// replace values and set exchanges flag
				array[min_ind] = left_neighbour_max;
				slave_ready = 0;
			}
		}

		// receive min from right neighbour
		if (my_gid < pvm_gsize(GROUP_NAME) - 1) {
			int right_neighbour_min;
			pvm_recv(right_neighbour_tid, MSG_MIN);
			pvm_upkint(&right_neighbour_min, 1, 1);

			if (max > right_neighbour_min) {
				printf(
					"Slave %d: array %s %d => %d\n"
					, my_gid, format_int_array(array, SLAVE_ARRAY_SIZE)
					, max, right_neighbour_min); 
				
				// replace values and set exchanges flag
				array[max_ind] = right_neighbour_min;
				slave_ready = 0;
			}
		}

		// inform parent if there were exchanges this loop
		pvm_initsend(PvmDataDefault);
		pvm_pkint(&slave_ready, 1, 1);
		pvm_send(pvm_parent(), MSG_SLAVE_STATUS);		

	} while (sorting_between_slaves_not_ended());

	printf("Slave %d: array %s after changes\n"
		, my_gid, format_int_array(array, SLAVE_ARRAY_SIZE));

	// receive local sorting command and perform it
	pvm_recv(-1, MSG_SORT_LOCAL_ARRAYS);
	sort_int_array(array, SLAVE_ARRAY_SIZE);

	printf("Slave %d: array %s sorted\n"
		, my_gid, format_int_array(array, SLAVE_ARRAY_SIZE));

	// send sorted array to master
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&my_gid, 1, 1);
	for (i = 0; i < SLAVE_ARRAY_SIZE; i++) {
		pvm_pkint(&array[i], 1, 1);
	}
	pvm_send(pvm_parent(), MSG_SORTED_LOCAL_ARRAY);

	// exit
	pvm_exit();
}

int find_min_ind(int array[], int length) {
	int min_ind = 0;
	int min = array[0];

	int i;
	for (i = 0; i < length; i++) {
		if (array[i] < min) {
			min = array[i];
			min_ind = i;
		}
	}

	return min_ind;
}

int find_max_ind(int array[], int length) {
	int max_ind = 0;
	int max = array[0];

	int i;
	for (i = 0; i < length; i++) {
		if (array[i] > max) {
			max = array[i];
			max_ind = i;
		}
	}

	return max_ind;
}

int sorting_between_slaves_not_ended() {	
	int status = 0;

	if (pvm_recv(-1, MSG_SORTING_BETWEEN_SLAVES_STATUS)) {
		pvm_upkint(&status, 1, 1);
	}

	return status;
}