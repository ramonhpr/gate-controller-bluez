#ifndef __ADAPTER_H
#define __ADAPTER_H

struct adapter_t {
	const char *name;
	const char *address;
	bool powered;
	struct l_dbus_proxy *proxy;
} adapter;


int get_adapter_properties();

int update_adapter_properties(struct l_dbus_message *msg, const char* name_property);

bool power_adapter_on();
#endif
