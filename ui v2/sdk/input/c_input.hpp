#pragma once
#include "../../c_includes.hpp"

namespace sdk {
    namespace virtual_keys {
        enum {
            left_button = 0x01,
            right_button = 0x02,
            cancel = 0x03,
            middle_button = 0x04,
            extra_button1 = 0x05,
            extra_button2 = 0x06,
            back = 0x08,
            tab = 0x09,
            clear = 0x0C,
            return_key = 0x0D,
            shift = 0x10,
            control = 0x11,
            menu = 0x12,
            pause = 0x13,
            caps_lock = 0x14,
            kana = 0x15,
            hangeul = 0x15,
            hangul = 0x15,
            junja = 0x17,
            final = 0x18,
            hanja = 0x19,
            kanji = 0x19,
            escape = 0x1B,
            convert = 0x1C,
            non_convert = 0x1D,
            accept = 0x1E,
            mode_change = 0x1F,
            space = 0x20,
            prior = 0x21,
            next = 0x22,
            end = 0x23,
            home = 0x24,
            left = 0x25,
            up = 0x26,
            right = 0x27,
            down = 0x28,
            select = 0x29,
            print = 0x2A,
            execute = 0x2B,
            snapshot = 0x2C,
            insert = 0x2D,
            delete_key = 0x2E,
            help = 0x2F,
            n0 = 0x30,
            n1 = 0x31,
            n2 = 0x32,
            n3 = 0x33,
            n4 = 0x34,
            n5 = 0x35,
            n6 = 0x36,
            n7 = 0x37,
            n8 = 0x38,
            n9 = 0x39,
            a = 0x41,
            b = 0x42,
            c = 0x43,
            d = 0x44,
            e = 0x45,
            f = 0x46,
            g = 0x47,
            h = 0x48,
            i = 0x49,
            j = 0x4A,
            k = 0x4B,
            l = 0x4C,
            m = 0x4D,
            n = 0x4E,
            o = 0x4F,
            p = 0x50,
            q = 0x51,
            r = 0x52,
            s = 0x53,
            t = 0x54,
            u = 0x55,
            v = 0x56,
            w = 0x57,
            x = 0x58,
            y = 0x59,
            z = 0x5A,
            left_windows = 0x5B,
            right_windows = 0x5C,
            application = 0x5D,
            sleep = 0x5F,
            numpad0 = 0x60,
            numpad1 = 0x61,
            numpad2 = 0x62,
            numpad3 = 0x63,
            numpad4 = 0x64,
            numpad5 = 0x65,
            numpad6 = 0x66,
            numpad7 = 0x67,
            numpad8 = 0x68,
            numpad9 = 0x69,
            multiply = 0x6A,
            add = 0x6B,
            separator = 0x6C,
            subtract = 0x6D,
            decimal = 0x6E,
            divide = 0x6F,
            f1 = 0x70,
            f2 = 0x71,
            f3 = 0x72,
            f4 = 0x73,
            f5 = 0x74,
            f6 = 0x75,
            f7 = 0x76,
            f8 = 0x77,
            f9 = 0x78,
            f10 = 0x79,
            f11 = 0x7A,
            f12 = 0x7B,
            f13 = 0x7C,
            f14 = 0x7D,
            f15 = 0x7E,
            f16 = 0x7F,
            f17 = 0x80,
            f18 = 0x81,
            f19 = 0x82,
            f20 = 0x83,
            f21 = 0x84,
            f22 = 0x85,
            f23 = 0x86,
            f24 = 0x87,
            num_lock = 0x90,
            scroll_lock = 0x91,
            nec_equal = 0x92,
            fujitsu_jisho = 0x92,
            fujitsu_masshou = 0x93,
            fujitsu_touroku = 0x94,
            fujitsu_loya = 0x95,
            fujitsu_roya = 0x96,
            left_shift = 0xA0,
            right_shift = 0xA1,
            left_control = 0xA2,
            right_control = 0xA3,
            left_menu = 0xA4,
            right_menu = 0xA5,
            browser_back = 0xA6,
            browser_forward = 0xA7,
            browser_refresh = 0xA8,
            browser_stop = 0xA9,
            browser_search = 0xAA,
            browser_favorites = 0xAB,
            browser_home = 0xAC,
            volume_mute = 0xAD,
            volume_down = 0xAE,
            volume_up = 0xAF,
            media_next_track = 0xB0,
            media_prev_track = 0xB1,
            media_stop = 0xB2,
            media_play_pause = 0xB3,
            launch_mail = 0xB4,
            launch_media_select = 0xB5,
            launch_application1 = 0xB6,
            launch_application2 = 0xB7,
            oem1 = 0xBA,
            oem_plus = 0xBB,
            oem_comma = 0xBC,
            oem_minus = 0xBD,
            oem_period = 0xBE,
            oem2 = 0xBF,
            oem3 = 0xC0,
            oem4 = 0xDB,
            oem5 = 0xDC,
            oem6 = 0xDD,
            oem7 = 0xDE,
            oem8 = 0xDF,
            oemax = 0xE1,
            oem102 = 0xE2,
            ico_help = 0xE3,
            ico00 = 0xE4,
            process_key = 0xE5,
            ico_clear = 0xE6,
            packet = 0xE7,
            oem_reset = 0xE9,
            oem_jump = 0xEA,
            oempa1 = 0xEB,
            oempa2 = 0xEC,
            oempa3 = 0xED,
            oem_ws_ctrl = 0xEE,
            oem_cu_sel = 0xEF,
            oemattn = 0xF0,
            oem_finish = 0xF1,
            oem_copy = 0xF2,
            oem_auto = 0xF3,
            oem_enlw = 0xF4,
            oem_back_tab = 0xF5,
            attn = 0xF6,
            cr_sel = 0xF7,
            ex_sel = 0xF8,
            ereof = 0xF9,
            play = 0xFA,
            zoom = 0xFB,
            noname = 0xFC,
            pa1 = 0xFD,
            oem_clear = 0xFE,
        };
    }

	namespace input {
		enum class c_key_state : int {
			none = 1,
			down,
			up,
			pressed,
		};

		inline bool key_state[ 256 ];
		inline bool prev_key_state[ 256 ];

		inline int scroll;

		inline math::vec2_t mouse_pos;
		inline math::vec2_t mouse_delta;

		c_function update( );

		bool down( int key );
		bool released( int key );
		bool pressed( int key );

		/* class */
		class input_sys : public sdk::protection::non_copyable {
		public:
			static sdk::protection::encrypted_t<input_sys> get( );

			virtual bool initialize( IDirect3DDevice9* pDevice ) = 0;
			virtual sdk::c_function destroy( ) = 0;

			virtual sdk::c_function* get_main_window( ) const = 0;
			virtual c_key_state get_key_state( int vk ) = 0;
			virtual bool is_key_down( int vk ) = 0;
			virtual bool was_key_pressed( int vk ) = 0;
			virtual bool is_in_box( sdk::math::vec2_t box_pos, sdk::math::vec2_t box_size ) = 0;
			virtual sdk::c_function register_hotkey( int vk, std::function<sdk::c_function( sdk::c_function )> f ) = 0;
			virtual sdk::c_function remove_hotkey( int vk ) = 0;

			virtual sdk::math::vec2_t get_mouse_position( ) = 0;
			virtual float get_scroll_mouse( ) = 0;
			virtual sdk::c_function set_scroll_mouse( float scroll ) = 0;

		protected:
			input_sys( ) { };
			virtual ~input_sys( ) { };
		};
	}
}