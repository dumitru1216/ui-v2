#pragma once
#include "../../c_includes.hpp"
#include "../../sdk/input/c_input.hpp"

namespace gui {
	struct c_multi_item_t {
		sdk::c_str name;
		bool* value;
	};

	struct c_dropdown_info_t {
		sdk::math::vec2_t pos = { 0, 0 };
		sdk::c_size hashed_id;
		sdk::c_str_vec elements;

		int* option;
		int max_items;

		bool out_anim;
		bool dragging_scroll;

		float size = 0.f;
		std::map< sdk::c_size, float > u_scroll;
	};

	struct c_multi_dropdown_info_t {
		sdk::math::vec2_t pos = { 0, 0 };
		sdk::c_size hashed_id;

		int max_items;
		bool dragging_scroll;
		float size = 0.f;

		std::vector< c_multi_item_t > elements;
		std::map< sdk::c_size, float > u_scroll;
	};

	struct c_hotkey_info_t {
		sdk::math::vec2_t pos = { 0, 0 };
		sdk::c_size id;

		bool open = false;
	};

	struct c_colorpicker_info_t {
		sdk::math::vec2_t pos = { 0, 0 };
		sdk::c_size hashed_id;
		sdk::color::fcol_t* value;

		bool picking_alpha = false;
		bool picking_color = false;
		bool picking_hue = false;

		bool copying_color = false;
		bool pasting_color = false;

		int action_crg;
		int size = 0.f;
	};

	struct c_slider_info_t {
		std::map<sdk::c_size, float> value_animation;
		std::map<sdk::c_size, float> value_timer;
		std::map<sdk::c_size, float> preview_animation;
		std::map<sdk::c_size, float> prev_amount;
		std::map<sdk::c_size, float> last_change_time;
		std::map<sdk::c_size, std::pair<float, bool>> should_change_value;
	};

	struct c_menu_context {
		sdk::math::vec2_t pos = { 100, 100 };
		sdk::math::vec2_t size = { 568, 635 };

		sdk::color::col_t accent{ 235, 103, 52 };

		sdk::math::vec2_t next_group_pos;
		sdk::math::vec2_t parent_pos;
		sdk::math::vec2_t parent_size;

		std::stack< sdk::math::vec2_t > cursor_pos_stack;
		sdk::c_str_vec tabs;

		int active_tab = 0;

		/* elements shares */
		c_dropdown_info_t dropdown_info;
		c_multi_dropdown_info_t multi_dropdown_info;
		c_colorpicker_info_t colorpicker_info;
		c_hotkey_info_t hotkey_info;
		c_slider_info_t slider_info;

		/* other */
		sdk::c_str parent;
		sdk::c_str current_tab;
		sdk::c_str current_sub_tab;
		sdk::c_str current_group;
		sdk::c_str current_weapon_group;

		sdk::c_size focused_id = 0;

		bool dragging = false;
		bool resizing = false;
		bool setup = false;
		bool typing = false;
		bool hovered_listbox = false;
		bool dragging_scrollbar = false;

		float animation = 0.f;
	};

	/* setup gui */
	inline c_menu_context* ctx = new c_menu_context( );

	/* helpers */
	namespace helpers {
		inline int object_padding( ) {
			constexpr int spacing{ 10 };
			return spacing;
		}

		template < typename t = float >
		inline float approach( float a, float b, float multiplier ) {
			return ( a + static_cast< t >( multiplier * ( b - a ) ) );
		}

		sdk::math::vec2_t pop_cursor_pos( );
		sdk::math::vec2_t get_last_cursor_pos( );

		sdk::c_function push_cusor_pos( const sdk::math::vec2_t& cursor_pos );

		float animation_interval( float interval = 0.035f );
		float map_number( float input, float input_min, float input_max, float output_min, float output_max );

		sdk::c_str_vec split_str( const sdk::c_str& str, const char* separator );
		sdk::c_size hash( const sdk::c_str& name );
	}
}

/* here down we are going to do the includes */
#include "../controls/window/c_window.hpp"
#include "../controls/child/c_child.hpp"
#include "../controls/checkbox/c_checkbox.hpp"
