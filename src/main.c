#include <config.h>
#include <ell/ell.h>

int main (int argc, char* argv[])
{
	if (!l_main_init())
		return EXIT_FAILURE;
	l_log_set_stderr();
	l_debug_enable("*");
	l_info("Gate controller daemon version %s", VERSION);

	l_main_run();

	l_main_exit();
	return 0;
}
