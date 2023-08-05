#pragma once

namespace sdk {
	namespace protection {
		template<typename T>
		class encrypted_t {
		public:
			T* pointer;

			__forceinline encrypted_t( T* ptr ) {
				pointer = ptr;
			}

			__forceinline  T* xor_str( ) const {
				return  pointer;
			}

			__forceinline  T* operator-> ( ) {
				return xor_str( );
			}

			__forceinline bool is_valid( ) const {
				return pointer != nullptr;
			}
		};

		class non_copyable {
		protected:
			non_copyable( ) { }
			~non_copyable( ) { }

		private:
			non_copyable( const non_copyable& ) = delete;
			non_copyable& operator=( const non_copyable& ) = delete;

			non_copyable( non_copyable&& ) = delete;
			non_copyable& operator=( non_copyable&& ) = delete;
		};
	}
}