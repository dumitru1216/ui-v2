#include "../c_warper.hpp"
#include "../../../hooking/c_hooks.hpp"

sdk::c_function sdk::warper::impl::alloc_console( ) {
	AllocConsole( );
}

sdk::c_bool sdk::warper::impl::take_client_rect( c_hwnd window_handle, RECT* screen_rect ) {
    if ( window_handle && screen_rect ) { /* safety condition */
        return GetClientRect( window_handle, screen_rect );
    }

    /* for any other values we are going to return false */
    return FALSE;
}

sdk::c_bool sdk::warper::impl::take_window_rect( c_hwnd window_handle, RECT* screen_rect ) {
    /*
        might be an issue the operator from screen_rect, we are going to see later
    */
    if ( window_handle && screen_rect ) { /* safety condition */
        return GetWindowRect( window_handle, screen_rect );
    }

    /* for any other values we are going to return false */
    return FALSE;
}

sdk::c_bool sdk::warper::impl::unregister_class( LPCSTR lp_class_name, HINSTANCE instance ) {
    return UnregisterClass( lp_class_name, instance );
}

sdk::c_bool sdk::warper::impl::destroy_window( c_hwnd hwnd ) {
    return DestroyWindow( hwnd );
}

sdk::c_bool sdk::warper::impl::show_window( c_hwnd hwnd, int n_cmd_show ) {
    return ShowWindow( hwnd, n_cmd_show );
}

sdk::c_bool sdk::warper::impl::update_window( c_hwnd hwnd ) {
    return UpdateWindow( hwnd );
}

sdk::c_bool sdk::warper::impl::peek_message( LPMSG lp_msg, c_hwnd hwnd, UINT w_msfilter_min, UINT w_msfilter_max,
                                             UINT w_remove_msg ) {
    return PeekMessage( lp_msg, hwnd, w_msfilter_min, w_msfilter_max, w_remove_msg );
}

sdk::c_bool sdk::warper::impl::translate_message( const MSG* lp_msg ) {
    return TranslateMessage( lp_msg );
}

sdk::c_lresult sdk::warper::impl::dispatch_message( const MSG* lp_msg ) {
    return DispatchMessage( lp_msg );
}

sdk::c_lresult sdk::warper::impl::def_window_proc( c_hwnd hwnd, UINT u_msg, WPARAM w_param, LPARAM l_param ) {
    return DefWindowProcA( hwnd, u_msg, w_param, l_param );
}

sdk::c_atom sdk::warper::impl::register_class( const WNDCLASSEX& wcex ) {
    return RegisterClassEx( &wcex );
}