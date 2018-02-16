#include <ell/ell.h>
#include <stdbool.h>
#include "adapter.h"

int get_adapter_properties()
{
	if (!l_dbus_proxy_get_property(adapter.proxy, "Address", "s", &adapter.address))
		return -1;

	if (!l_dbus_proxy_get_property(adapter.proxy, "Name", "s", &adapter.name))
		return -1;

	if (!l_dbus_proxy_get_property(adapter.proxy, "Powered", "b", &adapter.powered))
		return -1;
	return 0;
}

int update_adapter_properties(struct l_dbus_message *msg, const char* name_property)
{
	if (!strcmp(name_property,"Address")) {
		if (!l_dbus_message_get_arguments(msg, "s", &adapter.address))
			return -1;
		l_info("   Address: %s", adapter.address);
	}

	if (!strcmp(name_property,"Name")){
		if (!l_dbus_message_get_arguments(msg, "s", &adapter.name))
			return -1;
		l_info("   Name: %s", adapter.name);
	}

	if (!strcmp(name_property,"Powered")) {
		if (!l_dbus_message_get_arguments(msg, "b", &adapter.powered))
			return -1;
		l_info("   Powered: %d", adapter.powered);
	}

	return 0;
}

static void cb_powered_on(struct l_dbus_proxy *proxy, struct l_dbus_message *result, void *user_data) { l_dbus_message_set_arguments(result, "b", user_data); l_info("Powered on"); }

bool power_adapter_on()
{
	bool _true = true;
	if (adapter.proxy)
		if (!l_dbus_proxy_set_property(adapter.proxy, cb_powered_on, &_true, NULL, "Powered", "b"))
			return false;

	return true;
}
