#include "../../c_includes.hpp"
#include "../../sdk/notify/c_notify.hpp"

int install( sdk::c_instance instance, sdk::c_instance prev_instance,
		  LPWSTR cmd_line, int cmd_show ) {
	sdk::c_warper->alloc_console( );

	/* name */
	sdk::c_lstr lpz_class{ "ui-v2" };
	sdk::c_lstr name{ "ui-v2" };

	if ( !i::c_hooks->register_window( instance, lpz_class ) ||
		 !i::c_hooks->initialize_window( instance, lpz_class, name ) ) {

		/* stop executing */
		return 0;
	}

	if ( !i::c_hooks->hook_create_device( i::window ) ) {
		i::c_hooks->clean_device( );

		/* unregister class */
		sdk::c_warper->unregister_class( lpz_class, instance );

		/* stop it */
		return 0;
	}

	/* window updating */
	sdk::c_warper->show_window( i::window, SW_SHOWDEFAULT );
	sdk::c_warper->update_window( i::window );

	/* warp imgui */
	i::c_hooks->init_imgui( i::window, i::device );
}

/* main function */
int main( sdk::c_instance instance, sdk::c_instance prev_instance,
		  LPWSTR cmd_line, int cmd_show ) {
	install( instance, prev_instance, cmd_line, cmd_show );
}