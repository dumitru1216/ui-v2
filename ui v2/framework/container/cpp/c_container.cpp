#include "../c_container.hpp"
#include "../../gui/c_gui.hpp"

sdk::c_function gui::containter::impl::draw_menu( ) {
	if ( gui::window::begin_window( "ui" ) || gui::ctx->setup ) {
		if ( gui::window::begin_tab( "i" ) || gui::ctx->setup ) {
			gui::child::begin_child( "aimbot", sdk::math::vec2_t( 50, 60 ) );  {

			}
			gui::child::end_child( );
			gui::child::begin_child( "aimbot2", sdk::math::vec2_t( 50, 20 ) );
			{

			}
			gui::child::end_child( );

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