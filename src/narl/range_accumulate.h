#pragma once

#include "range_factory.h"

#include <stdexcept>


namespace narl
{

	template< typename range_type, typename seed, typename accumulator >
	class range_accumulate
	{

		private:
			mutable range_type r;
			seed initial;
			accumulator a;


		public:
			range_accumulate( const range_type & r, const seed & initial, const accumulator & a )
				: r{ r }, initial{ initial }, a( a )
			{
			}


			auto value() const -> decltype( a( initial, *r ) )
			{
				if( ! r )
					throw std::invalid_argument( "Cannot accumulate over an empty range" );
				
				auto current = a( initial, *r++ );
				while( r )
					current = a( current, *r++ );
				return current;
			}

	};

	template< typename range_type, typename accumulator >
	class range_default_accumulate
	{

		private:
			mutable range_type r;
			accumulator a;


		public:
			range_default_accumulate( const range_type & r, const accumulator & a )
				: r{ r }, a( a )
			{
			}


			auto value() const -> decltype( a( *r, *r ) )
			{
				if( !r )
					throw std::invalid_argument( "Cannot accumulate over an empty range" );

				auto current = *r++;
				while( r )
					current = a( current, *r++ );
				return current;
			}

	};


	template< typename range_type >
	class range_counter
	{

		private:
			mutable range_type r;


		public:
			range_counter( const range_type & r )
				: r{ r }
			{
			}


			auto value() const -> unsigned
			{
				unsigned c = 0;
				if( r )
					while( r++ )
						++c;
				return c;
			}

	};


	template< typename seed, typename accumulator >
	auto aggregate( const seed & initial, const accumulator & expr ) -> decltype( make_factory< range_accumulate >( initial, expr ) )
	{
		return make_factory< range_accumulate >( initial, expr );
	}

	template< typename accumulator >
	auto aggregate( const accumulator & expr ) -> decltype( make_factory< range_default_accumulate >( expr ) )
	{
		return make_factory< range_default_accumulate >( expr );
	}

	inline auto count() -> decltype( make_factory< range_counter >() )
	{
		return make_factory< range_counter >();
	}

}
