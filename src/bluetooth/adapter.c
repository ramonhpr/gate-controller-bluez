#include <ell/ell.h>
#include <stdbool.h>
#include "adapter.h"

typedef struct {
	const char* name;
	const char* signature;
	union {
		void *value;
		void **vector;
	} ref; // This is a pointer to the related property in adapter model
} property_t ;

static property_t vproperty[] = {
	{ "Address", "s" , { .vector = (void**)&adapter.address } },
	{ "Name", "s" , { .vector = (void**)&adapter.name } },
	{ "Alias", "s" , { .vector = (void**)&adapter.alias } },
	{ "Powered", "b", { .value = (void*)&adapter.powered } },
	{ "Discoverable", "b", { .value = (void*)&adapter.discoverable } },
	{ NULL, NULL, { NULL, NULL} }
};

static void print_property(int i)
{
	if (!strcmp(vproperty[i].signature, "s")) {
		l_info("   %s: %s", vproperty[i].name, (char*)*vproperty[i].ref.vector);
	} else if (!strcmp(vproperty[i].signature, "b")) {
		if (*((bool*)vproperty[i].ref.value))
			l_info("   %s: on", vproperty[i].name);
		else
			l_info("   %s: off", vproperty[i].name);
	}
}

int get_adapter_properties()
{
	uint8_t i;
	for (i = 0 ; vproperty[i].name != NULL ; i++) {
		if (!strcmp(vproperty[i].signature, "s")) {
			if (!l_dbus_proxy_get_property(adapter.proxy, vproperty[i].name, vproperty[i].signature, &(*vproperty[i].ref.vector)))
				return -1;
		} else if (!strcmp(vproperty[i].signature, "b")) {
			if (!l_dbus_proxy_get_property(adapter.proxy, vproperty[i].name, vproperty[i].signature, vproperty[i].ref.value))
				return -1;
		}
	}
	for (i = 0 ; vproperty[i].name != NULL ; i++)
		print_property(i);
	return 0;
}

int update_adapter_properties(struct l_dbus_message *msg, const char* name_property)
{
	uint8_t i;

	for (i = 0; vproperty[i].name != NULL ; i++) {
		if (!strcmp(name_property, vproperty[i].name)) {
			if (!strcmp(vproperty[i].signature, "s")) {
				if (!l_dbus_message_get_arguments(msg, vproperty[i].signature, &(*vproperty[i].ref.vector)))
					return -1;
			} else if (!strcmp(vproperty[i].signature, "b")) {
				if (!l_dbus_message_get_arguments(msg, vproperty[i].signature, vproperty[i].ref.value ))
					return -1;
			}
			print_property(i);
			return 0;
		}
	}

	return 0;
}

static void cb_powered_on(struct l_dbus_proxy *proxy, struct l_dbus_message *result, void *user_data)
{
	const char *msg, *txt;
	if (l_dbus_message_is_error(result)) {
		if (l_dbus_message_get_error(result, &msg, &txt))
			l_error("%s: %s",msg, txt);
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

bool power_adapter_off()
{
	if (adapter.proxy)
		if (!l_dbus_proxy_set_property(adapter.proxy, cb_powered_on, NULL, NULL, "Powered", "b", false))
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
