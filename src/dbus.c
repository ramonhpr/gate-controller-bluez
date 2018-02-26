#include "dbus.h"
#include "option.h"
#include <ell/ell.h>

struct l_timeout *exit_timeout;

static void do_debug(const char *str, void *user_data)
{
        l_info("%s", str);
}

static void ready_callback(void *user_data)
{
        l_info("Ready");

	if (!l_dbus_object_manager_enable(p_dbus))
                l_error("Unable to register the ObjectManager");
}

static void disconnect_callback(void *user_data)
{
       l_info("Dbus disconnected");
}

static void signal_message(struct l_dbus_message *message, void *user_data)
{
        const char *path, *interface, *member, *destination, *sender, *msg, *txt;

        path = l_dbus_message_get_path(message);
        destination = l_dbus_message_get_destination(message);

	if (l_dbus_message_is_error(message))
		if (l_dbus_message_get_error(message, &msg, &txt))
			l_error("%s: %s", msg, txt);

	if (verbose)
	        l_info("Path=%s Destination=%s", path, destination);

        interface = l_dbus_message_get_interface(message);
        member = l_dbus_message_get_member(message);

	if (verbose)
	        l_info("Interface=%s Member=%s", interface, member);

        sender = l_dbus_message_get_sender(message);

	if (verbose)
	        l_info("Sender=%s", sender);

        if (!strcmp(member, "NameOwnerChanged")) {
                const char *name, *old_owner, *new_owner;

                if (!l_dbus_message_get_arguments(message, "sss",
                                        &name, &old_owner, &new_owner))
                        return;
		if (verbose)
			l_info("Name=%s Old=%s New=%s", name, old_owner, new_owner);
        }
}

bool dbus_init() {
	p_dbus = l_dbus_new_default(L_DBUS_SYSTEM_BUS);
	if (enable_debug)
		l_dbus_set_debug(p_dbus, do_debug, "[DBUS] ", NULL);
	l_dbus_set_ready_handler(p_dbus, ready_callback, p_dbus, NULL);
	l_dbus_set_disconnect_handler(p_dbus, disconnect_callback, NULL, NULL);
	l_dbus_register(p_dbus, signal_message, NULL, NULL);

	return true;
}

static void cb_destroy(struct l_timeout* timeout, void* user_data)
{
        l_dbus_destroy(p_dbus);
	l_timeout_remove(exit_timeout);
}


bool dbus_exit() {
        exit_timeout = l_timeout_create_ms(10, cb_destroy, NULL, NULL);
	return true;
}
