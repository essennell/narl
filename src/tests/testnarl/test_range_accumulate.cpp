#include "testnarl.h"

#include <iterable_range.h>
#include <range_accumulate.h>

#include <catch.hpp>

#include <stdexcept>


using namespace narl;


TEST_CASE( "Range accumulator throws exception when underlying range is empty", "[narl][accumulate]" )
{
	std::vector< int > src;
	auto r = make_test_range< range_accumulate >( from( src ), 1, []( int, int ) { return 0; } );

	REQUIRE_THROWS_AS( r.value(), std::invalid_argument );
}


TEST_CASE( "Range accumulator value is same as seed value when only one item in range", "[narl][accumulate][single]" )
{
	auto r = make_test_range< range_accumulate >( from( { 1 } ), 1, []( int, int i ) { return i; } );
	REQUIRE( r.value() == 1 );
}


TEST_CASE( "Range accumulator applies fn to each item in turn", "[narl][accumulate][eachitem]" )
{
	auto r = make_test_range< range_accumulate >( from( { 1, 2, 3 } ), 1, []( int c, int i ) { return c * i; } );
	REQUIRE( r.value() == 6 );
}


TEST_CASE( "Range default accumulator throws exception when underlying range is empty", "[narl][defaultaccumulate]" )
{
	std::vector< int > src;
	auto r = make_test_range< range_default_accumulate >( from( src ), []( int, int ) { return 0; } );

	REQUIRE_THROWS_AS( r.value(), std::invalid_argument );
}


TEST_CASE( "Range default accumulator uses first element as seed when none provided", "[narl][defaultaccumulate][1stitemseed]" )
{
	auto r = make_test_range< range_default_accumulate >( from( { 2, 3, 4 } ), []( int c, int i ) { return c * i; } );
	REQUIRE( r.value() == 24 );
}


TEST_CASE( "Range default accumulator applies fn to each item in turn", "[narl][defaultaccumulate][eachitem]" )
{
	auto r = make_test_range< range_default_accumulate >( from( { 1, 2, 3 } ), []( int c, int i ) { return c * i; } );
	REQUIRE( r.value() == 6 );
}



