#include "def.h"

void check_and_update(int* global_sorting_status);
void send_status();
void sort_local_arrays_on(int slave_tids[]);
void show_joined(int slave_arrays[SLAVE_COUNT][SLAVE_ARRAY_SIZE]);
void put_slaves_arrays_in(int slave_arrays[SLAVE_COUNT][SLAVE_ARRAY_SIZE]);

int main(int argc, char **argv) {

	// spawn slaves
	int slave_tids[SLAVE_COUNT];

	int slaves_spawned = pvm_spawn(
		SLAVE_NAME, NULL, PvmTaskDefault, "", SLAVE_COUNT, slave_tids);
	
	// 1 = sorting continues
	// 0 = sorting ended	
	int global_sorting_status = 1;
	do {			
		check_and_update(&global_sorting_status);					
		send_status(slave_tids, global_sorting_status);
	} while (global_sorting_status);
	
	sort_local_arrays_on(slave_tids);
	int slave_arrays[SLAVE_COUNT][SLAVE_ARRAY_SIZE];
	put_slaves_arrays_in(slave_arrays);
	
	show_joined(slave_arrays);
	pvm_exit();
}

void check_and_update(int* global_sorting_status) {
	*global_sorting_status = 0;	
	int i;
	for (i = 0; i < SLAVE_COUNT; i++) {
		int slave_ready;
		pvm_recv(-1, MSG_SLAVE_STATUS);
		pvm_upkint(&slave_ready, 1, 1);
		if (slave_ready == 0) {
			*global_sorting_status = 1;			
		}
	}
}

void send_status(int slave_tids[], int global_sorting_status) {
	int i;
	for (i = 0; i < SLAVE_COUNT; i++) {
		pvm_initsend(PvmDataDefault);
		pvm_pkint(&global_sorting_status, 1, 1);
		pvm_send(slave_tids[i], MSG_SORTING_BETWEEN_SLAVES_STATUS);
	}
}

void sort_local_arrays_on(int slave_tids[]) {
	int i;
	for (i = 0; i < SLAVE_COUNT; i++) {
		pvm_initsend(PvmDataDefault);		
		pvm_send(slave_tids[i], MSG_SORT_LOCAL_ARRAYS);
	}
}

void show_joined(int slave_arrays[SLAVE_COUNT][SLAVE_ARRAY_SIZE]) {
	printf("\nMaster: joined sorted slaves arrays:\n");
	int i;
	for (i = 0; i < SLAVE_COUNT; i++) {
		printf("%s ", format_int_array(slave_arrays[i], SLAVE_ARRAY_SIZE));
	}
	printf("\n\n");
}

void put_slaves_arrays_in(int slave_arrays[SLAVE_COUNT][SLAVE_ARRAY_SIZE]) {
	int i, j;
	for (i = 0; i < SLAVE_COUNT; i++) {
		int slave_gid;		
		
		pvm_recv(-1, MSG_SORTED_LOCAL_ARRAY);
		pvm_upkint(&slave_gid, 1, 1);

		for (j = 0; j < SLAVE_ARRAY_SIZE; j++) {
			pvm_upkint(&slave_arrays[slave_gid][j], 1, 1);
		}		
	}
}
