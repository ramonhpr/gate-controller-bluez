#include "dbus.h"
#include "option.h"
#include <ell/ell.h>

static void do_debug(const char *str, void *user_data)
{
        l_info("%s", str);
}

static void ready_callback(void *user_data)
{
        l_info("ready");

	if (!l_dbus_object_manager_enable(p_dbus))
                l_error("Unable to register the ObjectManager");
}

static void disconnect_callback(void *user_data)
{
        l_main_quit();
}

static void signal_message(struct l_dbus_message *message, void *user_data)
{
        const char *path, *interface, *member, *destination, *sender;

        path = l_dbus_message_get_path(message);
        destination = l_dbus_message_get_destination(message);

        l_info("path=%s destination=%s", path, destination);

        interface = l_dbus_message_get_interface(message);
        member = l_dbus_message_get_member(message);

        l_info("interface=%s member=%s", interface, member);

        sender = l_dbus_message_get_sender(message);

        l_info("sender=%s", sender);

        if (!strcmp(member, "NameOwnerChanged")) {
                const char *name, *old_owner, *new_owner;

                if (!l_dbus_message_get_arguments(message, "sss",
                                        &name, &old_owner, &new_owner))
                        return;

                l_info("name=%s old=%s new=%s", name, old_owner, new_owner);
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

bool dbus_exit() {
        l_dbus_destroy(p_dbus);
	return true;
}
