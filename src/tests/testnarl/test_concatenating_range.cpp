#include "testnarl.h"

#include <iterable_range.h>
#include <concatenating_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Concatenating range is lazy evaluated", "[narl][concat][lazy]" )
{
	auto r = make_test_range< concatenating_range >( throwing_range< int >(), from( { 3, 4 } ) );

	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( --r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( r--, range_access_exception );
	REQUIRE_THROWS_AS( !r, range_access_exception );
}


TEST_CASE( "Concatenating range produces elements from r before l", "[narl][concat][lthenr]" )
{
	auto r = make_test_range< concatenating_range >( from( { 1, 2 } ), from( { 3, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Concatenating range can be moved to the end and decremented", "[narl][concat][gotoend]" )
{
	auto r = make_test_range< concatenating_range >( from( { 1, 2 } ), from( { 3, 4 } ) );
	r.goto_end();

	REQUIRE( !r );
	--r;
	REQUIRE( !!r );
	REQUIRE( *r == 4 );
}


TEST_CASE( "Concatenating range can be post decremented to yield each element", "[narl][concat][postdecrement]" )
{
	auto r = make_test_range< concatenating_range >( from( { 1, 2 } ), from( { 3, 4 } ) );
	r.goto_end();
	--r;

	REQUIRE( *r-- == 4 );
	REQUIRE( *r-- == 3 );
	REQUIRE( *r-- == 2 );
	REQUIRE( *r-- == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Concatenating range can be incremented from before begin", "[narl][concat][beforebegin]" )
{
	auto r = make_test_range< concatenating_range >( from( { 1, 2 } ), from( { 3, 4 } ) );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}



