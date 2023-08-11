#include "../c_dropdown.hpp"
#include "../../../gui/c_gui.hpp"
#include "../../../../sdk/notify/c_notify.hpp"
#include "../../../../sdk/easing/c_easing.hpp"

/* _a -> animation 
   _t -> timer
*/

std::map<int, float> dh_a; 
std::map<int, float> dh_t;
std::map<int, float> do_a;
std::map<int, float> do_t;
std::map<int, float> dt_a;
std::map<int, float> dt_t;

bool gui::controls::dropdown( const sdk::c_str& name, sdk::c_str_vec values, int* var_name, int max_items ) {
	sdk::math::vec2_t cursor_pos = gui::helpers::pop_cursor_pos( );
	sdk::math::vec2_t draw_pos = ctx->pos + cursor_pos;
	sdk::math::vec2_t draw_size( std::fmin( ( int )ctx->parent_size.x - 90, 270 ), 28 );

	draw_pos.x += 20;
	draw_pos.y += 20;
	sdk::drawing::text(
		draw_pos.x, draw_pos.y - 17, sdk::color::col_t( ).modify_alpha( 80 ), sdk::drawing::c_fonts::verdana, name.c_str( )
	);
	draw_pos.y += 2;

	if ( *var_name < 0 )
		*var_name = 0;
	else if ( *var_name >= values.size( ) )
		*var_name = values.size( ) - 1;

	sdk::c_str name_to_show;
	if ( !gui::helpers::split_str( name, "#" )[ 0 ].empty( ) ) {
		name_to_show = name;
	} else {
		name_to_show = "Error while grabbing!";
	}

	static sdk::color::col_t menu_colors[ 4 ] = {
		sdk::color::col_t( 0, 0, 0 ), // outline 1
		sdk::color::col_t( 49, 49, 49 ), // outline 2
		sdk::color::col_t( 37, 37, 37 ), // backround
		sdk::color::col_t( 42, 42, 42 ) // backround
	};

	bool hovered = sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size ) && sdk::input::input_sys::get( )->is_in_box( ctx->parent_pos, ctx->parent_size );

	auto i = gui::helpers::hash( name );
	if ( dh_a.find( i ) == dh_a.end( ) ) {
		dh_a.insert( { i, 0.f } );
		dh_t.insert( { i, 0.f } );

		do_a.insert( { i, 0.f } );
		do_t.insert( { i, 0.f } );	

		dt_a.insert( { i, 0.f } );
		dt_t.insert( { i, 0.f } );
	}
	
	if ( sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size ) ) {
		dh_t.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
		dh_t.at( i ) = std::clamp<float>( dh_t.at( i ), 0.f, 1.f );

		dh_a.at( i ) = sdk::easing::out_circ( dh_t.at( i ) );

		if ( dh_a.at( i ) >= 0.999998 ) {
			dh_a.at( i ) = 1.f;
		}

		if ( dh_a.at( i ) == 1.f ) {
			dt_t.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
			dt_t.at( i ) = std::clamp<float>( dt_t.at( i ), 0.f, 1.f );

			dt_a.at( i ) = sdk::easing::out_circ( dt_t.at( i ) );

			if ( dt_a.at( i ) >= 0.999998 ) {
				dt_a.at( i ) = 1.f;
			}
		} else {
			dt_t.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
			dt_t.at( i ) = std::clamp<float>( dt_t.at( i ), 0.f, 1.f );

			dt_a.at( i ) = sdk::easing::out_circ( dt_t.at( i ) );

			if ( dt_a.at( i ) <= 0.05f ) {
				dt_a.at( i ) = 0.f;
			}
		}
	} else {
		dh_t.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
		dh_t.at( i ) = std::clamp<float>( dh_t.at( i ), 0.f, 1.f );

		dh_a.at( i ) = sdk::easing::out_circ( dh_t.at( i ) );

		if ( dh_a.at( i ) <= 0.05f ) {
			dh_a.at( i ) = 0.f;
		}
	}
	
	sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, draw_size.x - 2, draw_size.y - 2, menu_colors[ 2 ].modify_alpha( 255 * ctx->animation ), 2 );
	sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, draw_size.x - 2, draw_size.y - 2, menu_colors[ 3 ].modify_alpha( 255 * dh_a.at( i ) ), 2 );

	/* flashlight */
	draw_list->PushClipRect( { draw_pos.x, draw_pos.y }, { draw_pos.x + draw_size.x, draw_pos.y + draw_size.y }, true );

	sdk::math::vec2_t mouse_pos = sdk::input::input_sys::get( )->get_mouse_position( );
	sdk::drawing::gradient_circle_filled(
		mouse_pos, 60, sdk::color::col_t( 66, 66, 66, 25 ), sdk::color::col_t( 66, 66, 66, 0 )
	);

	draw_list->PopClipRect( );

	sdk::drawing::rect( draw_pos.x - 1, draw_pos.y - 1, draw_size.x + 2, draw_size.y + 2, menu_colors[ 0 ].modify_alpha( 255 * ctx->animation ), 2 );
	sdk::drawing::rect( draw_pos.x, draw_pos.y, draw_size.x, draw_size.y, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 2 );

	sdk::drawing::text(
		draw_pos.x + 10, draw_pos.y + 6, sdk::color::col_t( ).modify_alpha( 80 ), sdk::drawing::c_fonts::verdana, values[ *var_name ].c_str( )
	);

	sdk::drawing::text(
		draw_pos.x + 10, draw_pos.y + 6, sdk::color::col_t( ).modify_alpha( 100 * dh_a.at( i ) ), sdk::drawing::c_fonts::verdana, values[ *var_name ].c_str( )
	);

	sdk::drawing::text(
		draw_pos.x, draw_pos.y - 19, sdk::color::col_t( ).modify_alpha( 100 * dh_a.at( i ) ), sdk::drawing::c_fonts::verdana, name.c_str( )
	);

	bool focused = ctx->focused_id == gui::helpers::hash( name );
	sdk::drawing::text(
		draw_pos.x + draw_size.x - 20, draw_pos.y + 6, sdk::color::col_t( ).modify_alpha( 80 ), sdk::drawing::c_fonts::arrows, focused ? "a" : "d", true
	);

	sdk::drawing::text(
		draw_pos.x + draw_size.x - 20, draw_pos.y + 6, sdk::color::col_t( ).modify_alpha( 100 * dh_a.at( i ) ), sdk::drawing::c_fonts::arrows, focused ? "a" : "d", true
	);

	if ( ctx->focused_id == 0 ) {
		if ( hovered && sdk::input::input_sys::get( )->was_key_pressed( VK_LBUTTON ) )
			ctx->focused_id = gui::helpers::hash( name );
	} else if ( ctx->focused_id == gui::helpers::hash( name ) ) {
		if ( hovered ) {
			ctx->dropdown_info.elements = values;
			ctx->dropdown_info.size = draw_size.x;
			ctx->dropdown_info.option = var_name;
			ctx->dropdown_info.pos = draw_pos + sdk::math::vec2_t( 0, draw_size.y + 3 );
			ctx->dropdown_info.hashed_id = gui::helpers::hash( name );
		}
	}

	gui::helpers::push_cusor_pos( cursor_pos + sdk::math::vec2_t( 0, draw_size.y + gui::helpers::object_padding( ) ) );
	return true; // ??
}