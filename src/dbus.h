#ifndef __DBUS_H
#define __DBUS_H
#include <stdbool.h>
struct l_dbus* p_dbus;
bool dbus_init();
bool dbus_exit();
#endif
