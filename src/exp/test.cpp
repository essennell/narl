#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "iterable_range.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


using namespace xri;

//std::string to_string( int i ) { std::stringstream strm; strm << i; return strm.str(); }


TEST_CASE( "Range is constructable from standard collection and iterable", "[range][init][iterable][stl]" )
{
	std::vector< int > data { 1, 2, 3 };
	auto range = make_range( data );

	REQUIRE( *range++ == data[ 0 ] );
	REQUIRE( *range++ == data[ 1 ] );
	REQUIRE( *range++ == data[ 2 ] );
}

TEST_CASE( "Transformation of elements results in new range leaving originals intact", "[range][transform]" )
{
	std::vector< int > data { 1, 2, 3 };
	auto range = make_range( data );

	using namespace std;
	auto result = select( range, []( int i ) { return to_string( i ); } );

	std::string expected[] = { "1", "2", "3" };

	REQUIRE( *result++ == expected[ 0 ] );
	REQUIRE( *result++ == expected[ 1 ] );
	REQUIRE( *result++ == expected[ 2 ] );

	REQUIRE( *range++ == data[ 0 ] );
	REQUIRE( *range++ == data[ 1 ] );
	REQUIRE( *range++ == data[ 2 ] );
}

TEST_CASE( "Filtering elements contains just the matches", "[range][filter]" )
{
	std::vector< int > data { 1, 2, 3 };
	auto range = make_range( data );

	auto result = where( range, []( int i ) { return i % 2 != 0; } );

	REQUIRE( *result++ == data[ 0 ] );
	REQUIRE( *result++ == data[ 2 ] );
}

TEST_CASE( "Filtering range returns correct matches when first element is a mismatch", "[range][filter][empty]" )
{
	std::vector< int > data { 2, 3, 4 };
	auto range = make_range( data );

	auto result = where( range, []( int i ) { return i % 2 != 0; } );

	REQUIRE( !!result );
	REQUIRE( *result++ == data[ 1 ] );
}

TEST_CASE( "Filtering range is immediately invalid when no elements match", "[range][filter][nomatch]" )
{
	std::vector< int > data { 2, 3, 4 };
	auto range = make_range( data );

	auto result = where( range, []( int ) { return false; } );

	REQUIRE( ! result );
}

TEST_CASE( "Take returns a range with the specified number of elements", "[range][take][inline]" )
{
	std::vector< int > data { 1, 2, 4, 8, 16 };
	auto range = make_range( data );

	auto result = take( range, 1 );

	REQUIRE( *result++ == data[ 0 ] );
	REQUIRE( !result );
}

TEST_CASE( "Ranges can be composed using the overloaded operator", "[range][composition][operator]" )
{
	std::vector< int > data { 1, 2, 3 };
	auto range = make_range( data );

	auto result =  range | where( []( int i ) { return i % 2 != 0; } )
						 | select( []( int i ) { return i * 10; } );

	REQUIRE( *result++ == data[ 0 ] * 10 );
	REQUIRE( *result++ == data[ 2 ] * 10 );
	REQUIRE( ! result );
}

TEST_CASE( "Range can be limited to a number of elements with take without invalidating original range", "[range][take]" )
{
	std::vector< int > data { 1, 2, 3, 4, 5 };
	auto range = make_range( data );

	auto result = range | take( 3 );

	REQUIRE( *result++ == data[ 0 ] );
	REQUIRE( *result++ == data[ 1 ] );
	REQUIRE( *result++ == data[ 2 ] );
	REQUIRE( !result );

	REQUIRE( *range == data[ 0 ] );
}


TEST_CASE( "Range can be used to populate a standard lib container", "[range][export][stl]" )
{
	using namespace std;

	vector< int > data { 1, 2, 3 };
	auto range = make_range( data ) | select( []( int x ) { return to_string( x ); } );

	vector< string > result { begin( range ), end( range ) };

	REQUIRE( result.size() == data.size() );
	REQUIRE( to_string( data[ 0 ] ) == result[ 0 ] );
	REQUIRE( to_string( data[ 1 ] ) == result[ 1 ] );
	REQUIRE( to_string( data[ 2 ] ) == result[ 2 ] );
}


