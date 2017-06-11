#include "def.h"

int main() {
	// get parameters from master	
	int my_ind;
	int r;
	int repeats;

	int rec_result = pvm_recv(-1, MSG_TASK_DETAILS);
	if (!rec_result) {
		printf("Error. Couldn't receive message with tag %d!\n", MSG_TASK_DETAILS);
		exit(1);
	}

	pvm_upkint(&my_ind, 1, 1);
	pvm_upkint(&r, 1, 1);
	pvm_upkint(&repeats, 1, 1);

	printf(
		"%d slave: "
		"Master told me R = %d and "
		"repeats = %d\n", my_ind, r, repeats);

	// count points inside circle
	int i;
	int inside_circle_points = 0;

	srand(pvm_mytid());
	for (i = 0; i < repeats; i++) {
		int x = rand() % r;
		int y = rand() % r;
		if (x*x + y*y <= r*r) {
			inside_circle_points++;
		}
	}
	
	// send results to master
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&my_ind, 1, 1);
	pvm_pkint(&inside_circle_points, 1, 1);
	pvm_send(pvm_parent(), MSG_INSIDE_CIRCLE_POINTS);

	// exit
	pvm_exit();
}