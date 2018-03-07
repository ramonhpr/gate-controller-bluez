#include <ell/ell.h>
#include <stdbool.h>
#include "option.h"
#include "manager.h"
#include "dbus.h"
#include "bluetooth/bluez_client.h"
#include "bluetooth/adapter.h"

static bool print_once = true;

#define PRINT_ONCE(x) if(print_once){ l_info("[STATE]:"x);print_once=false;}

struct l_timeout *timeout_saving_power = NULL;

static int8_t update_state(fsm_t* fsm, uint8_t next_state) {
	if (next_state >= UNKNOW_STATE)
		return -EINVAL;
	fsm->cur_state = next_state;

	// Update this for debug messages
	print_once = true;
	return 0;
}
// TODO: make an option to power saving
static void handle_power_saving(struct l_timeout* timeout, void* user_data) {
	fsm_t *fsm = user_data;
	if (fsm) {
		if (fsm->cur_state == SCANNING) {
			if (enable_debug)
				l_debug("Timeout scaning!");
			update_state(fsm, IDLE);
			l_timeout_remove(timeout);
			timeout_saving_power = l_timeout_create(TIME_SECONDS_IDLE, handle_power_saving, fsm, NULL);
		} else if (fsm->cur_state == IDLE) {
			if (enable_debug)
				l_debug("Timeout idle!");
			update_state(fsm, SCANNING);
			l_timeout_remove(timeout);
			timeout_saving_power = l_timeout_create(TIME_SECONDS_SCANNING, handle_power_saving, fsm, NULL);
		}
	} else {
		l_error("Error in parameter received");
		exit(1);
	}
}
void manager_state() {
	static fsm_t fsm = { SETUP };
	switch (fsm.cur_state) {
		case SETUP:
			if (enable_debug)
				PRINT_ONCE("Setup");
			dbus_init();
			client_init();
			update_state(&fsm, IDLE);
			break;
		case IDLE:
			if (enable_debug)
				PRINT_ONCE("Idle");
			if (!timeout_saving_power)
				timeout_saving_power = l_timeout_create(TIME_SECONDS_IDLE, handle_power_saving, &fsm, NULL);
			break;
		case SCANNING:
			if (enable_debug)
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
	client_exit();
	dbus_exit();
	l_timeout_remove(timeout_saving_power);
}
