#include "../c_button.hpp"
#include "../../../gui/c_gui.hpp"
#include "../../../../sdk/notify/c_notify.hpp"
#include "../../../../sdk/easing/c_easing.hpp"

std::map<size_t, std::vector<std::pair<sdk::math::vec2_t, float>>> ripple;

void gui::controls::button( const sdk::c_str& name, std::function< sdk::c_function( ) > callback, bool use_unique_id ) {
	sdk::math::vec2_t cursor_pos = gui::helpers::pop_cursor_pos( );
	sdk::math::vec2_t draw_pos = ctx->pos + cursor_pos;
	sdk::math::vec2_t draw_size( std::fmin( ( int )ctx->parent_size.x - 90, 270 ), 35 );

	std::string split_str = gui::helpers::split_str( name, '#' )[ 0 ].data( );
	std::string unique_id = use_unique_id ? std::string( name ).erase( 0, split_str.length( ) ) : name;

	/* spacing */
	draw_pos.y += 5;

	bool hovered = sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size ) &&
		sdk::input::input_sys::get( )->is_in_box( ctx->parent_pos, ctx->parent_size );

	if ( ripple.find( gui::helpers::hash( unique_id ) ) == ripple.end( ) ) {
		ripple.insert( { gui::helpers::hash( unique_id ), { } } );
	}

	static sdk::color::col_t menu_colors[ 3 ] = {
		sdk::color::col_t( 0, 0, 0 ), // outline 1
		sdk::color::col_t( 49, 49, 49 ), // outline 2
		sdk::color::col_t( 37, 37, 37 ) // backround
	};

	sdk::drawing::rect( draw_pos.x - 1, draw_pos.y - 1, draw_size.x + 2, draw_size.y + 2, menu_colors[ 0 ].modify_alpha( 255 * ctx->animation ), 2 );
	sdk::drawing::rect( draw_pos.x, draw_pos.y, draw_size.x, draw_size.y, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 2 );

	sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, draw_size.x - 2, draw_size.y - 2, menu_colors[ 2 ].modify_alpha( 255 * ctx->animation ), 2 );


	draw_list->PushClipRect( { draw_pos.x, draw_pos.y }, { draw_pos.x + draw_size.x, draw_pos.y + draw_size.y }, true );
	for ( int r = 0; r < ripple.at( gui::helpers::hash( unique_id ) ).size( ); ++r ) {
		if ( ripple.at( gui::helpers::hash( unique_id ) ).empty( ) ) {
			continue;
		}
		ripple.at( gui::helpers::hash( unique_id ) ).at( r ).second -= ( 1.0f / 0.6f ) * 0.002;

		if ( ripple.at( gui::helpers::hash( unique_id ) ).at( r ).second <= 0.f ) {
			ripple.at( gui::helpers::hash( unique_id ) ).at( r ).second = 0.f;
			continue;
		}

		if ( ripple.at( gui::helpers::hash( unique_id ) ).at( r ).first > draw_pos &&
			 ripple.at( gui::helpers::hash( unique_id ) ).at( r ).first < draw_pos + draw_size ) {
			if ( ripple.at( gui::helpers::hash( unique_id ) ).at( r ).second != 0.f ) {
				sdk::drawing::filled_circle(
					ripple.at( gui::helpers::hash( unique_id ) ).at( r ).first, 120 * ( 1.0f - ripple.at( gui::helpers::hash( unique_id ) ).at( r ).second ),
					sdk::color::col_t( 66, 66, 66, 50 ).modify_alpha( int( 80 * ( ( ripple.at( gui::helpers::hash( unique_id ) ).at( r ).second ) * ctx->animation ) ) )
				);
			}
		}
	}

	/* flashlight stuff, seen that */
	sdk::math::vec2_t mouse_pos = sdk::input::input_sys::get( )->get_mouse_position( );
	sdk::drawing::gradient_circle_filled(
		mouse_pos, 60, sdk::color::col_t( 66, 66, 66, 25 ), sdk::color::col_t( 66, 66, 66, 0 )
	);

	draw_list->PopClipRect( );

	if ( ctx->focused_id == 0 ) {
		if ( hovered && sdk::input::input_sys::get( )->was_key_pressed( VK_LBUTTON ) ) {
			ctx->focused_id = gui::helpers::hash( unique_id );
		}
	} else if ( ctx->focused_id == gui::helpers::hash( unique_id ) ) {
		if ( !sdk::input::input_sys::get( )->is_key_down( VK_LBUTTON ) ) {
			ctx->focused_id = 0;

			if ( hovered ) {
				callback( );

				ripple.at( gui::helpers::hash( unique_id ) ).push_back( { sdk::input::input_sys::get( )->get_mouse_position( ), 1.0f } );
			}
		}
	}


	gui::helpers::push_cusor_pos( cursor_pos + sdk::math::vec2_t( 0, draw_size.y + gui::helpers::object_padding( ) ) );
}