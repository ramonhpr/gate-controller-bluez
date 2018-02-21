#include <ell/ell.h>
#include <stdbool.h>
#include "bluez_client.h"
#include "adapter.h"

struct l_timeout* timeout_powered = NULL;

static void verify_adapter_powered(struct l_timeout* timeout, void* user_data)
{
	if (!adapter.powered)
		power_adapter_on();
	if (!adapter.discoverable)
		make_visible();
	if (timeout_powered)
		l_timeout_remove(timeout_powered);
	timeout_powered = NULL;
}

static void bluez_client_connected(struct l_dbus *dbus, void *user_data)
{
        l_info("client connected");
}

static void bluez_client_disconnected(struct l_dbus *dbus, void *user_data)
{
        l_info("client disconnected");
}

static void bluez_client_ready(struct l_dbus_client *client, void *user_data)
{
        l_info("client ready");
	timeout_powered = l_timeout_create_ms(100, verify_adapter_powered, NULL, NULL);
	change_name(NAME_BEACON);
}

static void proxy_added(struct l_dbus_proxy *proxy, void *user_data)
{
        const char *interface = l_dbus_proxy_get_interface(proxy);
        const char *path = l_dbus_proxy_get_path(proxy);

        l_info("proxy added: %s %s", path, interface);

	if (!strcmp(interface, "org.bluez.Adapter1") ) {
                adapter.proxy = proxy;
		if (get_adapter_properties() < 0)
			return;

		l_info("   Address: %s", adapter.address);
		l_info("   Name: %s", adapter.name);
		l_info("   Alias: %s", adapter.alias);
		l_info("   Powered: %d", adapter.powered);
		l_info("   Discoverable: %d", adapter.discoverable);
	}

}

static void proxy_removed(struct l_dbus_proxy *proxy, void *user_data)
{
        l_info("proxy removed: %s %s", l_dbus_proxy_get_path(proxy),
                                        l_dbus_proxy_get_interface(proxy));
}

static void property_changed(struct l_dbus_proxy *proxy, const char *name,
                                struct l_dbus_message *msg, void *user_data)
{
	const char *interface = l_dbus_proxy_get_interface(proxy);
        l_info("property changed: %s (%s %s)", name,
                                        l_dbus_proxy_get_path(proxy),
                                        interface);
	if (!timeout_powered)
		timeout_powered = l_timeout_create_ms(100, verify_adapter_powered, NULL, NULL);

	if (!strcmp(interface, "org.bluez.Adapter1") )
		if (update_adapter_properties(msg, name) < 0 )
			return;
}

bool client_init() {

	if (p_dbus)
		client = l_dbus_client_new(p_dbus, "org.bluez", "/org/bluez");
	else
		return false;

        l_dbus_client_set_connect_handler(client, bluez_client_connected, NULL,
                                                                        NULL);
        l_dbus_client_set_disconnect_handler(client, bluez_client_disconnected,
                                                                NULL, NULL);

        l_dbus_client_set_proxy_handlers(client, proxy_added, proxy_removed,
                                                        property_changed, NULL, NULL);

        l_dbus_client_set_ready_handler(client, bluez_client_ready, NULL, NULL);

	return true;
}

bool client_exit() {
	change_name(adapter.name);
        l_dbus_client_destroy(client);
	l_timeout_remove(timeout_powered);
	return true;
}
