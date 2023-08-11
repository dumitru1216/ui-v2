#include "../c_window.hpp"
#include "../../../gui/c_gui.hpp"
#include "../../../../sdk/notify/c_notify.hpp"
#include "../../../../sdk/easing/c_easing.hpp"

bool title_bar_hovered;
sdk::c_str form_name;

std::map<int, float> select_animation;
std::map<int, float> select_timer;

std::map<int, float> hover_animation;
std::map<int, float> hover_timer;

std::map<int, float> tool_tip_animation;
std::map<int, float> tool_tip_timer;

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
    sdk::drawing::image( 0, 0, 1920, 1090, backround );

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
    static sdk::color::col_t menu_colors[ 3 ] = {
        sdk::color::col_t( 0, 0, 0 ), // outline 1
        sdk::color::col_t( 28, 28, 28 ), // outline 2
        sdk::color::col_t( 20, 20, 20 ) // backround
    };

    /* render menu */
    sdk::drawing::rect_filled( 
        ctx->pos.x, ctx->pos.y, ctx->size.x, ctx->size.y, menu_colors[ 2 ].modify_alpha( 255 * ctx->animation ), 3 
    );

    sdk::drawing::rect_filled(
        ctx->pos.x, ctx->pos.y, ctx->size.x, 37, menu_colors[ 0 ].modify_alpha( 255 * ctx->animation ), 3
    );

    sdk::drawing::rect_filled(
        ctx->pos.x, ctx->pos.y + 37, ctx->size.x, 1, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 3
    );

    sdk::drawing::text(
        ctx->pos.x + 10, ctx->pos.y + 10, ctx->accent.modify_alpha( 255 * ctx->animation ), sdk::drawing::c_fonts::verdanab, "d-ui"
    );

    sdk::drawing::text(
        ctx->pos.x + 10 + sdk::drawing::get_text_size( "d-ui", sdk::drawing::c_fonts::verdanab ).x, ctx->pos.y + 10, sdk::color::col_t( ).modify_alpha( 200 * ctx->animation ), sdk::drawing::c_fonts::verdana, "render"
    );

    /* lambada */
    auto to_vector = [ ]( float a, float b )-> sdk::math::vec2_t {
        return sdk::math::vec2_t( a, b );
    };

    for ( int i = 0; i < ctx->tabs.size( ); ++i ) {
        auto size_tab = sdk::drawing::get_text_size( ctx->tabs[ i ].c_str( ), sdk::drawing::c_fonts::icon );

        sdk::math::vec2_t tab_size = to_vector( size_tab.x, size_tab.y );
        sdk::math::vec2_t tab_pos = ctx->pos + sdk::math::vec2_t( ( 100 + ( ctx->size.x - 285 ) ) + ( 35 * i ), 10 );

        if ( hover_animation.find( i ) == hover_animation.end( ) ) {
            hover_animation.insert( { i, 0.f } );
            hover_timer.insert( { i, 0.f } );

            select_animation.insert( { i, 0.f } );
            select_timer.insert( { i, 0.f } );

            tool_tip_animation.insert( { i, 0.f } );
            tool_tip_timer.insert( { i, 0.f } );
        }

        if ( sdk::input::input_sys::get( )->is_in_box( tab_pos, sdk::math::vec2_t( 20, 20 ) ) ) {
            hover_timer.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
            hover_timer.at( i ) = std::clamp<float>( hover_timer.at( i ), 0.f, 1.f );

            hover_animation.at( i ) = sdk::easing::out_circ( hover_timer.at( i ) );

            if ( hover_animation.at( i ) >= 0.999998 ) {
                hover_animation.at( i ) = 1.f;
            }

            if ( hover_animation.at( i ) == 1.f ) {
                tool_tip_timer.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
                tool_tip_timer.at( i ) = std::clamp<float>( tool_tip_timer.at( i ), 0.f, 1.f );

                tool_tip_animation.at( i ) = sdk::easing::out_circ( tool_tip_timer.at( i ) );

                if ( tool_tip_animation.at( i ) >= 0.999998 ) {
                    tool_tip_animation.at( i ) = 1.f;
                }
            } else {
                tool_tip_timer.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
                tool_tip_timer.at( i ) = std::clamp<float>( tool_tip_timer.at( i ), 0.f, 1.f );

                tool_tip_animation.at( i ) = sdk::easing::out_circ( tool_tip_timer.at( i ) );

                if ( tool_tip_animation.at( i ) <= 0.05f ) {
                    tool_tip_animation.at( i ) = 0.f;
                }
            }
        } else {
            if ( i != ctx->active_tab ) {
                hover_timer.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
                hover_timer.at( i ) = std::clamp<float>( hover_timer.at( i ), 0.f, 1.f );

                hover_animation.at( i ) = sdk::easing::out_circ( hover_timer.at( i ) );

                if ( hover_animation.at( i ) <= 0.05f ) {
                    hover_animation.at( i ) = 0.f;
                }
            }
        }

        if ( i == ctx->active_tab ) {
            select_timer.at( i ) += ( 1.0f / 0.2f ) * 0.003f;
            select_timer.at( i ) = std::clamp<float>( select_timer.at( i ), 0.f, 1.f );

            select_animation.at( i ) = sdk::easing::out_circ( select_timer.at( i ) );

            if ( select_animation.at( i ) >= 0.999998 ) {
                select_animation.at( i ) = 1.f;
            }
        } else {
            select_timer.at( i ) -= ( 1.0f / 0.2f ) * 0.003f;
            select_timer.at( i ) = std::clamp<float>( select_timer.at( i ), 0.f, 1.f );

            select_animation.at( i ) = sdk::easing::out_circ( select_timer.at( i ) );

            if ( select_animation.at( i ) <= 0.05f ) {
                select_animation.at( i ) = 0.f;
            }
        }

        int add_to_tab{};
        if ( ctx->tabs[ i ] == "6" ) {
            add_to_tab = 5;
        }

        if ( ctx->active_tab != i ) {
            sdk::drawing::text(
                tab_pos.x + add_to_tab, tab_pos.y, sdk::color::col_t( 90, 90, 90 ), sdk::drawing::c_fonts::icon, ctx->tabs[i].c_str()
            );
        }

        if ( ctx->active_tab == i ) {
            sdk::drawing::rect_filled(
                tab_pos.x + add_to_tab - 6, tab_pos.y - 6, 32, 36, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 5
            );

            /* select */
            {
                int middle_x = tab_pos.x + add_to_tab + 10;
                int half_width = 16 * select_animation.at( i );

                sdk::drawing::rect_filled(
                    middle_x - half_width, tab_pos.y - 6, half_width + 1, 3,
                    ctx->accent.modify_alpha( 255 * ctx->animation ), 3
                );

                sdk::drawing::rect_filled(
                    middle_x, tab_pos.y - 6, half_width, 3,
                    ctx->accent.modify_alpha( 255 * ctx->animation ), 3
                );
            }
            
            sdk::drawing::rect_filled(
                tab_pos.x + add_to_tab - 5, tab_pos.y - 5, 30, 36, menu_colors[2].modify_alpha( 255 * ctx->animation ), 3
            );

            sdk::drawing::text(
                tab_pos.x + add_to_tab, tab_pos.y, ctx->accent.modify_alpha( 255 * ctx->animation ), sdk::drawing::c_fonts::icon, ctx->tabs[ i ].c_str( )
            );
        } else {
            if ( sdk::input::input_sys::get( )->is_in_box( tab_pos, sdk::math::vec2_t( 20, 20 ) ) ) {
                sdk::drawing::rect_filled(
                    tab_pos.x + add_to_tab - 6, tab_pos.y - 6, 32, 36, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 5
                );

                /* hover */
                {
                    int middle_x = tab_pos.x + add_to_tab + 10;
                    int half_width = 16 * hover_animation.at( i );

                    sdk::drawing::rect_filled(
                        middle_x - half_width, tab_pos.y - 6, half_width + 1, 6,
                        ctx->accent.modify_alpha( 150 * ctx->animation ), 3
                    );

                    sdk::drawing::rect_filled(
                        middle_x, tab_pos.y - 6, half_width, 6,
                        ctx->accent.modify_alpha( 150 * ctx->animation ), 3
                    );

                    /* tool tip */
                    {
                        if ( hover_animation.at( i ) == 1.f ) {
                            sdk::c_str tooltip_text{};

                            /* hardcodenz */
                            if ( ctx->tabs[ i ] == "i" ) {
                                tooltip_text = "ragebot";
                            } else if ( ctx->tabs[ i ] == "k" ) {
                                tooltip_text = "anti-hit";
                            } else if ( ctx->tabs[ i ] == "3" ) {
                                tooltip_text = "visuals";
                            } else if ( ctx->tabs[ i ] == "5" ) {
                                tooltip_text = "misc";
                            } else if ( ctx->tabs[ i ] == "6" ) {
                                tooltip_text = "profile";
                            }

                            /* some hardcodenz from dutu */
                            auto tooltip_size = sdk::drawing::get_text_size( tooltip_text.c_str( ), sdk::drawing::c_fonts::verdana );
                            auto integr_size{ tooltip_size.x + 10 };

                            sdk::drawing::rect_filled(
                                tab_pos.x + add_to_tab - 5, tab_pos.y + 30, integr_size, 27, sdk::color::col_t( 16, 16, 16 ).modify_alpha( 255 * tool_tip_timer.at( i ) ), 3
                            );
                            sdk::drawing::rect(
                                tab_pos.x + add_to_tab - 5, tab_pos.y + 30, integr_size, 27, menu_colors[ 1 ].modify_alpha( 255 * tool_tip_timer.at( i ) ), 3
                            );

                            sdk::drawing::text(
                                tab_pos.x + add_to_tab, tab_pos.y + 35, sdk::color::col_t( 150, 150, 150 ).modify_alpha( 255 * tool_tip_timer.at( i ) ), sdk::drawing::c_fonts::verdana, tooltip_text.c_str( )
                            );
                        }
                    }
                }

                sdk::drawing::rect_filled(
                    tab_pos.x + add_to_tab - 5, tab_pos.y - 5, 30, 36, menu_colors[ 2 ].modify_alpha( 255 * ctx->animation ), 3
                );

                sdk::drawing::text(
                    tab_pos.x + add_to_tab, tab_pos.y, sdk::color::col_t( 150, 150, 150 ).modify_alpha( 255 * ctx->animation ), sdk::drawing::c_fonts::icon, ctx->tabs[ i ].c_str( )
                );

                if ( sdk::input::input_sys::get( )->was_key_pressed( VK_LBUTTON ) ) {
                    ctx->active_tab = i;
                }       
            }
        }
    }

    /* render menu outline */
    sdk::drawing::rect(
        ctx->pos.x - 1, ctx->pos.y - 1, ctx->size.x + 2, ctx->size.y + 2,
        menu_colors[ 0 ].modify_alpha( 255 * ctx->animation ), 2
    );

    sdk::drawing::rect(
        ctx->pos.x, ctx->pos.y, ctx->size.x, ctx->size.y,
        menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 2
    );

    ctx->tabs.clear( );
    gui::helpers::push_cusor_pos( sdk::math::vec2_t(
        20, 60
    ) );

    return true;
}

