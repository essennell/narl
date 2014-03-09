#pragma once

#include "range_factory.h"

#include <tuple>


namespace narl
{

	template< typename range_type, typename other_range_type >
	class zipping_range
	{
		
		private:
			range_type r;
			other_range_type other;


		public:
			zipping_range( range_type r, other_range_type other )
				: r{ r }, other{ other }
			{
			}


			auto operator*() const -> decltype( std::make_tuple( *r, *other ) )
			{
				return std::make_tuple( *r, *other );
			}

			auto operator++() -> zipping_range &
			{
				++r;
				++other;
				return *this;
			}
			
			auto operator++( int ) -> zipping_range
			{
				zipping_range tmp{ *this };
				++*this;
				return tmp;
			}
			
			auto operator--() -> zipping_range &
			{
				--r;
				--other;
				return *this;
			}
			
			auto operator--( int ) -> zipping_range
			{
				zipping_range tmp{ *this };
				--*this;
				return tmp;
			}
			
			explicit operator bool() const
			{
				return !!r && !!other;
			}

			void goto_end()
			{
				r.goto_end();
				other.goto_end();
			}

	};


	template< typename range_type >
	auto zipwith( range_type r ) -> decltype( make_factory< zipping_range >( r ) )
	{
		return make_factory< zipping_range >( r );
	}

}


