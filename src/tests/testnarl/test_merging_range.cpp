#include "testnarl.h"

#include <iterable_range.h>
#include <merging_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Merging range is lazy evaluated", "[narl][merging_range][lazy]" )
{
	auto r = make_test_range< merging_range >( throwing_range< int >(), from( { 1, 2, 4 } ), std::less< int >() );

	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( !r, range_access_exception );
	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( --r, range_access_exception );
}


TEST_CASE( "Merging range with empty is same as first range", "[narl][merging_range][empty][left]" )
{
	std::vector< int > empty;
	auto r = make_test_range< merging_range_default >( from( { 1, 2 } ), from( empty ) );

	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Merging empty with range is same as second range", "[narl][merging_range][empty][right]" )
{
	std::vector< int > empty;
	auto r = make_test_range< merging_range_default >( from( empty ), from( { 1, 2 } ) );

	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Merging duplicate ranges has every item", "[narl][merging_range][same]" )
{
	auto r = make_test_range< merging_range_default >( from( { 1, 3 } ), from( { 1, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Merging contains all items from both ranges", "[narl][merging_range][all]" )
{
	auto r = make_test_range< merging_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Merging contains all items from both ranges with preincrement", "[narl][merging_range][all][preincrement]" )
{
	auto r = make_test_range< merging_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );

	REQUIRE( *r == 1 );
	REQUIRE( *++r == 2 );
	REQUIRE( *++r == 3 );
	REQUIRE( *++r == 4 );
	REQUIRE( !++r );
}


TEST_CASE( "Merging range contains all items from both ranges when first element is on the right", "[narl][merging_range][all][firstright]" )
{
	auto r = make_test_range< merging_range_default >( from( { 2, 4 } ), from( { 1, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Merging contains all items when left is longer", "[narl][merging_range][leftlonger]" )
{
	auto r = make_test_range< merging_range_default >( from( { 0, 2, 4, 6 } ), from( { 1, 2, 3 } ) );

	REQUIRE( *r++ == 0 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 6 );
	REQUIRE( !r );
}


TEST_CASE( "Merging range can be moved to end and decremented to get final element", "[narl][merging_range][gotoend][decrement]" )
{
	auto r = make_test_range< merging_range_default >( from( { 2, 4 } ), from( { 1, 3 } ) ) ;
	r.goto_end();

	REQUIRE( !r );
	--r;
	REQUIRE( !!r );
	REQUIRE( *r == 4 );
}


TEST_CASE( "Merging range can be moved to end and decremented to get final element on the right", "[narl][merging_range][gotoend][decrement][right]" )
{
	auto r = make_test_range< merging_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) ) ;
	r.goto_end();

	REQUIRE( !r );
	--r;
	REQUIRE( !!r );
	REQUIRE( *r == 4 );
}


TEST_CASE( "Merging range finds all elements in reverse with predecrement", "[narl][merging_range][predecrement]" )
{
	auto r = make_test_range< merging_range_default >( from( { 0, 2, 4, 6 } ), from( { 1, 2, 3 } ) );
	r.goto_end();

	REQUIRE( *--r == 6 );
	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( *--r == 0 );
	REQUIRE( !--r );
}


TEST_CASE( "Merging range can be incremented from before begin to get first element", "[narl][merging_range][beforebegin][increment]" )
{
	auto r = make_test_range< merging_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );
	--r;
	
	REQUIRE( !r++ );
	REQUIRE( !!r );
	REQUIRE( *r == 1 );
}


TEST_CASE( "Merging range can be incremented from before begin to get first element on right", "[narl][merging_range][beforebegin][increment][right]" )
{
	auto r = make_test_range< merging_range_default >( from( { 2, 4 } ), from( { 1, 3 } ) );
	--r;
	
	REQUIRE( !r++ );
	REQUIRE( !!r );
	REQUIRE( *r == 1 );
}


TEST_CASE( "Merging range can use custom comparer", "[narl][merging_range][customcmp]" )
{
	auto r = make_test_range< merging_range >( from( { 3, 1 } ), from( { 4, 2 } ), []( int l, int r ) { return l > r; } );

	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}


