#include "../c_gui.hpp"

sdk::math::vec2_t last_cursor_pos;
sdk::math::vec2_t gui::helpers::pop_cursor_pos( ) {
	if ( ctx->cursor_pos_stack.empty( ) )
		return sdk::math::vec2_t( );

	sdk::math::vec2_t ret = ctx->cursor_pos_stack.top( );
	ctx->cursor_pos_stack.pop( );
	last_cursor_pos = ret;
	return ret;
}

sdk::math::vec2_t gui::helpers::get_last_cursor_pos( ) {
	return last_cursor_pos;
}

sdk::c_function gui::helpers::push_cusor_pos( const sdk::math::vec2_t& cursor_pos ) {
	ctx->cursor_pos_stack.push( cursor_pos );
}

sdk::c_size gui::helpers::hash( const sdk::c_str& name ) {
	return std::hash< sdk::c_str >( )( name );
}

float gui::helpers::animation_interval( float interval ) {
	return ( 1.0f / interval ) * 0.02;
}

float gui::helpers::map_number( float input, float input_min, float input_max, float output_min, float output_max ) {
	return ( input - input_min ) / ( input_max - input_min ) * ( output_max - output_min ) + output_min;
}

sdk::c_str_vec gui::helpers::split_str( const sdk::c_str& str, char separator ) {
	std::vector<sdk::c_str> output;
	sdk::c_str::size_type prev_pos = 0, pos = 0;

	while ( ( pos = str.find( separator, pos ) ) != sdk::c_str::npos ) {
		sdk::c_str substring( str.substr( prev_pos, pos - prev_pos ) );
		output.push_back( substring );

		prev_pos = pos++;
	}

	output.push_back( str.substr( prev_pos, pos - prev_pos ) );
	return output;
}