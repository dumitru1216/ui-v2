#include "../c_hooks.hpp"

/*
	note @dutu: we could have used .get()-> on c_warp for better looking but i dont want to get 
	undefined behavior when the object its going to delete itself
*/

sdk::c_function i::hooks::impl::initialize( ) {

}

sdk::c_function i::hooks::impl::clean_device( ) {
	if ( device ) {
		sdk::warper::c_warp->release_device( &device );
	}

	if ( device_x ) {
		sdk::warper::c_warp->release_device_pointer( &device_x );
	}
}

sdk::c_function i::hooks::impl::reset_device( ) {
	sdk::c_result result{};

	/* store out in result */
	result = sdk::warper::c_warp->reset_device( device_parameter );
	if ( result != D3DERR_INVALIDCALL ) { }
}