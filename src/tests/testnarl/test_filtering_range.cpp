#include "testnarl.h"

#include <filtering_range.h>
#include <iterable_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Filtering range is evaluated lazily", "[narl][filtering_range][deferred]" )
{
	auto r = make_test_range< filtering_range >( throwing_range< int >{}, []( int ) { return false; } );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Filtering range returns first element if its a match", "[marl][filtering_range][firstismatch]" )
{
	auto src = from( { 1, 2 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i % 2 != 0; } );

	REQUIRE( *r == *src );
}


TEST_CASE( "Filtering range return first matching element when not 1st in range", "[narl][filtering_range][notfirstmatch]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i % 2 == 0; } );

	++src;
	REQUIRE( *r == *src );
}


TEST_CASE( "Filtering range returns match when found in last position", "[narl][filtering_range][lastismatch]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i == 3; } );

	++src; ++src;
	REQUIRE( *r == *src );
}


TEST_CASE( "Filtering range is immediately invalid if no matches", "[narl][filtering_range][nomatch]" )
{
	auto r = make_test_range< filtering_range >( from( { 1 } ), []( int ) { return false; } );

	REQUIRE( !r );
}


TEST_CASE( "Filtering range finds subsequent match in adjacent pos", "[narl][filtering_range][next][adjacent]" )
{
	auto src = from( { 1, 3, 5 } );
	auto r = make_test_range< filtering_range >( src, []( int ) { return true; } );

	REQUIRE( *r == *src );
	++r;
	++src;
	REQUIRE( *r == *src );
}


TEST_CASE( "Filtering range moves to next match", "[narl][filtering_range][next]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i % 2 != 0; } );

	REQUIRE( *r++ == *src );
	++src;
	++src;
	REQUIRE( *r == *src );
}


TEST_CASE( "Filtering range becomes invalid after last match", "[narl][filtering_range][invalid][afterlast]" )
{
	auto src = from( { 1, 2, 3, 5 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i % 2 == 0; } );

	++src;
	REQUIRE( *r++ == *src );
	REQUIRE( !r );
}


TEST_CASE( "Filtering range can be decremented to find previous match", "[narl][filtering_range][decrement]" )
{
	auto src = from( { 1, 2, 3, 4, 5 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i % 2 == 0; } );
	++r; ++r;

	REQUIRE( *r == 4 );
	--r;
	REQUIRE( *r == 2 );
}


TEST_CASE( "Filtering range is invalid when decremented from first match", "[narl][filtering_range][decrement][invalid]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< filtering_range >( src, []( int i ){ return i % 2 == 0; } );
	
	REQUIRE( *r == 2 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Filtering range goes to first match after decrement followed by increment", "[narl][filtering_range][decrementandincrement]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i % 2 == 0; } );
	REQUIRE( *r == 2 );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( *r == 2 );
}


TEST_CASE( "Filtering range goes to last match after increment followed by decrement", "[narl][filtering_range][incrementanddecrement]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i % 2 == 0; } );

	REQUIRE( *r++ == 2 );
	REQUIRE( !r-- );
	REQUIRE( !!r );
	REQUIRE( *r == 2 );
}


TEST_CASE( "Filtering range can be moved to the end", "[narl][filtering_range][goto_end]" )
{
	auto src = from( { 1, 2, 3, 4, 5 } );
	auto r = make_test_range< filtering_range >( src, []( int i ) { return i % 2 == 0; } );
	r.goto_end();

	REQUIRE( !r );
	--r;
	REQUIRE( *r == 4 );
}



