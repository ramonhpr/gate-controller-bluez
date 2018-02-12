#include <config.h>
#include <ell/ell.h>
#include "option.h"
#include "dbus.h"
#include "bluez_client.h"
#include "manager.h"

static struct l_timeout *timeout;
static struct l_idle *idle;
static bool terminating;

static void main_loop_quit(struct l_timeout *timeout, void *user_data)
{
        l_main_quit();
}

static void terminate(void)
{
        if (terminating)
                return;

        terminating = true;
	manager_exit();
	l_idle_remove(idle);
        timeout = l_timeout_create(1, main_loop_quit, NULL, NULL);
}

static void signal_handler(struct l_signal *signal, uint32_t signo,
                                                        void *user_data)
{
        switch (signo) {
        case SIGINT:
        case SIGTERM:
                terminate();
                break;
        }
}

static void idle_handler(struct l_idle *idle, void* user_data) {
	manager_state();
}
int main (int argc, char* argv[])
{
	struct l_signal *sig;
	sigset_t mask;
	if (process_options(argc,argv))
		return EXIT_FAILURE;
	if (!l_main_init())
		return EXIT_FAILURE;

	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
	sigaddset(&mask, SIGTERM);

	sig = l_signal_create(&mask, signal_handler, NULL, NULL);

	l_log_set_stderr();
	l_debug_enable("*");
	l_info("Gate controller daemon version %s", VERSION);

	dbus_init();
	client_init();
	idle = l_idle_create(idle_handler, NULL, NULL);

	l_main_run();

	client_exit();
	dbus_exit();
	l_signal_remove(sig);
	l_main_exit();
	return 0;
}
