#include "../c_child.hpp"
#include "../../../gui/c_gui.hpp"
#include "../../../../sdk/notify/c_notify.hpp"
#include "../../../../sdk/easing/c_easing.hpp"

sdk::c_viewport l_viewport;
sdk::math::vec2_t gp;
sdk::math::vec2_t gs;
sdk::math::vec2_t override_size = { -1, -1 };
sdk::c_str name;

std::map<int, float> hover_animation2;
std::map<int, float> hover_timer2;

int id;
std::map<int, int> scrolling;

void gui::child::begin_child( const sdk::c_str& name, const sdk::math::vec2_t& size ) {
	sdk::c_size id = gui::helpers::hash( name );
	::name = gui::helpers::split_str( name, "#" )[ 0 ].data( );
	::id = id;

	if ( scrolling.find( id ) == scrolling.end( ) ) {
		scrolling.insert( { id, 0 } );
	}

	sdk::math::vec2_t cursor_pos = gui::helpers::pop_cursor_pos( );
	sdk::math::vec2_t group_size;

	float width_percent = size.x / 100.f;
	float height_percent = size.y / 100.f;

	int width_available = ctx->size.x - 150;
	int height_available = ctx->size.y - 50;

	if ( cursor_pos.x == 50 && width_percent == 1.0f )
		width_available += 20;

	if ( cursor_pos.y == 60 && height_percent == 1.0f )
		height_available += 20;

	group_size.x = width_available * width_percent;
	group_size.y = height_available * height_percent;

	if ( override_size.x != -1 )
		group_size.x = override_size.x;
	
	if ( override_size.y != -1 )
		group_size.y = override_size.y;

	if ( ( cursor_pos.y - 20 + group_size.y ) > ctx->size.y - 37 ) {
		cursor_pos.x += group_size.x + 20;
		cursor_pos.y = 24;

		gui::helpers::push_cusor_pos( cursor_pos );
		child::begin_child( name, size );
	} else {
		sdk::math::vec2_t draw_pos = ctx->pos + cursor_pos;

		static sdk::color::col_t menu_colors[ 3 ] = {
			sdk::color::col_t( 0, 0, 0 ), // outline 1
			sdk::color::col_t( 28, 28, 28 ), // outline 2
			sdk::color::col_t( 24, 24, 24 ) // backround
		};

		sdk::drawing::rect(
			draw_pos.x - 1, draw_pos.y - 1, group_size.x + 2, group_size.y + 2, menu_colors[ 0 ].modify_alpha( 255 * ctx->animation ), 3
		);

		sdk::drawing::rect_filled(
			draw_pos.x - 1 , draw_pos.y - 1, group_size.x + 2, 6, ctx->accent.modify_alpha( 100 * ctx->animation ), 3
		);

		auto i = id;
		if ( hover_animation2.find( i ) == hover_animation2.end( ) ) {
			hover_animation2.insert( { i, 0.f } );
			hover_timer2.insert( { i, 0.f } );
		}

		if ( sdk::input::input_sys::get( )->is_in_box( draw_pos, group_size ) ) {
			hover_timer2.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
			hover_timer2.at( i ) = std::clamp<float>( hover_timer2.at( i ), 0.f, 1.f );

			hover_animation2.at( i ) = sdk::easing::out_circ( hover_timer2.at( i ) );

			if ( hover_animation2.at( i ) >= 0.999998 ) {
				hover_animation2.at( i ) = 1.f;
			}
		} else {
			hover_timer2.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
			hover_timer2.at( i ) = std::clamp<float>( hover_timer2.at( i ), 0.f, 1.f );

			hover_animation2.at( i ) = sdk::easing::out_circ( hover_timer2.at( i ) );

			if ( hover_animation2.at( i ) <= 0.05f ) {
				hover_animation2.at( i ) = 0.f;
			}
		}

		if ( sdk::input::input_sys::get( )->is_in_box( draw_pos, group_size ) ) {
			/* hover */
			{
				int middle_x = draw_pos.x + ( group_size.x / 2 );
				int half_width = ( group_size.x / 2 ) * hover_animation2.at( i );

				sdk::drawing::rect_filled(
					middle_x - half_width, draw_pos.y - 1, half_width + 2, 6,
					ctx->accent.modify_alpha( 255 * ctx->animation ), 3
				);

				sdk::drawing::rect_filled(
					middle_x, draw_pos.y - 1, half_width + 2, 6,
					ctx->accent.modify_alpha( 255 * ctx->animation ), 3
				);
			}
		}
		
		sdk::drawing::rect_filled(
			draw_pos.x, draw_pos.y, group_size.x, group_size.y, menu_colors[ 2 ].modify_alpha( 255 * ctx->animation ), 3
		);

		sdk::drawing::rect(
			draw_pos.x, draw_pos.y, group_size.x, group_size.y, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 3
		);

		/* yeah i know, thats retarded */
		sdk::drawing::text(
			draw_pos.x, draw_pos.y - 20, sdk::color::col_t( ).modify_alpha( ( sdk::input::input_sys::get( )->is_in_box( draw_pos, group_size ) ? 255 : 150 ) * hover_animation2.at( i ) ), sdk::drawing::c_fonts::verdana, name.c_str( )
		);
		sdk::drawing::text(
			draw_pos.x, draw_pos.y - 20, sdk::color::col_t( ).modify_alpha( 150 * ctx->animation ), sdk::drawing::c_fonts::verdana, name.c_str( )
		);

		/* store */
		gp = draw_pos;
		gs = group_size;

		// viewport	
		//draw_list->PushClipRect( { draw_pos.x, draw_pos.y + 5 }, { group_size.x, group_size.y - 7 }, true ); // i guess this works
		draw_list->PushClipRect( { draw_pos.x, draw_pos.y }, { draw_pos.x + group_size.x, draw_pos.y + group_size.y }, true );

		gui::helpers::push_cusor_pos( cursor_pos + sdk::math::vec2_t( 22, 23 + scrolling[ id ] ) );

		ctx->parent = "root." + ctx->tabs[ ctx->active_tab ] + "." + name;
		ctx->next_group_pos = cursor_pos + sdk::math::vec2_t( 0, group_size.y + 30 );

		ctx->parent_pos = draw_pos;
		ctx->parent_size = group_size;
	}
}

