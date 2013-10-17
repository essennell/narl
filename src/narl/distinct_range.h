#pragma once

#include "range_factory.h"


namespace narl
{

	template< typename range_type, typename comparer >
	class distinct_range
	{

		private:
			range_type r;
			comparer cmp;


		public:
			distinct_range( const range_type & r, const comparer & cmp )
				: r{ r }, cmp( cmp )
			{
			}


			auto operator*() const -> decltype( *r )
			{
				return *r;
			}

			auto operator++() -> distinct_range &
			{
				if( !r )
					++r;
				else
				{
					auto v = *r;
					while( ++r && cmp( v, *r ) )
						;
				}
				return *this;
			}

			auto operator++( int ) -> distinct_range
			{
				distinct_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> distinct_range &
			{
				if( !r )
					--r;
				else
				{
					auto v = *r;
					while( --r && cmp( *r, v ) )
						;
				}
				return *this;
			}

			auto operator--( int ) -> distinct_range
			{
				distinct_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				return !!r;
			}

			void goto_end()
			{
				r.goto_end();
			}

	};

	template< typename range_type >
	class distinct_range_default : public distinct_range< range_type, std::equal_to< decltype( *std::declval< range_type >() ) > >
	{

		public:
			distinct_range_default( const range_type & r )
				: distinct_range< range_type, std::equal_to< decltype( *r ) > >( r, std::equal_to< decltype( *r ) >() )
			{
			}

	};


	template< typename comparer >
	auto distinct( comparer cmp ) -> decltype( make_factory< distinct_range >( cmp ) )
	{
		return make_factory< distinct_range >( cmp );
	}

	auto inline distinct() -> decltype( make_factory< distinct_range_default >() )
	{
		return make_factory< distinct_range_default >();
	}

}
