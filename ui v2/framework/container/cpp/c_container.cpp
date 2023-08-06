#include "../c_container.hpp"
#include "../../gui/c_gui.hpp"

void save_window( ) {

}

sdk::c_function gui::containter::impl::draw_menu( ) {
	static bool salut = false;
	static bool salut2 = false;
	static int sl = 0;
	/* you can use the element warper too if you want 
		this->begin_window ... etc
	*/

	if ( gui::window::begin_window( "ui" ) || gui::ctx->setup ) {
		if ( gui::window::begin_tab( "i" ) || gui::ctx->setup ) {
			gui::child::begin_child( "Accuracy", sdk::math::vec2_t( 50, 60 ) );  {
				gui::controls::checkbox( "Enable", &salut, "This enables aimbot" );
				gui::controls::checkbox( "Auto fire", &salut2 );
				gui::controls::slider( "Hitchance", &sl, 0, 100 );
				gui::controls::button( "Save", [ & ]( ) {
					save_window( );
				} );
			}
			gui::child::end_child( );
			gui::child::begin_child( "Hitbox", sdk::math::vec2_t( 50, 20 ) );
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