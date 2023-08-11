#include "../c_slider.hpp"
#include "../../../gui/c_gui.hpp"
#include "../../../../sdk/notify/c_notify.hpp"
#include "../../../../sdk/easing/c_easing.hpp"

std::map<int, float> select_animation_s;
std::map<int, float> select_timer_s;

std::map<int, float> hover_animation_s;
std::map<int, float> hover_timer_s;

std::map<int, float> tool_tip_animation_s;
std::map<int, float> tool_tip_timer_s;

bool gui::controls::slider( const sdk::c_str& name, int* var_name, int min, int max,
			 const sdk::c_str& display, const sdk::c_str& tool_tip ) {
	sdk::math::vec2_t cursor_pos = gui::helpers::pop_cursor_pos( );
	sdk::math::vec2_t draw_pos = ctx->pos + cursor_pos;
	sdk::math::vec2_t draw_size( std::fmin( ( int )ctx->parent_size.x - 90, 270 ), 6 );

	static sdk::color::col_t menu_colors[ 3 ] = {
		sdk::color::col_t( 0, 0, 0 ), // outline 1
		sdk::color::col_t( 49, 49, 49 ), // outline 2
		sdk::color::col_t( 37, 37, 37 ) // backround
	};

	if ( !gui::helpers::split_str( name, '#' )[ 0 ].empty( ) ) {
		draw_pos.y += 11;
	}

	draw_pos.y += 4;
	sdk::drawing::text(
		draw_pos.x, draw_pos.y - 17, sdk::color::col_t( ).modify_alpha( 100 ), sdk::drawing::c_fonts::verdana, name.c_str( )
	);
	draw_pos.y += 2;

	bool hovered = sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size + sdk::math::vec2_t( 0, 1 ) ) &&
		sdk::input::input_sys::get( )->is_in_box( ctx->parent_pos, ctx->parent_size );

	if ( ctx->slider_info.prev_amount.find( gui::helpers::hash( name ) ) == ctx->slider_info.prev_amount.end( ) ) {
		ctx->slider_info.prev_amount.insert( { gui::helpers::hash( name ), 0.f } );
	}

	if ( ctx->slider_info.last_change_time.find( gui::helpers::hash( name ) ) == ctx->slider_info.last_change_time.end( ) ) {
		ctx->slider_info.last_change_time.insert( { gui::helpers::hash( name ), 0.f } );
	}

	if ( ctx->slider_info.should_change_value.find( gui::helpers::hash( name ) ) == ctx->slider_info.should_change_value.end( ) ) {
		ctx->slider_info.should_change_value.insert( { gui::helpers::hash( name ), { 0.f, false } } );
	}

	if ( ctx->focused_id == 0 ) { /* gheto */
		if ( hovered && sdk::input::input_sys::get( )->was_key_pressed( VK_LBUTTON ) ) {
			ctx->focused_id = gui::helpers::hash( name );
			sdk::c_notify->log( "[S] 1" );
		}

		if ( hovered && sdk::input::input_sys::get( )->is_key_down( VK_LBUTTON ) ) {
			int offset = std::floor<int>( std::clamp<int>( sdk::math::vec2_t(
				sdk::input::input_sys::get( )->get_mouse_position( ) - draw_pos ).x, 0, draw_size.x ) );

			/* thats what map_number does: (offset - 0) / ( draw_size.x - 0 ) * ( max - min ) + min */
			float target_value = gui::helpers::map_number( offset, 0, draw_size.x, min, max );
			*var_name = lerp<int>( *var_name, target_value, 0.2f );


		} else {
			ctx->focused_id = 0;
		}

		// Declare clicked as a static variable outside the hovered block
		static bool clicked = false;
		static bool res1 = false, res2 = false;

		if ( hovered ) {
			// left boss
			bool left_key = GetAsyncKeyState( VK_LEFT ) & 0x8000;
			bool right_key = GetAsyncKeyState( VK_RIGHT ) & 0x8000;

			if ( sdk::input::input_sys::get( )->is_key_down( VK_RIGHT ) ) {
				res2 = false;

				if ( !clicked ) {
					*var_name += 1;
					clicked = true;
				}

				// ghetto clamping
				if ( *var_name < min )
					*var_name = min;

				if ( *var_name > max )
					*var_name = max;
			} else {
				if ( !res2 ) {
					clicked = false;
					res2 = true;
				}
			}

			if ( sdk::input::input_sys::get( )->is_key_down( VK_LEFT ) ) {
				res1 = false;

				if ( !clicked ) {
					*var_name -= 0.1;
					clicked = true;
				}

				// ghetto clamping
				if ( *var_name < min )
					*var_name = min;

				if ( *var_name > max )
					*var_name = max;
			} 
			else {
				if ( !res1 ) {
					clicked = false;
					res1 = true;
				}
			}

		}
	}

	/* animation handling */
	auto i = gui::helpers::hash( name );
	if ( hover_animation_s.find( i ) == hover_animation_s.end( ) ) {
		hover_animation_s.insert( { i, 0.f } );
		hover_timer_s.insert( { i, 0.f } );

		select_animation_s.insert( { i, 0.f } );
		select_timer_s.insert( { i, 0.f } );

		tool_tip_animation_s.insert( { i, 0.f } );
		tool_tip_timer_s.insert( { i, 0.f } );
	}

	if ( hovered && ctx->focused_id != gui::helpers::hash( name ) ) {
		hover_timer_s.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
		hover_timer_s.at( i ) = std::clamp<float>( hover_timer_s.at( i ), 0.f, 1.f );

		hover_animation_s.at( i ) = sdk::easing::out_circ( hover_timer_s.at( i ) );

		if ( hover_animation_s.at( i ) >= 0.999998 ) {
			hover_animation_s.at( i ) = 1.f;
		}

		if ( hover_animation_s.at( i ) == 1.f ) {
			tool_tip_timer_s.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
			tool_tip_timer_s.at( i ) = std::clamp<float>( tool_tip_timer_s.at( i ), 0.f, 1.f );

			tool_tip_animation_s.at( i ) = sdk::easing::out_circ( tool_tip_timer_s.at( i ) );

			if ( tool_tip_animation_s.at( i ) >= 0.999998 ) {
				tool_tip_animation_s.at( i ) = 1.f;
			}
		} else {
			tool_tip_timer_s.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
			tool_tip_timer_s.at( i ) = std::clamp<float>( tool_tip_timer_s.at( i ), 0.f, 1.f );

			tool_tip_animation_s.at( i ) = sdk::easing::out_circ( tool_tip_timer_s.at( i ) );

			if ( tool_tip_animation_s.at( i ) <= 0.05f ) {
				tool_tip_animation_s.at( i ) = 0.f;
			}
		}
	} else {
		hover_timer_s.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
		hover_timer_s.at( i ) = std::clamp<float>( hover_timer_s.at( i ), 0.f, 1.f );

		hover_animation_s.at( i ) = sdk::easing::out_circ( hover_timer_s.at( i ) );

		if ( hover_animation_s.at( i ) <= 0.05f ) {
			hover_animation_s.at( i ) = 0.f;
		}
	}
	
	sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, draw_size.x - 2, draw_size.y + 1 - 2, menu_colors[ 2 ].modify_alpha( 255 * ctx->animation ), 2 );

	/* flashlight */
	draw_list->PushClipRect( { draw_pos.x, draw_pos.y }, { draw_pos.x + draw_size.x, draw_pos.y + draw_size.y }, true );

	sdk::math::vec2_t mouse_pos = sdk::input::input_sys::get( )->get_mouse_position( );
	sdk::drawing::gradient_circle_filled(
		mouse_pos, 60, sdk::color::col_t( 66, 66, 66, 25 ), sdk::color::col_t( 66, 66, 66, 0 )
	);

	draw_list->PopClipRect( );

	sdk::drawing::rect( draw_pos.x - 1, draw_pos.y - 1, draw_size.x + 2, draw_size.y + 2 + 1, menu_colors[ 0 ].modify_alpha( 255 * ctx->animation ), 2 );
	sdk::drawing::rect( draw_pos.x, draw_pos.y, draw_size.x, draw_size.y + 1, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 2 );

	std::string complete = name + " - ";

	sdk::drawing::text(
		draw_pos.x, draw_pos.y - 19, sdk::color::col_t( ).modify_alpha( 80 * hover_animation_s.at( i ) ), sdk::drawing::c_fonts::verdana, complete.c_str( )
	);

	if ( GetAsyncKeyState( VK_DELETE ) )
		sdk::c_notify->log( std::to_string( *var_name ) );

	int width = gui::helpers::map_number ( *var_name, min, max, 0, draw_size.x );

	if ( *var_name != 0 )
		sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, width - 2, draw_size.y + 1 - 2, ctx->accent, 2 );

	static bool preview_launch = false;
	char buffer[32];
	sprintf_s( buffer, display.data( ), *var_name );

	if ( hovered && ctx->focused_id == 0 ) {
		preview_launch = true;
		float preview_value = std::clamp(
			( int )gui::helpers::map_number(
			std::clamp( ( int )sdk::math::vec2_t( sdk::input::input_sys::get( )->get_mouse_position( ) - draw_pos ).x, 0, ( int )draw_size.x ), 0, draw_size.x, min, max ), min, max
		);

		sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, gui::helpers::map_number( preview_value, min, max, 0, draw_size.x ), draw_size.y + 1 - 2, ctx->accent.modify_alpha( 100 * hover_animation_s.at( i ) ), 2 );
	}
	else {
		preview_launch = false;
	}

	if ( hovered ) {
		sdk::drawing::text(
			draw_pos.x + sdk::drawing::get_text_size(complete.c_str( ), sdk::drawing::c_fonts::verdana ).x + 2, draw_pos.y - 19, sdk::color::col_t( ).modify_alpha( 150 * hover_animation_s.at( i ) ), sdk::drawing::c_fonts::verdana, buffer
		);
	}

	int add = ( !gui::helpers::split_str( name, '#' )[ 0 ].empty( ) ) ? 12 : - 3;

	gui::helpers::push_cusor_pos( cursor_pos + sdk::math::vec2_t( 0, draw_size.y + add + gui::helpers::object_padding( ) ) );
	return ctx->focused_id == gui::helpers::hash( name );
}