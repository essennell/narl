#pragma once

#include "range_factory.h"


namespace narl
{

	template< typename range_type >
	class cycling_range
	{

		private:
			range_type r, orig;

		public:
			cycling_range( range_type r )
				: r{ r }, orig{ r }
			{
			}


			auto operator*() const -> decltype( *r )
			{
				return *r;
			}

			auto operator++() -> cycling_range &
			{
				if( !++r )
					r = orig;
				return *this;
			}

			auto operator++( int ) -> cycling_range
			{
				cycling_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> cycling_range &
			{
				if( !--r )
					goto_end();
				return *this;
			}

			auto operator--( int ) -> cycling_range
			{
				cycling_range tmp{ *this };
				--*this;
				return tmp;
			}
			
			void goto_end()
			{
				r.goto_end();
			}

			explicit operator bool() const
			{
				return r || orig;
			}

	};

	inline auto cycle() -> decltype( make_factory< cycling_range >() )
	{
		return make_factory< cycling_range >();
	}

}
