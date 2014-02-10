#include "testnarl.h"

#include <iterable_range.h>
#include <cycling_range.h>
#include <partial_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Cycling range with taker is evaluated lazily", "[narl][cycling_range][deferred]" )
{
	auto r = make_test_range< cycling_range >( throwing_range< int >{} );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( r && true, range_access_exception );
}


TEST_CASE( "Cycling range restarts input range at the beginning", "[narl][cycling_range][cycle]" )
{
	auto r = make_test_range< cycling_range >( from( { 1, 2 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE_FALSE( !r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
}


