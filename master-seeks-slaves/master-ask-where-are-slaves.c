#include "def.h"

int main() {

	// introduce master
	int master_tid = pvm_mytid();
	printf("I am master and my tid is %d\n", master_tid);
	printf("Where are you my slaves?\n");

	int slave_tids[SLAVECOUNT];
	int executed_tasks_num;

	// create new slave processes
	executed_tasks_num = pvm_spawn(SLAVENAME, NULL, PvmTaskDefault, "", SLAVECOUNT, slave_tids);

	// ask each slave where it is
	int i;
	for (i = 0; i < executed_tasks_num; i++)	{
		pvm_initsend(PvmDataDefault);
		pvm_pkint(&master_tid, 1, 1);
		pvm_pkint(&i, 1, 1);
		pvm_send(slave_tids[i], MSG_WHERE_ARE_YOU);
	}

	// wait for answer
	for (i = 0; i < executed_tasks_num; i++) {		
		if (pvm_recv(-1, MSG_I_AM_HERE)) {
			int slave_ind;
			char *hostname = malloc(HOSTNAME_LENGTH);

			pvm_upkint(&slave_ind, 1, 1);
			pvm_upkstr(hostname);
			printf("%d slave told me he is on %s\n", slave_ind, hostname);
		}
	} 
	// exit
	pvm_exit();
}