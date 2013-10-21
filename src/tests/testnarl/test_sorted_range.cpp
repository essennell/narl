#include "testnarl.h"

#include <iterable_range.h>
#include <partial_range.h>
#include <range_generator.h>
#include <sorted_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Sorted range is lazily evaluated", "[narl][sorted][deferred]" )
{
	auto r = make_test_range< sorted_range >( throwing_range< int >{}, []( int, int ) { return true; } );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( r && true, range_access_exception );
}


TEST_CASE( "Sorted range is valid if underlying range is valid", "[narl][sorted][valid]" )
{
	auto r = make_test_range< sorted_range >( from( { 1 } ), []( int, int ) { return true; } );
	REQUIRE( !!r );
}


TEST_CASE( "Sorted range is invalid if underlying range is invalid", "[narl][sorted][invalid]" )
{
	auto r = make_test_range< sorted_range >( from( { 1 } ), []( int, int ) { return true; } );
	REQUIRE( !++r );
}


TEST_CASE( "Sorted range uses minimum calls to comparer", "[narl][sorted][efficient]" )
{
	int count = 0;
	int N = 100000;
	auto src = make_test_range< partial_range >( make_range< int >(), taker( N ) );

	auto r = make_test_range< sorted_range >( src, [&]( int l, int r ) { return ++count, r < l; } );

	REQUIRE( !!r );
	REQUIRE( *r == N-1 );

#if defined _MSC_VER && defined _DEBUG
	REQUIRE( count < ( N * ( std::log( N ) ) ) * 3 );
#else
	REQUIRE( count < N * ( std::log( N ) ) );
#endif
}


TEST_CASE( "Sorted range can be decremented to yield previous element", "[narl][sorted][decrement]" )
{
	auto r = make_test_range< sorted_range >( from( { 2, 1, 0 } ), []( int l, int r ) { return l < r; } );
	++r;
	REQUIRE( *r-- == 1 );
	REQUIRE( *r-- == 0 );
	REQUIRE( !r );
}


TEST_CASE( "Sorted range can be pre-decremented to yield previous element", "[narl][sorted][predecrement]" )
{
	auto r = make_test_range< sorted_range >( from( { 2, 1, 0 } ), []( int l, int r ) { return l < r; } );
	++r;
	REQUIRE( *r == 1 );
	--r;
	REQUIRE( !!r );
	REQUIRE( *r == 0 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Sorted range is invalid when before begin", "[narl][sorted][pastbegin]" )
{
	auto r = make_test_range< sorted_range >( from( { 1 } ), []( int l, int r ) { return l < r; } );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
}


TEST_CASE( "Sorted range can be moved to the end", "[narl][sorted][goto_end]" )
{
	auto r = make_test_range< sorted_range >( from( { 1, 2 } ), []( int l, int r ) { return l < r; } );
	r.goto_end();
	REQUIRE( !r );
}


TEST_CASE( "Sorted range becomes valid after decrement from the end", "[narl][sorted][decrement_from_end]" )
{
	auto r = make_test_range< sorted_range >( from( { 1, 2 } ), []( int l, int r ) { return l < r; } );
	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( !!r );
	REQUIRE( *r == 2 );
}