void gui::child::end_child( ) {
	sdk::math::vec2_t v1 = gui::helpers::pop_cursor_pos( );
	int max_height = ( int )v1.y - ( ( int )gp.y - ( int )ctx->pos.y ) - scrolling[ id ];
	int visible_height = gs.y + 5;
	gui::helpers::push_cusor_pos( v1 );

	int scrollbar_height, scrollbar_pos;

	if ( max_height > visible_height ) {
		scrollbar_height = ( ( float )gs.y / ( float )max_height ) * gs.y;
		scrollbar_pos = std::fmin( std::fmax( ( -( float )scrolling[ id ] / ( float )max_height ) * ( float )gs.y, 2.f ), gs.y - scrollbar_height - 2.f );

		bool ScrollBarHovered = sdk::input::input_sys::get()->is_in_box( gp + sdk::math::vec2_t( gs.x - 6, scrollbar_pos ), sdk::math::vec2_t( 4, scrollbar_height ) );

		if ( !ctx->hovered_listbox && !ctx->multi_dropdown_info.dragging_scroll && !ctx->dropdown_info.dragging_scroll ) {
			if ( ctx->focused_id == 0 || ctx->focused_id == id ) {
				bool bHoveredMain = sdk::input::input_sys::get( )->is_in_box( gp, gs - sdk::math::vec2_t( 6, 0 ) );
				bool bHoveredScroll = sdk::input::input_sys::get( )->is_in_box( gp + sdk::math::vec2_t( gs.x - 6, scrollbar_pos ), sdk::math::vec2_t( 4, scrollbar_height ) );

				if ( bHoveredMain || bHoveredScroll || ctx->focused_id == id ) {
					if ( ctx->focused_id != id && sdk::input::down( VK_LBUTTON ) && bHoveredScroll ) {
						ctx->focused_id = id;
					} else if ( ctx->focused_id == id ) {
						if ( sdk::input::down( VK_LBUTTON ) ) {
							const auto scale = [ ]( int in, int bmin, int bmax, int lmin, int lmax ) {
								return float( ( lmax - lmin ) * ( in - bmin ) ) / float( bmax - bmin ) + lmin;
							};

							// i think not perfect
							auto pizdo = std::fmax( float( float( gs.y * ( gs.y - 12 * 2 ) )
												   / float( -max_height + ( gs.y - 12 * 2 ) ) ), 30.f );

							scrolling[ id ] += scale( sdk::input::mouse_delta.y, 0, gs.y - pizdo, 0, max_height );
						} else {
							ctx->focused_id = 0;
						}
					}

					if ( ctx->focused_id != id ) {
						if ( sdk::input::input_sys::get( )->get_scroll_mouse( ) > 0 ) {
							scrolling[ id ] = scrolling[ id ] + 15;
						} else if ( sdk::input::input_sys::get( )->get_scroll_mouse( ) < 0 ) {
							scrolling[ id ] = scrolling[ id ] - 15;
						}
					}
				}

				ctx->dragging_scrollbar = ctx->focused_id == id;

				scrolling[ id ] = std::clamp<float>( scrolling[ id ], -max_height + ( int )gs.y, 0 );
			}
		}
	} else {
		scrolling[ id ] = 0;
	}

	gui::helpers::push_cusor_pos( ctx->next_group_pos );
	ctx->next_group_pos = sdk::math::vec2_t( 0, 0 );
	draw_list->PopClipRect( );
}

void gui::child::set_next_size( const sdk::math::vec2_t& size ) {
	override_size = size;
}

void gui::child::pop_last_size( ) {
	override_size = { -1, -1 };
}