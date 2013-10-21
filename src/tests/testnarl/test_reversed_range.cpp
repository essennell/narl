#include "testnarl.h"

#include <iterable_range.h>
#include <range_generator.h>
#include <range_input_iterator.h>
#include <reversed_range.h>
#include <transforming_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Reversed range is evaluated lazily", "[narl][reversed_range][deferred]" )
{
	auto r = make_test_range< reversed_range >( throwing_range< int >{} );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( r && true, range_access_exception );
}


TEST_CASE( "Reversed range produces elements in reverse order", "[narl][reversed_range][reversed]" )
{
	auto r = make_test_range< reversed_range >( from( { 2, 1, 0 } ) );

	REQUIRE( *r++ == 0 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Reversed range of single element procuces that element", "[narl][reversed_range][single]" )
{
	auto r = make_test_range< reversed_range >( from( { 1 } ) );

	REQUIRE( *r == 1 );
}


TEST_CASE( "Reversed range is invalid after incrementing past the end", "[narl][reversed_range][pasttheend]" )
{
	auto r = make_test_range< reversed_range >( from( { 1 } ) );
	++r;

	REQUIRE( !r );
}


TEST_CASE( "Reversed range can be decremented to produce previous value", "[narl][reversed_range][decrement]" )
{
	auto r = make_test_range< reversed_range >( from( { 1, 2, 3 } ) );
	++r;

	REQUIRE( *r == 2 );
	--r;
	REQUIRE( *r == 3 );
}



