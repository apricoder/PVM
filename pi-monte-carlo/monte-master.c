#include "def.h"

int main(int argc, char **argv) {

	// get parameters
	int r = atoi(argv[1]);
	int slaves_count = atoi(argv[2]);
	int slave_all_points = atoi(argv[3]);

	// spawn slaves
	int executed_task_num;
	int slave_tids[slaves_count];

	executed_task_num = pvm_spawn(
		SLAVENAME, NULL, PvmTaskDefault, "", slaves_count, slave_tids);
	printf("Master: %d slaves spawned successfully!\n", executed_task_num);

	// send parameters to slaves
	int i;
	for (i = 0; i < slaves_count; i++) {
		pvm_initsend(PvmDataDefault);
		pvm_pkint(&i, 1, 1);
		pvm_pkint(&r, 1, 1);
		pvm_pkint(&slave_all_points, 1, 1);
		pvm_send(slave_tids[i], MSG_TASK_DETAILS);
	}
	
	// wait for computed answer
	for (i = 0; i< slaves_count; i++) {

		int rec_result = pvm_recv(-1, MSG_INSIDE_CIRCLE_POINTS);
		if (!rec_result) {
			printf("Master: Error. Couldn't receive message with tag %d!\n", MSG_TASK_DETAILS);
		}

		int slave_ind;
		int slave_inside_circle_points;
		double pi;

		pvm_upkint(&slave_ind, 1, 1);
		pvm_upkint(&slave_inside_circle_points, 1, 1);		
		printf("Master: %d slave told me %d points of %d were inside circle\n"
		, slave_ind, slave_inside_circle_points, slave_all_points);
		
		pi = 4.0 * ((double)slave_inside_circle_points) 
				 / ((double)slave_all_points);
		printf("Master: pi = %f based on %d slave data\n"
		, pi, slave_ind);
	}

	// exit
	pvm_exit();
}