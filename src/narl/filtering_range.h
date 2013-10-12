#pragma once

#include "range_factory.h"


namespace narl
{

	template< typename range_type, typename predicate >
	class filtering_range
	{

		private:
			mutable range_type r;
			predicate filter;

			void find_next_match() const
			{
				while( r && !filter( *r ) )
					++r;
			}

			void find_prev_match() const
			{
				while( r && !filter( *r ) )
					--r;
			}

		public:
			filtering_range( const range_type & r, const predicate & filter )
				: r{ r }, filter( filter )
			{
			}

			
			auto operator*() const -> decltype( *r )
			{
				find_next_match();
				return *r;
			}

			auto operator++() -> filtering_range &
			{
				++r;
				find_next_match();
				return *this;
			}

			auto operator++( int ) -> filtering_range
			{
				find_next_match();
				filtering_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> filtering_range &
			{
				--r;
				find_prev_match();
				return *this;
			}

			auto operator--( int ) -> filtering_range
			{
				find_prev_match();
				filtering_range tmp{ *this };
				--*this;
				return tmp;
			}

			void goto_end()
			{
				r.goto_end();
			}

			explicit operator bool() const
			{
				find_next_match();
				return !!r;
			}

	};

	
	template< typename expression >
	auto where( const expression & expr ) -> decltype( make_factory< filtering_range >( expr ) )
	{
		return make_factory< filtering_range >( expr );
	}

}
