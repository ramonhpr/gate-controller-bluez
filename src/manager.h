#include <stdint.h>
#include <errno.h>

#define TIME_SECONDS_IDLE 	2
#define TIME_SECONDS_SCANNING 	10
#define TIME_MSECONDS_IDLE 	(1000*TIME_SECONDS_IDLE)
#define TIME_MSECONDS_SCANNING  (1000*TIME_SECONDS_SCANNING)

enum state {
	SETUP,
	IDLE,
	SCANNING,
	PAIRED,
	SENDING_RF_DATA,
	ACK_RESPONSE_BLUEZ,
	DISCONNECT,
	UNKNOW_STATE
};

typedef struct fsm {
	uint8_t cur_state; // Current state
} fsm_t;

void manager_state();
void manager_exit();
