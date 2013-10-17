#include "testnarl.h"

#include <iterable_range.h>
#include <range_tocontainer.h>

#include <catch.hpp>

#include <array>
#include <list>
#include <vector>


using namespace narl;


TEST_CASE( "To container range creates a vector", "[narl][to_container][vector]" )
{
	auto r = from( { 1, 2, 3 } );
	auto c = to_container< std::vector >()( r );

	for( auto i : c )
	{
		REQUIRE( i == *r++ );
	}
	REQUIRE( !r );
}


TEST_CASE( "To container range creates a list", "[narl][to_container][list]" )
{
	auto r = from( { 1, 2, 3 } );
	auto c = to_container< std::list >()( r );

	for( auto i : c )
	{
		REQUIRE( i == *r++ );
	}
	REQUIRE( !r );
}


