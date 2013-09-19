#pragma once

#include "iterable_range.h"
#include "filtering_range.h"
#include "partial_range.h"
#include "range_factory.h"
#include "range_input_iterator.h"
#include "range_generator.h"
#include "transforming_range.h"


namespace narl
{

	template< typename container >
	auto from( const container & src ) 
		-> iterable_range< typename container::const_iterator, typename container::value_type >
	{
		return iterable_range< typename container::const_iterator, typename container::value_type >
			{ std::begin( src ), std::end( src ) };
	}

	template< typename value_type, size_t len >
	auto from( value_type( &array )[ len ] ) 
		-> iterable_range< const value_type*, value_type >
	{
		return iterable_range< const value_type*, value_type >
			{ array, array + len };
	}

	template< typename item_type >
	auto from( std::initializer_list< item_type > src ) 
		-> iterable_range< typename std::initializer_list< item_type >::const_iterator, item_type >
	{
		return iterable_range< typename std::initializer_list< item_type >::const_iterator, item_type >
			{ std::begin( src ), std::end( src ) };
	}

	template< typename item_type >
	auto make_range() -> range_generator< item_type >
	{
		return range_generator< item_type >{ };
	}


	template< typename expression >
	auto select( expression expr ) -> decltype( make_factory< transforming_range >( expr ) )
	{
		return make_factory< transforming_range >( expr );
	}

	
	template< typename expression >
	auto where( expression expr ) -> decltype( make_factory< filtering_range >( expr ) )
	{
		return make_factory< filtering_range >( expr );
	}


	inline auto skip( unsigned count ) -> decltype( make_factory< partial_range >( skipper( count ) ) )
	{
		return make_factory< partial_range >( skipper( count ) );
	}

	inline auto take( unsigned count ) -> decltype( make_factory< partial_range >( taker( count ) ) )
	{
		return make_factory< partial_range >( taker( count ) );
	}


	template< typename expression >
	auto skip_while( expression expr ) -> decltype( make_factory< partial_range >( make_skipping_while( expr ) ) )
	{
		return make_factory< partial_range >( make_skipping_while( expr ) );
	}

	template< typename expression >
	auto take_while( expression expr ) -> decltype( make_factory< partial_range >( make_taking_while( expr ) ) )
	{
		return make_factory< partial_range >( make_taking_while( expr ) );
	}


	template< typename range_type, typename factory_type >
	auto operator|( range_type r, factory_type factory )
		-> decltype( factory( r ) )
	{
		return factory( r );
	}

}


