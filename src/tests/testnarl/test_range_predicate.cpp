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


TEST_CASE( "Range equality is true for two empty ranges", "[narl][equality][bothempty]" )
{
	std::vector< int > empty;
	auto r = make_test_range< range_equality >( from( empty ), from( empty ), []( int l, int r ) { return l == r; } );

	REQUIRE( !!r.value() );
}


TEST_CASE( "Range equality fails for empty vs nonempty", "[narl][equality][emptyleft]" )
{
	std::vector< int > empty;
	auto r = make_test_range< range_equality >( from( empty ), from( { 1 } ), []( int l, int r ) { return l == r; } );

	REQUIRE( !r.value() );
}


TEST_CASE( "Range equality fails for nonempty vs empty", "[narl][equality][emptyright]" )
{
	std::vector< int > empty;
	auto r = make_test_range< range_equality >( from( { 1 } ), from( empty ), []( int l, int r ) { return l == r; } );

	REQUIRE( !r.value() );
}


TEST_CASE( "Range equality fails when left shorter than right", "[narl][equality][shortleft]" )
{
	auto r = make_test_range< range_equality >( from( { 1 } ), from( { 1, 2 } ), []( int l, int r ) { return l == r; } );

	REQUIRE( !r.value() );
}


TEST_CASE( "Range equality fails when right shorter than left", "[narl][equality][shortright]" )
{
	auto r = make_test_range< range_equality >( from( { 1, 2, 3 } ), from( { 1, 2 } ), []( int l, int r ) { return l == r; } );

	REQUIRE( !r.value() );
}


TEST_CASE( "Range equality fails when first element mismatch", "[narl][equality][mismatch][first]" )
{
	auto r = make_test_range< range_equality >( from( { 1, 2, 3 } ), from( { 0, 1, 2 } ), []( int l, int r ) { return l == r; } );

	REQUIRE( !r.value() );
}


TEST_CASE( "Range equality fails when last element mismatch", "[narl][equality][mismatch][last]" )
{
	auto r = make_test_range< range_equality >( from( { 1, 2, 3 } ), from( { 1, 2, 4 } ), []( int l, int r ) { return l == r; } );

	REQUIRE( !r.value() );
}


TEST_CASE( "Range equality fails when any element mismatch", "[narl][equality][mismatch][any]" )
{
	auto r = make_test_range< range_equality >( from( { 1, 0, 3 } ), from( { 1, 2, 3 } ), []( int l, int r ) { return l == r; } );

	REQUIRE( !r.value() );
}


TEST_CASE( "Range equality succeeds when ranges are the same", "[narl][equality][equal]" )
{
	auto r = make_test_range< range_equality >( from( { 2, 1, 4, 0 } ), from( { 2, 1, 4, 0 } ), []( int l, int r ) { return l == r; } );

	REQUIRE( !!r.value() );
}


TEST_CASE( "Range equality uses the provided comparer", "[narl][equality][customcomparer]" )
{
	auto r = make_test_range< range_equality >( from( { 0 } ), from( { 1 } ), []( int l, int r ) { return l != r; } );

	REQUIRE( !!r.value() );
}


TEST_CASE( "Range equality has a default comparer", "[narl][equality][defaultcomparer]" )
{
	auto r = make_test_range< range_equality_default >( from( { 0, 10, 5 } ), from( { 0, 10, 5 } ) );
	REQUIRE( !!r.value() );
}


