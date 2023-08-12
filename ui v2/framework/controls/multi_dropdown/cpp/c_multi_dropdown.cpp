#include "../c_multi_dropdown.hpp"
#include "../../../gui/c_gui.hpp"
#include "../../../../sdk/notify/c_notify.hpp"
#include "../../../../sdk/easing/c_easing.hpp"

std::map<int, float> mdh_a;
std::map<int, float> mdh_t;
std::map<int, float> mdo_a;
std::map<int, float> mdo_t;
std::map<int, float> mdt_a;
std::map<int, float> mdt_t;

bool gui::controls::multi_dropdown( const sdk::c_str& name, std::vector< c_multi_item_t > values, int max_items ) {
	sdk::math::vec2_t cursor_pos = gui::helpers::pop_cursor_pos( );
	sdk::math::vec2_t draw_pos = ctx->pos + cursor_pos;
	sdk::math::vec2_t draw_size( std::fmin( ( int )ctx->parent_size.x - 90, 270 ), 28 );

	if ( values.empty( ) )
		return false;

	draw_pos.x += 20;
	draw_pos.y += 35;
	sdk::drawing::text(
		draw_pos.x, draw_pos.y - 17, sdk::color::col_t( ).modify_alpha( 80 ), sdk::drawing::c_fonts::verdana, name.c_str( )
	);
	draw_pos.y += 2;

	bool hovered = sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size ) && sdk::input::input_sys::get( )->is_in_box( ctx->parent_pos, ctx->parent_size );
	if ( ctx->focused_id == 0 ) {
		if ( hovered && sdk::input::input_sys::get( )->was_key_pressed( VK_LBUTTON ) )
			ctx->focused_id = gui::helpers::hash( name );
	} else if ( ctx->focused_id == gui::helpers::hash( name ) ) {
		if ( hovered ) {
	

			ctx->multi_dropdown_info.elements = values;
			ctx->multi_dropdown_info.size = draw_size.x;
			ctx->multi_dropdown_info.pos = draw_pos + sdk::math::vec2_t( 0, draw_size.y + 3 );
			ctx->multi_dropdown_info.hashed_id = gui::helpers::hash( name );
		}
	}

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

	auto i = gui::helpers::hash( name );
	if ( mdh_a.find( i ) == mdh_a.end( ) ) {
		mdh_a.insert( { i, 0.f } );
		mdh_t.insert( { i, 0.f } );

		mdo_a.insert( { i, 0.f } );
		mdo_t.insert( { i, 0.f } );

		mdt_a.insert( { i, 0.f } );
		mdt_t.insert( { i, 0.f } );
	}

	if ( sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size ) ) {
		mdh_t.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
		mdh_t.at( i ) = std::clamp<float>( mdh_t.at( i ), 0.f, 1.f );

		mdh_a.at( i ) = sdk::easing::out_circ( mdh_t.at( i ) );

		if ( mdh_a.at( i ) >= 0.999998 ) {
			mdh_a.at( i ) = 1.f;
		}

		if ( mdh_a.at( i ) == 1.f ) {
			mdt_t.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
			mdt_t.at( i ) = std::clamp<float>( mdt_t.at( i ), 0.f, 1.f );

			mdt_a.at( i ) = sdk::easing::out_circ( mdt_t.at( i ) );

			if ( mdt_a.at( i ) >= 0.999998 ) {
				mdt_a.at( i ) = 1.f;
			}
		} else {
			mdt_t.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
			mdt_t.at( i ) = std::clamp<float>( mdt_t.at( i ), 0.f, 1.f );

			mdt_a.at( i ) = sdk::easing::out_circ( mdt_t.at( i ) );

			if ( mdt_a.at( i ) <= 0.05f ) {
				mdt_a.at( i ) = 0.f;
			}
		}
	} else {
		mdh_t.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
		mdh_t.at( i ) = std::clamp<float>( mdh_t.at( i ), 0.f, 1.f );

		mdh_a.at( i ) = sdk::easing::out_circ( mdh_t.at( i ) );

		if ( mdh_a.at( i ) <= 0.05f ) {
			mdh_a.at( i ) = 0.f;
		}
	}

	sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, draw_size.x - 2, draw_size.y - 2, menu_colors[ 2 ].modify_alpha( 255 * ctx->animation ), 2 );
	sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, draw_size.x - 2, draw_size.y - 2, menu_colors[ 3 ].modify_alpha( 255 * mdh_a.at( i ) ), 2 );

	/* flashlight */
	draw_list->PushClipRect( { draw_pos.x, draw_pos.y }, { draw_pos.x + draw_size.x, draw_pos.y + draw_size.y }, true );

	sdk::math::vec2_t mouse_pos = sdk::input::input_sys::get( )->get_mouse_position( );
	sdk::drawing::gradient_circle_filled(
		mouse_pos, 60, sdk::color::col_t( 66, 66, 66, 25 ), sdk::color::col_t( 66, 66, 66, 0 )
	);

	draw_list->PopClipRect( );

	sdk::drawing::rect( draw_pos.x - 1, draw_pos.y - 1, draw_size.x + 2, draw_size.y + 2, menu_colors[ 0 ].modify_alpha( 255 * ctx->animation ), 2 );
	sdk::drawing::rect( draw_pos.x, draw_pos.y, draw_size.x, draw_size.y, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 2 );

	bool too_long = false;
	char buf[ 128 ] = { 0 };
	for ( int i = 0; i < values.size( ); i++ ) {
		auto& it = values.at( i ).value;
		if ( !*it )
			continue;

		strcat( buf, values.at( i ).name.data( ) );

		if ( i != values.size( ) - 1 )
			strcat( buf, ( ", " ) );

		auto max_text_width = draw_size.x - ( 25 + sdk::drawing::get_text_size( "...", sdk::drawing::c_fonts::verdana ).x );
		if ( sdk::drawing::get_text_size( buf, sdk::drawing::c_fonts::verdana ).x > max_text_width ) {
			if ( max_text_width >= 1 ) {
				while ( sdk::drawing::get_text_size( buf, sdk::drawing::c_fonts::verdana ).x > max_text_width 
						|| buf[ strlen( buf ) - 1 ] == ',' || buf[ strlen( buf ) - 1 ] == ' ' ) {
					buf[ strlen( buf ) - 1 ] = '\0';
				}
			}

			too_long = true;
			break;
		}
	}

	if ( too_long )
		strcat( buf, "..." );

	if ( strlen( buf ) == 0 )
		strcat( buf, "-" );

	if ( buf[ strlen( buf ) - 1 ] == ' ' )
		buf[ strlen( buf ) - 2 ] = '\0';

	if ( buf[ strlen( buf ) - 1 ] == ' ' )
		buf[ strlen( buf ) - 2 ] = '\0';

	sdk::drawing::text(
		draw_pos.x + 10, draw_pos.y + 6, sdk::color::col_t( ).modify_alpha( 80 ), sdk::drawing::c_fonts::verdana, buf
	);
	
	sdk::drawing::text(
		draw_pos.x + 10, draw_pos.y + 6, sdk::color::col_t( ).modify_alpha( 100 * mdh_a.at( i ) ), sdk::drawing::c_fonts::verdana, buf
	);

	sdk::drawing::text(
		draw_pos.x, draw_pos.y - 19, sdk::color::col_t( ).modify_alpha( 100 * mdh_a.at( i ) ), sdk::drawing::c_fonts::verdana, name.c_str( )
	);

	bool focused = ctx->focused_id == gui::helpers::hash( name );
	sdk::drawing::text(
		draw_pos.x + draw_size.x - 20, draw_pos.y + 6, sdk::color::col_t( ).modify_alpha( 80 ), sdk::drawing::c_fonts::arrows, focused ? "a" : "d", true
	);

	sdk::drawing::text(
		draw_pos.x + draw_size.x - 20, draw_pos.y + 6, sdk::color::col_t( ).modify_alpha( 100 * mdh_a.at( i ) ), sdk::drawing::c_fonts::arrows, focused ? "a" : "d", true
	);

	int add = ( !gui::helpers::split_str( name, '#' )[ 0 ].empty( ) ) ? 12 : -3;
	gui::helpers::push_cusor_pos( cursor_pos + sdk::math::vec2_t( 0, draw_size.y + gui::helpers::object_padding( ) ) );
	return ctx->focused_id == gui::helpers::hash( name ); //true; // ??
}