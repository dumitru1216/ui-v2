#include "../c_checkbox.hpp"
#include "../../../gui/c_gui.hpp"
#include "../../../../sdk/notify/c_notify.hpp"
#include "../../../../sdk/easing/c_easing.hpp"

std::map<int, float> select_animation_check;
std::map<int, float> select_timer_check;

std::map<int, float> hover_animation_check;
std::map<int, float> hover_timer_check;

std::map<int, float> tool_tip_animation_check;
std::map<int, float> tool_tip_timer_check;

bool gui::controls::checkbox( const sdk::c_str& name, bool* value, const sdk::c_str& tool_tip )
{
	sdk::math::vec2_t cursor_pos = gui::helpers::pop_cursor_pos( );
	sdk::math::vec2_t draw_pos = ctx->pos + cursor_pos;
	sdk::math::vec2_t draw_size = sdk::math::vec2_t( 11, 11 );

	/* i */
	auto i = gui::helpers::hash( name );
	if ( hover_animation_check.find( i ) == hover_animation_check.end( ) ) {
		hover_animation_check.insert( { i, 0.f } );
		hover_timer_check.insert( { i, 0.f } );

		select_animation_check.insert( { i, 0.f } );
		select_timer_check.insert( { i, 0.f } );

		tool_tip_animation_check.insert( { i, 0.f } );
		tool_tip_timer_check.insert( { i, 0.f } );
	}

	if ( sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size + sdk::math::vec2_t( 50, 0 ) ) ) {
		hover_timer_check.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
		hover_timer_check.at( i ) = std::clamp<float>( hover_timer_check.at( i ), 0.f, 1.f );

		hover_animation_check.at( i ) = sdk::easing::out_circ( hover_timer_check.at( i ) );

		if ( hover_animation_check.at( i ) >= 0.999998 ) {
			hover_animation_check.at( i ) = 1.f;
		}

		if ( hover_animation_check.at( i ) == 1.f ) {
			tool_tip_timer_check.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
			tool_tip_timer_check.at( i ) = std::clamp<float>( tool_tip_timer_check.at( i ), 0.f, 1.f );

			tool_tip_animation_check.at( i ) = sdk::easing::out_circ( tool_tip_timer_check.at( i ) );

			if ( tool_tip_animation_check.at( i ) >= 0.999998 ) {
				tool_tip_animation_check.at( i ) = 1.f;
			}
		} else {
			tool_tip_timer_check.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
			tool_tip_timer_check.at( i ) = std::clamp<float>( tool_tip_timer_check.at( i ), 0.f, 1.f );

			tool_tip_animation_check.at( i ) = sdk::easing::out_circ( tool_tip_timer_check.at( i ) );

			if ( tool_tip_animation_check.at( i ) <= 0.05f ) {
				tool_tip_animation_check.at( i ) = 0.f;
			}
		}
	} else {
		hover_timer_check.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
		hover_timer_check.at( i ) = std::clamp<float>( hover_timer_check.at( i ), 0.f, 1.f );

		hover_animation_check.at( i ) = sdk::easing::out_circ( hover_timer_check.at( i ) );

		if ( hover_animation_check.at( i ) <= 0.05f ) {
			hover_animation_check.at( i ) = 0.f;
		}
	}

	if ( *value ) {
		select_timer_check.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
		select_timer_check.at( i ) = std::clamp<float>( select_timer_check.at( i ), 0.f, 1.f );

		select_animation_check.at( i ) = sdk::easing::out_circ( select_timer_check.at( i ) );

		if ( select_animation_check.at( i ) >= 0.999998 ) {
			select_animation_check.at( i ) = 1.f;
		}
	} else {
		select_timer_check.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
		select_timer_check.at( i ) = std::clamp<float>( select_timer_check.at( i ), 0.f, 1.f );

		select_animation_check.at( i ) = sdk::easing::out_circ( select_timer_check.at( i ) );

		if ( select_animation_check.at( i ) <= 0.05f ) {
			select_animation_check.at( i ) = 0.f;
		}
	}

	static sdk::color::col_t menu_colors[ 3 ] = {
		sdk::color::col_t( 0, 0, 0 ), // outline 1
		sdk::color::col_t( 49, 49, 49 ), // outline 2
		sdk::color::col_t( 37, 37, 37 ) // backround
	};

	bool hovered = sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size + sdk::math::vec2_t( 50, 0 ) - sdk::math::vec2_t( 1, 1 ) ) &&
		sdk::input::input_sys::get( )->is_in_box( ctx->parent_pos, ctx->parent_size );

	bool hovered_text = sdk::input::input_sys::get( )->is_in_box( draw_pos + sdk::math::vec2_t( 0, -3 ), sdk::math::vec2_t( 120, 0 ) ) &&
		sdk::input::input_sys::get( )->is_in_box( ctx->parent_pos, ctx->parent_size );

	if ( ctx->focused_id == 0 ) {
		if ( ( hovered || hovered_text ) && sdk::input::input_sys::get( )->was_key_pressed( VK_LBUTTON ) ) {
			ctx->focused_id = i;
		}
	} else if ( ctx->focused_id == i ) {
		if ( !sdk::input::input_sys::get( )->is_key_down( VK_LBUTTON ) ) {
			ctx->focused_id = 0;

			if ( ( hovered || hovered_text ) )
				*value ^= 1;
		}
	}

	sdk::drawing::rect_filled(
		draw_pos.x, draw_pos.y, draw_size.x, draw_size.y, menu_colors[ 2 ], 3
	);

	/* flashlight */
	draw_list->PushClipRect( { draw_pos.x, draw_pos.y }, { draw_pos.x + draw_size.x, draw_pos.y + draw_size.y }, true );

	sdk::math::vec2_t mouse_pos = sdk::input::input_sys::get( )->get_mouse_position( );
	sdk::drawing::gradient_circle_filled(
		mouse_pos, 60, sdk::color::col_t( 66, 66, 66, 25 ), sdk::color::col_t( 66, 66, 66, 0 )
	);

	draw_list->PopClipRect( );

	sdk::drawing::rect(
		draw_pos.x - 1, draw_pos.y - 1, draw_size.x + 2, draw_size.y + 2, menu_colors[ 0 ], 2
	);

	sdk::drawing::rect_filled(
		draw_pos.x + 1, draw_pos.y + 1, draw_size.x - 2, draw_size.y - 2, ctx->accent.modify_alpha( 255 * select_animation_check.at( i ) ), 2
	);

	sdk::drawing::rect(
		draw_pos.x, draw_pos.y, draw_size.x, draw_size.y, menu_colors[ 1 ], 2
	);

	sdk::drawing::text(
		draw_pos.x + 20, draw_pos.y - 2, sdk::color::col_t( ).modify_alpha( 100 ), sdk::drawing::c_fonts::verdana, name.c_str( )
	);

	if ( !*value ) {
		sdk::drawing::text(
			draw_pos.x + 20, draw_pos.y - 2, sdk::color::col_t( ).modify_alpha( 80 * hover_animation_check.at( i ) ), sdk::drawing::c_fonts::verdana, name.c_str( )
		);
	}


	sdk::drawing::text(
		draw_pos.x + 20, draw_pos.y - 2, sdk::color::col_t( ).modify_alpha( ( *value ? 225 : 100 ) * select_animation_check.at( i ) ), sdk::drawing::c_fonts::verdana, name.c_str( )
	);

	if ( sdk::input::input_sys::get( )->is_in_box( draw_pos, draw_size + sdk::math::vec2_t( 50, 0 ) ) && !tool_tip.empty( ) ) {
		if ( hover_animation_check.at( i ) == 1.f ) {
			auto tooltip_size = sdk::drawing::get_text_size( tool_tip.c_str( ), sdk::drawing::c_fonts::verdana );
			auto integr_size{ tooltip_size.x + 10 };

			auto tab_pos = draw_pos;

			draw_list->PopClipRect( ); /* we want to see our tooltip*/

			sdk::drawing::text(
				ctx->pos.x + 10, ctx->pos.y + ctx->size.y - 17, sdk::color::col_t( 150, 150, 150 ).modify_alpha( 255 * tool_tip_timer_check.at( i ) ), sdk::drawing::c_fonts::verdana, tool_tip.c_str( )
			);
		}
	}

	gui::helpers::push_cusor_pos( cursor_pos + sdk::math::vec2_t( 0, draw_size.y + gui::helpers::object_padding( ) ) );
	return *value;
}