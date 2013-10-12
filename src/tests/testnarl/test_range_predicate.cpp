#include "testnarl.h"

#include <iterable_range.h>
#include <range_predicate.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Range validator is true if underlying object is true", "[narl][range_validator]" )
{
	auto src = from( { 1 } );
	auto r = make_test_range< range_validator >( src );

	REQUIRE( r.value() );
}


TEST_CASE( "Range validator is false if underlying range is invalid", "[narl][range_validator][invalid]" )
{
	auto src = from( { 1 } );
	++src;
	auto r = make_test_range< range_validator >( src );

	REQUIRE( !r.value() );
}


TEST_CASE( "Range predicate is true if filter of underlying range is true", "[narl][range_predicate]" )
{
	auto src = from( { 1 } );
	auto r = make_test_range< range_predicate >( src, []( int ) { return true; } );

	REQUIRE( !!r.value() );
}


TEST_CASE( "Range predicate is false if filter of underlying range is false", "[narl][range_predicate][invalid]" )
{
	auto src = from( { 1 } );
	auto r = make_test_range< range_predicate >( src, []( int ) { return false; } );

	REQUIRE( !r.value() );
}


TEST_CASE( "Inverter predicate is true if filter of underlying range is true", "[narl][inverter]" )
{
	auto src = from( { 1 } );
	auto r = make_test_range< range_predicate_inverter >( src, []( int ) { return true; } );

	REQUIRE( !!r.value() );
}


TEST_CASE( "Inverter predicate is false if filter of underlying range is false", "[narl][inverter][invalid]" )
{
	auto src = from( { 1 } );
	auto r = make_test_range< range_predicate_inverter >( src, []( int ) { return false; } );

	REQUIRE( !r.value() );
}



