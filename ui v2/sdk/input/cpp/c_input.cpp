#include "../c_input.hpp"
#include "../../notify/c_notify.hpp"

/* thanks alpha */
sdk::c_function sdk::input::update( ) {
	if ( GetForegroundWindow( ) == window ) {
		for ( sdk::c_size i = 0; i < 256; i++ ) {
			prev_key_state[ i ] = key_state[ i ];
			key_state[ i ] = GetAsyncKeyState( i );
		}
	}

	static sdk::c_hwnd f_window;
	if ( !f_window )
		f_window = FindWindowA( "Valve001", 0 ); // ui-v2

	sdk::c_point mouse;
	GetCursorPos( &mouse );
	ScreenToClient( f_window, &mouse );

	mouse_delta = mouse_pos - sdk::math::vec2_t( mouse.x, mouse.y );
	mouse_pos = sdk::math::vec2_t( mouse.x, mouse.y );
}

bool sdk::input::down( int key ) {
	return key_state[ key ] && prev_key_state[ key ];
}

bool sdk::input::pressed( int key ) {
	return key_state[ key ] && !prev_key_state[ key ];
}

bool sdk::input::released( int key ) {
	return !key_state[ key ] && prev_key_state[ key ];
}

/* main input system */
class win32_input_sys : public sdk::input::input_sys {
public:
    win32_input_sys( );
    virtual ~win32_input_sys( );

    virtual bool initialize( IDirect3DDevice9* pDevice );
    virtual sdk::c_function destroy( );

    virtual sdk::c_function* get_main_window( ) const { return ( sdk::c_function* )m_target_window; }

    virtual sdk::input::c_key_state get_key_state( int vk );
    virtual bool is_key_down( int vk );
    virtual bool is_in_box( sdk::math::vec2_t box_pos, sdk::math::vec2_t box_size );
    virtual bool was_key_pressed( int vk );

    virtual sdk::c_function register_hotkey( int vk, std::function<sdk::c_function( sdk::c_function )> f );
    virtual sdk::c_function remove_hotkey( int vk );

    virtual sdk::math::vec2_t get_mouse_position( ) {
        return m_mouse_pos;
    }

    virtual float get_scroll_mouse( ) {
        return m_scroll_mouse;
    }

    virtual sdk::c_function set_scroll_mouse( float scroll ) {
        m_scroll_mouse = scroll;
    }

private:
    static sdk::c_lresult WINAPI wnd_proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

    bool process_message( UINT uMsg, WPARAM wParam, LPARAM lParam );
    bool process_mouse_message( UINT uMsg, WPARAM wParam, LPARAM lParam );
    bool process_keybd_message( UINT uMsg, WPARAM wParam, LPARAM lParam );

    float m_scroll_mouse = 0.0f;

    HWND m_target_window;
    LONG_PTR m_old_wnd_proc;
    sdk::input::c_key_state m_key_map[ 256 ];

    sdk::math::vec2_t m_mouse_pos;

    std::function<sdk::c_function( sdk::c_function )> m_hotkeys[ 256 ];
};

sdk::protection::encrypted_t<sdk::input::input_sys> sdk::input::input_sys::get( ) {
    static win32_input_sys instance;
    return &instance;
}

win32_input_sys::win32_input_sys( ) :
    m_target_window( nullptr ), m_old_wnd_proc( 0 ) { }

win32_input_sys::~win32_input_sys( ) { }

/* init */
bool win32_input_sys::initialize( IDirect3DDevice9* pDevice ) {
    D3DDEVICE_CREATION_PARAMETERS params;

    if ( FAILED( pDevice->GetCreationParameters( &params ) ) ) {
        sdk::c_notify->log( "failed to create parameters" );
        return false;
    }

    m_target_window = params.hFocusWindow;
    m_old_wnd_proc = SetWindowLongPtr( m_target_window, GWLP_WNDPROC, ( LONG_PTR )wnd_proc );

    if ( !m_old_wnd_proc ) {
        sdk::c_notify->log( "failed to set window processor" );
        return false;
    }

    return true;
}

sdk::c_function win32_input_sys::destroy( ) {
    if ( m_old_wnd_proc )
        SetWindowLongPtr( m_target_window, GWLP_WNDPROC, m_old_wnd_proc );
    m_old_wnd_proc = 0;
}

