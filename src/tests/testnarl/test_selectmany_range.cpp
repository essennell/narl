#include "testnarl.h"

#include <iterable_range.h>
#include <selectmany_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Select many range is lazy evaluated", "[narl][selectmany][lazy]" )
{
	auto r = make_test_range< selectmany_range >( throwing_range< int >(), []( int ) { return from( { 2, 4, 6 } ); } );

	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( --r, range_access_exception );
	REQUIRE_THROWS_AS( r--, range_access_exception );
	REQUIRE_THROWS_AS( !!r, range_access_exception );
}


TEST_CASE( "Select many range produces all elements of inner range", "[narl][selectmany][inner]" )
{
	auto r = make_test_range< selectmany_range >( from( { 1, 2 } ), []( int i ) { return from( { i*2, i*3 } ); } );

	REQUIRE( !!r );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 6 );
	REQUIRE( !r );
}


TEST_CASE( "Select many range can be moved to end and decremented for last element", "[narl][selectmany][end][decrement]" )
{
	auto r = make_test_range< selectmany_range >( from( { 1, 2 } ), []( int i ) { return from( { i*2, i*3 } ); } );
	r.goto_end();
	REQUIRE( !r );
	--r;
	REQUIRE( !!r );
	REQUIRE( *r == 6 );
}


TEST_CASE( "Select many range produces all elements of inner range in reverse", "[narl][selectmany][inner][reverse]" )
{
	auto r = make_test_range< selectmany_range >( from( { 1, 2 } ), []( int i ) { return from( { i*2, i*3 } ); } );
	r.goto_end();
	--r;
	REQUIRE( !!r );
	REQUIRE( *r-- == 6 );
	REQUIRE( *r-- == 4 );
	REQUIRE( *r-- == 3 );
	REQUIRE( *r-- == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Select many range can be incremented from before begin for first element", "[narl][selectmany][beforebegin][increment]" )
{
	auto r = make_test_range< selectmany_range >( 
		from( { 1, 2 } ), 
		[]( int i ) { return from( { i*2, i*3 } ); } );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
	REQUIRE( *r == 2 );
}

