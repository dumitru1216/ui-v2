#include "../c_warper.hpp"
#include "../../../hooking/c_hooks.hpp"

sdk::c_function sdk::warper::impl::set_vertex_shader( c_dev* device, IDirect3DVertexShader9* shader ) {
	device->SetVertexShader( shader );
}

sdk::c_function sdk::warper::impl::set_pixel_shader( c_dev* device, IDirect3DPixelShader9* shader ) {
	device->SetPixelShader( shader );
}

sdk::c_function sdk::warper::impl::set_render_state( c_dev* device, D3DRENDERSTATETYPE state, c_dword value ) {
	device->SetRenderState( state, value );
}

sdk::c_function sdk::warper::impl::set_texture_stage_state( c_dev* device, c_dword stage, D3DTEXTURESTAGESTATETYPE type, c_dword value ) {
	device->SetTextureStageState( stage, type, value );
}

sdk::c_function sdk::warper::impl::set_sampler_state( c_dev* device, c_dword sampler, D3DSAMPLERSTATETYPE type, c_dword value ) {
	device->SetSamplerState( sampler, type, value );
}

sdk::c_function sdk::warper::impl::set_transform( c_dev* device, D3DTRANSFORMSTATETYPE state, const D3DMATRIX* matrix ) {
	device->SetTransform( state, matrix );
}

sdk::c_function sdk::warper::impl::set_viewport( c_dev* device, const c_viewport* viewport ) {
	device->SetViewport( viewport );
}

sdk::c_result sdk::warper::impl::begin_scene( c_device device ) {
	return device->BeginScene( );
}

sdk::c_result sdk::warper::impl::end_scene( c_device device ) {
	return device->EndScene( );
}

sdk::c_function sdk::warper::impl::warp_dx( c_device_parameter* destination ) {
	destination->Windowed = TRUE;
	destination->SwapEffect = D3DSWAPEFFECT_DISCARD;
	destination->BackBufferFormat = D3DFMT_UNKNOWN;
	destination->EnableAutoDepthStencil = TRUE;
	destination->AutoDepthStencilFormat = D3DFMT_D16;
	destination->PresentationInterval = D3DPRESENT_INTERVAL_ONE;
}

sdk::c_function sdk::warper::impl::release_device( c_device* device ) {
	if ( device && *device ) {
		( *device )->Release( );
		*device = nullptr;
	}
}

sdk::c_function sdk::warper::impl::release_device_pointer( c_device_x* device_pointer ) {
	if ( device_pointer && *device_pointer ) {
		( *device_pointer )->Release( );
		*device_pointer = nullptr;
	}
}

sdk::c_result sdk::warper::impl::reset_device( c_device_parameter reset_parrameter ) {
	return device->Reset( &reset_parrameter );
}

sdk::c_result sdk::warper::impl::clear( c_device device, c_dword count, const D3DRECT* p_rects, c_dword flags, D3DCOLOR color, float z, c_dword stencil ) {
	return device->Clear( count, p_rects, flags, color, z, stencil );
}

sdk::c_result sdk::warper::impl::create_device_window( c_device_x* p_d3d, UINT adapter, D3DDEVTYPE device_type, c_hwnd h_focus_window,
									c_dword behavior_flags, c_device_parameter* presentation_parameters, c_device pp_device ) {
	if ( !p_d3d )
		return NULL;

	return ( *p_d3d )->CreateDevice( adapter, device_type, h_focus_window, behavior_flags, presentation_parameters, &pp_device );
}

sdk::c_device_x sdk::warper::impl::create_device_warper( c_uint32 sdk_version ) {
	return Direct3DCreate9( sdk_version );
}

sdk::c_function sdk::warper::impl::warp_frame( ) {
	ImGui_ImplDX9_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
}

sdk::c_function sdk::warper::impl::begin_frame( ) {
	ImGui::NewFrame( );
}

sdk::c_function sdk::warper::impl::end_frame( ) {
	ImGui::EndFrame( );
}