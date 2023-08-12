#include "../c_text_box.hpp"
#include "../../../gui/c_gui.hpp"
#include "../../../../sdk/notify/c_notify.hpp"
#include "../../../../sdk/easing/c_easing.hpp"

bool gui::controls::text_box( const sdk::c_str& name, sdk::c_str* input, int max_text ) {
	sdk::math::vec2_t cursor_pos = gui::helpers::pop_cursor_pos( );
	sdk::math::vec2_t draw_pos = ctx->pos + cursor_pos;
	sdk::math::vec2_t draw_size( std::fmin( ( int )ctx->parent_size.x - 90, 270 ), 28 );


}