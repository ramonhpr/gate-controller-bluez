#include <ell/ell.h>
#include <stdbool.h>
#include "manager.h"

static bool print_once = true;

#define PRINT_ONCE(x) if(print_once){ l_info(x);print_once=false;}

struct l_timeout *timeout_saving_power = NULL;

static int8_t update_state(fsm_t* fsm, uint8_t next_state) {
	if (next_state >= UNKNOW_STATE)
		return -EINVAL;
	fsm->cur_state = next_state;
	return 0;
}
// TODO: make an option to power saving
static void handle_power_saving(struct l_timeout* timeout, void* user_data) {
	fsm_t *fsm = user_data;
	if (fsm) {
		if (fsm->cur_state == SCANNING) {
			l_debug("timeout scaning!");
			update_state(fsm, IDLE);
			l_timeout_remove(timeout);
			timeout_saving_power = l_timeout_create(TIME_SECONDS_IDLE, handle_power_saving, fsm, NULL);
		} else if (fsm->cur_state == IDLE) {
			l_debug("Timeout idle!");
			update_state(fsm, SCANNING);
			l_timeout_remove(timeout);
			timeout_saving_power = l_timeout_create(TIME_SECONDS_SCANNING, handle_power_saving, fsm, NULL);
		}
	} else {
		l_error("Error in parameter received");
		exit(1);
	}
	print_once = true;
}
void manager_state() {
	static fsm_t fsm = { IDLE };
	switch (fsm.cur_state) {
		case IDLE:
			PRINT_ONCE("Idle");
			if (!timeout_saving_power)
				timeout_saving_power = l_timeout_create(TIME_SECONDS_IDLE, handle_power_saving, &fsm, NULL);
			break;
		case SCANNING:
			PRINT_ONCE("Scanning");
			if (!timeout_saving_power)
				timeout_saving_power = l_timeout_create(TIME_SECONDS_SCANNING, handle_power_saving, &fsm, NULL);
			// TODO: scan devices over bluetooth
			break;
		case PAIRED:
			// TODO: handle devices paired
			break;
		case SENDING_RF_DATA:
			// TODO: send signal over RF
			break;
		case ACK_RESPONSE_BLUEZ:
			// TODO: handle any response from bluetooth
			break;
		case DISCONNECT:
			// TODO: handle disconnecting from devices
			break;
		case UNKNOW_STATE:
			break;
		default:
			exit(1);
			break;
	}
}

void manager_exit() {
	l_timeout_remove(timeout_saving_power);
}