LRESULT __stdcall win32_input_sys::wnd_proc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    auto win32input = static_cast< win32_input_sys* >( get( ).xor_str( ) );
    win32input->process_message( msg, wParam, lParam );

    if ( msg == WM_MOUSEMOVE ) {
        win32input->m_mouse_pos.x = ( signed short )( lParam );
        win32input->m_mouse_pos.y = ( signed short )( lParam >> 16 );
    } else if ( msg == WM_MOUSEWHEEL || msg == WM_MOUSEHWHEEL ) {
        win32input->m_scroll_mouse += ( float )GET_WHEEL_DELTA_WPARAM( wParam ) / ( float )WHEEL_DELTA;
    }

    if ( sdk::g::c_menu_open ) {
        if ( msg == WM_MOUSEWHEEL || msg == WM_MOUSEHWHEEL )
            return true;
    }

    return CallWindowProc( ( WNDPROC )win32input->m_old_wnd_proc, hWnd, msg, wParam, lParam );
}

bool win32_input_sys::process_message( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
    switch ( uMsg ) {
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_LBUTTONDBLCLK:
        case WM_XBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_XBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_XBUTTONUP:
        return process_mouse_message( uMsg, wParam, lParam );
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        return process_keybd_message( uMsg, wParam, lParam );
        default:
        return false;
    }
}

bool win32_input_sys::process_mouse_message( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
    auto key = VK_LBUTTON;
    auto state = sdk::input::c_key_state::none;

    switch ( uMsg ) {
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        state = uMsg == WM_MBUTTONUP ? sdk::input::c_key_state::up : sdk::input::c_key_state::down;
        key = VK_MBUTTON;
        break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        state = uMsg == WM_RBUTTONUP ? sdk::input::c_key_state::up : sdk::input::c_key_state::down;
        key = VK_RBUTTON;
        break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        state = uMsg == WM_LBUTTONUP ? sdk::input::c_key_state::up : sdk::input::c_key_state::down;
        key = VK_LBUTTON;
        break;
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        state = uMsg == WM_XBUTTONUP ? sdk::input::c_key_state::up : sdk::input::c_key_state::down;
        key = ( HIWORD( wParam ) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2 );
        break;
        default:
        return false;
    }

    if ( state == sdk::input::c_key_state::up && m_key_map[ key ] == sdk::input::c_key_state::down ) {
        m_key_map[ key ] = sdk::input::c_key_state::pressed;
    } else
        m_key_map[ key ] = state;
    return true;
}

bool win32_input_sys::process_keybd_message( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
    auto key = wParam;
    auto state = sdk::input::c_key_state::none;

    switch ( uMsg ) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        state = sdk::input::c_key_state::down;
        break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
        state = sdk::input::c_key_state::up;
        break;
        default:
        return false;
    }

    if ( state == sdk::input::c_key_state::up && m_key_map[ int( key ) ] == sdk::input::c_key_state::down ) {
        m_key_map[ int( key ) ] = sdk::input::c_key_state::pressed;

        auto& hotkey_callback = m_hotkeys[ key ];

        if ( hotkey_callback )
            hotkey_callback( );

    } else {
        m_key_map[ int( key ) ] = state;
    }

    return true;
}

sdk::input::c_key_state win32_input_sys::get_key_state( int vk ) {
    return m_key_map[ vk ];
}

bool win32_input_sys::is_key_down( int vk ) {
    if ( vk <= 0 || vk > 255 )
        return false;

    return m_key_map[ vk ] == sdk::input::c_key_state::down;
}

bool win32_input_sys::is_in_box( sdk::math::vec2_t box_pos, sdk::math::vec2_t box_size ) {
    return (
        m_mouse_pos.x > box_pos.x &&
        m_mouse_pos.y > box_pos.y &&
        m_mouse_pos.x < box_pos.x + box_size.x &&
        m_mouse_pos.y < box_pos.y + box_size.y
        );
}

bool win32_input_sys::was_key_pressed( int vk ) {
    if ( vk <= 0 || vk > 255 )
        return false;

    if ( m_key_map[ vk ] == sdk::input::c_key_state::pressed ) {
        m_key_map[ vk ] = sdk::input::c_key_state::up;
        return true;
    }

    return false;
}

void win32_input_sys::register_hotkey( int vk, std::function<void( void )> f ) {
    m_hotkeys[ vk ] = f;
}

void win32_input_sys::remove_hotkey( int vk ) {
    m_hotkeys[ vk ] = nullptr;
}
