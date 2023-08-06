#include "../c_drawing.hpp"
#include "../../fonts/segoeuib.h"
#include "../../fonts/segoeui.h"
#include "../../fonts/icons.h"
#include "../../fonts/tahoma.h"
#include "../../fonts/acta.h"
#include "../../fonts/arrowmod.h"

/* init */ sdk::view_port::c_view_port* sdk::view_port::view_port;

ImU32 lerp_color( ImU32 col1, ImU32 col2, float t ) {
	ImVec4 c1 = ImGui::ColorConvertU32ToFloat4( col1 );
	ImVec4 c2 = ImGui::ColorConvertU32ToFloat4( col2 );
	ImVec4 lerped = ImVec4( c1.x + t * ( c2.x - c1.x ),
							c1.y + t * ( c2.y - c1.y ),
							c1.z + t * ( c2.z - c1.z ),
							c1.w + t * ( c2.w - c1.w ) );
	return ImGui::ColorConvertFloat4ToU32( lerped );
}

/* drawing */
sdk::c_function sdk::drawing::init_fonts( ) {
	ImGuiIO& io = ImGui::GetIO( );
	ImFontConfig cfg;
	cfg.FontDataOwnedByAtlas = false;


	io.Fonts->AddFontFromFileTTF( "c:/windows/fonts/smallest_pixel-7.ttf", 10.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	io.Fonts->AddFontFromFileTTF( "c:/windows/fonts/verdana.ttf", 11.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	io.Fonts->AddFontFromFileTTF( "c:/windows/fonts/verdanab.ttf", 11.f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	io.Fonts->AddFontFromMemoryTTF( fs_tahoma_8px_ttf, fs_tahoma_8px_ttf_len, 12.0f, &cfg, io.Fonts->GetGlyphRangesCyrillic( ) );
	io.Fonts->AddFontFromMemoryCompressedTTF( MenuIcons_compressed_data, MenuIcons_compressed_size, 21.0f, &cfg, io.Fonts->GetGlyphRangesJapanese( ) );

	io.Fonts->AddFontDefault( );

	ImGuiFreeType::BuildFontAtlas( io.Fonts, ImGuiFreeTypeBuilderFlags_Bitmap );
}

sdk::c_function sdk::drawing::rect_filled( int x, int y, int w, int h, color::col_t c, int rounding ) {
	draw_list->AddRectFilled( im_vec_2( x, y ), im_vec_2( x + w, y + h ), c.convert( ), rounding );
}

sdk::c_function sdk::drawing::filled_circle( math::vec2_t center, float radius, color::col_t c ) {
	draw_list->AddCircleFilled( ImVec2( center.x, center.y ), radius, c.convert( ) );
}

sdk::c_function sdk::drawing::gradient_circle_filled( sdk::math::vec2_t c, int radius, sdk::color::col_t inner_color, sdk::color::col_t outer_color ) {
	for ( int i = 0; i <= radius; ++i ) {
		float t = static_cast< float >( i ) / radius;
		ImU32 color = ImGui::GetColorU32( lerp_color( inner_color.convert( ), outer_color.convert( ), t ) );
		draw_list->AddCircleFilled( ImVec2( c.x, c.y ), static_cast< float >( i ), color );
	}
}

sdk::c_function sdk::drawing::rect( int x, int y, int w, int h, color::col_t c, int rounding ) {
	draw_list->AddRect( im_vec_2( x, y ), im_vec_2( x + w, y + h ), c.convert( ), rounding );
}

sdk::c_function sdk::drawing::gradient_vertical( int x, int y, int w, int h, color::col_t c1, color::col_t c2 ) {
	draw_list->AddRectFilledMultiColor( im_vec_2( x, y ), im_vec_2( x + w, y + h ), c1.convert( ), c2.convert( ), c2.convert( ), c1.convert( ) );
}

sdk::c_function sdk::drawing::gradient_horizontal( int x, int y, int w, int h, color::col_t c1, color::col_t c2 ) {
	draw_list->AddRectFilledMultiColor( im_vec_2( x, y ), im_vec_2( x + w, y + h ), c1.convert( ), c1.convert( ), c2.convert( ), c2.convert( ) );
}

sdk::c_function sdk::drawing::image( int x, int y, int w, int h, ImTextureID user_texture_id ) {
	draw_list->AddImage( user_texture_id, im_vec_2( x, y ), im_vec_2( x + w, y + h ) );
}

sdk::c_function sdk::drawing::text( float x, float y, color::col_t c, int font_index, const char* text, bool dropshadow ) {
	ImGuiIO& io = ImGui::GetIO( );
	ImGui::PushFont( io.Fonts->Fonts[ font_index ] );

	if ( dropshadow )
		draw_list->AddText( im_vec_2( x + 1.f, y + 1.f ), color::col_t( 5, 5, 5 ).convert( ), text );

	// run everything now
	draw_list->AddText( im_vec_2( x, y ), c.convert( ), text );
	ImGui::PopFont( );
}

im_vec_2 sdk::drawing::get_text_size( const char* text, int font_index ) {
	ImGuiIO& io = ImGui::GetIO( );
	ImGui::PushFont( io.Fonts->Fonts[ font_index ] );
	return ImGui::GetFont( )->CalcTextSizeA( ImGui::GetFontSize( ), FLT_MAX, -1.0f, text );
	ImGui::PopFont( );
}

void sdk::view_port::c_view_port::init_device( ) {
	/* setup shaders */
	dev->SetVertexShader( nullptr );
	dev->SetPixelShader( nullptr );
	dev->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
	dev->SetRenderState( D3DRS_LIGHTING, false );
	dev->SetRenderState( D3DRS_FOGENABLE, false );
	dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	dev->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	/* setup states */
	dev->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	dev->SetRenderState( D3DRS_SCISSORTESTENABLE, true );
	dev->SetRenderState( D3DRS_ZWRITEENABLE, false );
	dev->SetRenderState( D3DRS_STENCILENABLE, false );

	/* setup states for fonts */
	dev->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, true );
	dev->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, true );

	/* setup states for render system */
	dev->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_ALPHATESTENABLE, false );
	dev->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, true );
	dev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	dev->SetRenderState( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	dev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	dev->SetRenderState( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );

	/* setup states for color */
	dev->SetRenderState( D3DRS_SRGBWRITEENABLE, false );
	dev->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );

	/* setup states for textures */
	dev->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	dev->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	/* init samples */
	dev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	dev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
}