void gui::window::end_window( ) {
    if ( ctx->dropdown_info.hashed_id != 0 ) {
        sdk::math::vec2_t pos = ctx->dropdown_info.pos;
        int width = ctx->dropdown_info.size;

        static sdk::color::col_t menu_colors[ 4 ] = {
             sdk::color::col_t( 0, 0, 0 ), // outline 1
             sdk::color::col_t( 49, 49, 49 ), // outline 2
             sdk::color::col_t( 37, 37, 37 ), // backround
             sdk::color::col_t( 42, 42, 42 ) // backround
        };

        auto draw_pos = pos + sdk::math::vec2_t( 0, 5 );
        auto draw_size = sdk::math::vec2_t( width, 20 * ctx->dropdown_info.elements.size( ) + 3 );
        sdk::drawing::rect_filled( draw_pos.x + 1, draw_pos.y + 1, draw_size.x - 2, draw_size.y - 2, menu_colors[ 2 ].modify_alpha( 255 * ctx->animation ), 2 );
        sdk::drawing::rect( draw_pos.x - 1, draw_pos.y - 1, draw_size.x + 2, draw_size.y + 2, menu_colors[ 0 ].modify_alpha( 255 * ctx->animation ), 2 );
        sdk::drawing::rect( draw_pos.x, draw_pos.y, draw_size.x, draw_size.y, menu_colors[ 1 ].modify_alpha( 255 * ctx->animation ), 2 );

        for ( int i = 0; i < ctx->dropdown_info.elements.size( ); i++ ) {
            sdk::math::vec2_t option_pos = pos + sdk::math::vec2_t( 0, 20 * i );
            sdk::math::vec2_t option_size = sdk::math::vec2_t( width, 20 );

            auto draw_pos = pos + sdk::math::vec2_t( 0, 5 );
            auto draw_size = sdk::math::vec2_t( width, 20 * i );

            sdk::drawing::text(
                option_pos.x + 10, option_pos.y + 8, ( *ctx->dropdown_info.option != i ) ? sdk::color::col_t( 200, 200, 200 ) : ctx->accent , sdk::drawing::c_fonts::verdana, ctx->dropdown_info.elements[ i ].c_str( )
            );

            if ( sdk::input::input_sys::get( )->is_in_box( option_pos, option_size ) ) {
                if ( sdk::input::input_sys::get( )->was_key_pressed( VK_LBUTTON ) ) {
                    *ctx->dropdown_info.option = i;
                    ctx->dropdown_info.hashed_id = 0;
                    ctx->focused_id = 0;
                }
            }
        }

        if ( sdk::input::input_sys::get()->was_key_pressed( VK_LBUTTON ) ) {
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

bool gui::window::begin_tab( sdk::c_str name ) {
    ctx->tabs.push_back( name.data( ) );
    ctx->current_tab = name;

    return ( ctx->active_tab == ctx->tabs.size( ) - 1 ) || ctx->setup;
}