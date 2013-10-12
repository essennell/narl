#pragma once

#include "range_factory.h"

#include <iterator>
#include <list>
#include <memory>


namespace narl
{

	template< typename range_type >
	class reversed_range;

	template<>
	class range_0_factory< reversed_range >
	{

		public:

			template< typename other_range_type >
			auto operator()( const other_range_type & r ) const -> reversed_range< other_range_type >
			{
				return reversed_range< other_range_type >{ r };
			}

			template< typename other_range_type >
			auto operator()( const reversed_range< other_range_type > & r ) const -> other_range_type
			{
				return r.r;
			}
	};


	template< typename range_type >
	class reversed_range
	{
		
		friend class range_0_factory< ::narl::reversed_range >;

		private:
			range_type r;
			mutable range_type retro;
			mutable bool reversed;

			void do_reverse() const
			{
				if( ! reversed )
				{
					retro.goto_end();
					--retro;
					reversed = true;
				}
			}


		public:
			reversed_range( const range_type & r )
				: r{ r }, retro{ r }, reversed{ false }
			{
			}

			auto operator*() const -> decltype( *r )
			{
				do_reverse();
				return *retro;
			}

			auto operator++() -> reversed_range &
			{
				do_reverse();
				--retro;
				return *this;
			}

			auto operator++( int ) -> reversed_range
			{
				do_reverse();
				reversed_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> reversed_range &
			{
				do_reverse();
				++retro;
				return *this;
			}

			auto operator--( int ) -> reversed_range
			{
				reversed_range tmp{ *this };
				do_reverse();
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				do_reverse();
				return !!retro;
			}

			void goto_end()
			{
				r.goto_end();
			}

	};


	inline auto reverse() -> decltype( make_factory< reversed_range >() )
	{
		return make_factory< reversed_range >();
	}

}
