#pragma once
#include "../../c_includes.hpp"

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

		bool picking_alpha = false;
		bool picking_color = false;
		bool picking_hue = false;

		bool copying_color = false;
		bool pasting_color = false;

		int action_crg;
		int size = 0.f;
	};
}