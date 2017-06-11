#include "def.h"

int main() {
	int slave_tid = pvm_mytid();	
	int master_tid;
	int slave_ind;

	if (pvm_recv(-1, MSG_WHERE_ARE_YOU)) {
		pvm_upkint(&master_tid, 1, 1);
		pvm_upkint(&slave_ind, 1, 1);
		printf("I am %d slave, my tid is %d. I've got a message from %d\n", 
			slave_ind, slave_tid, master_tid);

		// prepare hostname for sending
		char *hostname = malloc(HOSTNAME_LENGTH);
		gethostname(hostname, HOSTNAME_LENGTH);
		hostname[HOSTNAME_LENGTH] = 0;
	
		pvm_initsend(PvmDataDefault);	
		pvm_pkint(&slave_ind, 1, 1);
		pvm_pkstr(hostname);
		pvm_send(master_tid, MSG_I_AM_HERE);
	}

	pvm_exit();
}