#include "../c_container.hpp"
#include "../../gui/c_gui.hpp"

sdk::c_function gui::containter::impl::draw_menu( ) {
	if ( gui::window::begin_window( "ui" ) || gui::ctx->setup ) {

		/* end window */
		gui::window::end_window( );
	}
}