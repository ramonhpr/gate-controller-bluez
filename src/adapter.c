#include <ell/ell.h>
#include <stdbool.h>
#include "adapter.h"

int get_adapter_properties()
{
	if (!l_dbus_proxy_get_property(adapter.proxy, "Address", "s", &adapter.address))
		return -1;

	if (!l_dbus_proxy_get_property(adapter.proxy, "Name", "s", &adapter.name))
		return -1;

	if (!l_dbus_proxy_get_property(adapter.proxy, "Alias", "s", &adapter.alias))
		return -1;

	if (!l_dbus_proxy_get_property(adapter.proxy, "Powered", "b", &adapter.powered))
		return -1;

	if (!l_dbus_proxy_get_property(adapter.proxy, "Discoverable", "b", &adapter.discoverable))
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

	if (!strcmp(name_property,"Alias")) {
		if (!l_dbus_message_get_arguments(msg, "s", &adapter.alias))
			return -1;
		l_info("   Alias: %s", adapter.alias);
	}

	if (!strcmp(name_property,"Powered")) {
		if (!l_dbus_message_get_arguments(msg, "b", &adapter.powered))
			return -1;
		l_info("   Powered: %d", adapter.powered);
	}

	if (!strcmp(name_property,"Discoverable")) {
		if (!l_dbus_message_get_arguments(msg, "b", &adapter.discoverable))
			return -1;
		l_info("   Discoverable: %d", adapter.discoverable);
	}


	return 0;
}

static void cb_powered_on(struct l_dbus_proxy *proxy, struct l_dbus_message *result, void *user_data)
{
	const char *msg, *txt;
	if (l_dbus_message_is_error(result)) {
		if (l_dbus_message_get_error(result, &msg, &txt))
			l_error("%s: %s",msg, txt);
	} else {
		l_info("Powered on");
	}
}

static void cb_discoverable(struct l_dbus_proxy *proxy, struct l_dbus_message *result, void *user_data)
{
	const char *msg, *txt;
        if (l_dbus_message_is_error(result)) {
                if (l_dbus_message_get_error(result, &msg, &txt))
                        l_error("%s: %s",msg, txt);
        } else {
                l_info("%s is visible", adapter.name);
        }
}

static void cb_name_changed(struct l_dbus_proxy *proxy, struct l_dbus_message *result, void *user_data)
{
	const char *msg, *txt;
        if (l_dbus_message_is_error(result)) {
                if (l_dbus_message_get_error(result, &msg, &txt))
                        l_error("%s: %s",msg, txt);
        } else {
                l_info("Name changed");
        }
}



bool power_adapter_on()
{
	if (adapter.proxy)
		if (!l_dbus_proxy_set_property(adapter.proxy, cb_powered_on, NULL, NULL, "Powered", "b", true))
			return false;

	return true;
}

bool make_visible()
{
	if (adapter.proxy)
		if (!l_dbus_proxy_set_property(adapter.proxy, cb_discoverable, NULL, NULL, "Discoverable", "b", true))
			return false;
	return true;
}

bool change_name(const char* new_name)
{
	if (adapter.proxy)
		if (!l_dbus_proxy_set_property(adapter.proxy, cb_name_changed, NULL, NULL, "Alias", "s", new_name))
			return false;
	return true;
}
