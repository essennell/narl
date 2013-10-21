#include "testnarl.h"

#include <iterable_range.h>
#include <transforming_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Transforming range is evaluated lazily", "[narl][transforming_range][deferred]" )
{
	auto r = make_test_range< transforming_range >( throwing_range< int >{}, []( int ) { return 0; } );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( r && true, range_access_exception );
}


TEST_CASE( "Transforming range dereference leaves original value intact", "[narl][transforming_range][dereference]" )
{
	int src[] = { 1 };
	auto r = make_test_range< transforming_range >( src, []( int i ) { return i * 2; } );

	REQUIRE( *r == 2 );
	REQUIRE( src[ 0 ] == 1 );
}


TEST_CASE( "Incrementing Transforming range leaves underlying range unchanged", "[narl][transforming_range][increment]" )
{
	int src[] = { 5 };
	auto u = from( src );
	auto r = make_test_range< transforming_range >( src, []( int i ) { return i * 10; } );

	REQUIRE( *r++ == 50 );
	REQUIRE( !!u );
	REQUIRE( *u == 5 );
}


TEST_CASE( "Transforming range becomes invalid if incremented past the end", "[narl][transforming_range][invalidation][preinc]" )
{
	auto r = make_test_range< transforming_range >( from( { 1 } ), []( int i ) { return i * 10; } );
	REQUIRE( !!r );
	++r;
	REQUIRE( !r );
}


TEST_CASE( "Transforming range becomes invalid if post-incremented past the end", "[narl][transforming_range][invalidation][postinc]" )
{
	auto r = make_test_range< transforming_range >( from( { 1 } ), []( int i ) { return i * 10; } );
	REQUIRE( !!r );
	r++;
	REQUIRE( !r );
}


TEST_CASE( "Transforming range can be decremented to yield previous value", "[narl][transforming_range][decrement]" )
{
	auto r = make_test_range< transforming_range >( from( { 1, 2 } ), []( int i ) { return i * 10; } );
	++r;
	REQUIRE( *r-- == 20 );
	REQUIRE( *r-- == 10 );
	REQUIRE( !r );
}



TEST_CASE( "Transforming range can be pre-decremented to yield previous value", "[narl][transforming_range][pre-decrement]" )
{
	auto r = make_test_range< transforming_range >( from( { 1, 2 } ), []( int i ) { return i * 10; } );
	++r;
	--r;
	REQUIRE( *r == 10 );
}


TEST_CASE( "Transforming range is invalid when before begin", "[narl][transforming_range][pastbegin]" )
{
	auto r = make_test_range< transforming_range >( from( { 1, 2 } ), []( int i ) { return i * 10; } );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
	REQUIRE( *r == 10 );
}


TEST_CASE( "Transforming range can be moved to the end", "[narl][transforming_range][goto_end]" )
{
	auto r = make_test_range< transforming_range >( from( { 1, 2 } ), []( int i ) { return i * 10; } );
	r.goto_end();
	REQUIRE( !r );
	--r;
	REQUIRE( !!r );
}




