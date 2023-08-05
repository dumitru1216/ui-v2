#include "../c_window.hpp"
#include "../../../gui/c_gui.hpp"

bool title_bar_hovered;
sdk::c_str form_name;

/* reset window */
sdk::c_function gui::window::impl::reset( ) {
    if ( gui::ctx->focused_id != 0 )
        gui::ctx->focused_id = 0;

    if ( gui::ctx->dropdown_info.hashed_id != 0 )
        gui::ctx->dropdown_info.hashed_id = 0;

    if ( gui::ctx->colorpicker_info.hashed_id != 0 )
        gui::ctx->colorpicker_info.hashed_id = 0;

    if ( gui::ctx->multi_dropdown_info.hashed_id != 0 )
        gui::ctx->multi_dropdown_info.hashed_id = 0;

    if ( gui::ctx->dragging )
        gui::ctx->dragging = false;

    if ( gui::ctx->typing )
        gui::ctx->typing = false;
}

bool gui::window::begin_window( sdk::c_str name ) {
    static bool b_reset_window = true;
    if ( ctx->animation <= 0.0f ) {
        if ( b_reset_window ) {
            c_window->reset( );
            b_reset_window = false;
        }

        return false;
    }

    /* reset window */
    b_reset_window = true;

    if ( ctx->focused_id == 0 )
        ctx->typing = false;

    title_bar_hovered = sdk::input::input_sys::get( )->is_in_box( ctx->pos, sdk::math::vec2_t( ctx->size.x, 25 ) );

    /* safety */
    if ( ctx->focused_id == 0 ) {
        static sdk::math::vec2_t prev_mouse_pos;
        static sdk::math::vec2_t mouse_delta;

        /* get mouse pos */
        mouse_delta = prev_mouse_pos - sdk::input::input_sys::get( )->get_mouse_position( );

        /* move menu */
        if ( !ctx->dragging_scrollbar ) {
            if ( !ctx->dragging && sdk::input::input_sys::get( )->is_key_down( VK_LBUTTON ) && title_bar_hovered )
                ctx->dragging = true;
            else if ( ctx->dragging && sdk::input::input_sys::get( )->is_key_down( VK_LBUTTON ) )
                ctx->pos -= mouse_delta;
            else if ( ctx->dragging && !sdk::input::input_sys::get( )->is_key_down( VK_LBUTTON ) )
                ctx->dragging = false;
        }

        prev_mouse_pos = sdk::input::input_sys::get( )->get_mouse_position( );
    }

    /* screen size */
    /* note: if you have bigger/smaller screen change it */
    sdk::math::vec2_t screen{ 1920, 1080 };

    /* clamp size */
    ctx->size.x = std::clamp<int>( ctx->size.x, 648, screen.x );
    ctx->size.y = std::clamp<int>( ctx->size.y, 538, screen.y );

    /* clamp pos */
    ctx->pos.x = std::clamp<int>( ctx->pos.x, 0, screen.x - ctx->size.x );
    ctx->pos.y = std::clamp<int>( ctx->pos.y, 0, screen.y - ctx->size.y );

    /* swap cursor pos stack */
    std::stack< sdk::math::vec2_t >( ).swap( ctx->cursor_pos_stack );

    /* menu animation */
    sdk::alpha_mod = ctx->animation;

    /* menu outline */
    static sdk::color::col_t menu_outline[ 6 ] = {
        sdk::color::col_t( 60, 60, 60 ),
        sdk::color::col_t( 40, 40, 40 ),
        sdk::color::col_t( 40, 40, 40 ),
        sdk::color::col_t( 40, 40, 40 ),
        sdk::color::col_t( 60, 60, 60 ),
        sdk::color::col_t( 10, 10, 10 )
    };
    
    /* render menu outline */
    for ( int rects = 1; rects < 7; ++rects ) {
        sdk::drawing::rect(
            ctx->pos.x - rects, ctx->pos.y - rects, ( ctx->size.x - 1 ) + ( rects * 2 ), ( ctx->size.y - 1 ) + ( rects * 2 ), 
            menu_outline[ rects - 1 ].modify_alpha( 255 )
        );
    }

    return true;
}

void gui::window::end_window( ) {
    if ( ctx->dropdown_info.hashed_id != 0 ) {
        sdk::math::vec2_t pos = ctx->dropdown_info.pos;
        int width = ctx->dropdown_info.size;

        if ( sdk::input::pressed( VK_LBUTTON ) ) {
            ctx->dropdown_info.hashed_id = 0;
            ctx->focused_id = 0;
        }
    }

    if ( ctx->multi_dropdown_info.hashed_id != 0 ) {
        sdk::math::vec2_t pos = ctx->multi_dropdown_info.pos;
        int width = ctx->multi_dropdown_info.size;

    }

    sdk::alpha_mod = -1.f;
}