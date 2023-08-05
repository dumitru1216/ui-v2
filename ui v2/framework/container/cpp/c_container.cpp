#include "../c_container.hpp"
#include "../../gui/c_gui.hpp"

sdk::c_function gui::containter::impl::draw_menu( ) {
	if ( gui::window::begin_window( "ui" ) || gui::ctx->setup ) {
		if ( gui::window::begin_tab( "i" ) || gui::ctx->setup ) {

		}
		if ( gui::window::begin_tab( "k" ) || gui::ctx->setup ) {

		}
		if ( gui::window::begin_tab( "3" ) || gui::ctx->setup ) {

		}
		if ( gui::window::begin_tab( "5" ) || gui::ctx->setup ) {

		}
		if ( gui::window::begin_tab( "6" ) || gui::ctx->setup ) {

		}

		/* end window */
		gui::window::end_window( );
	}
}