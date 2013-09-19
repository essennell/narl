#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "throwing_range.h"

#include <narl.h>

#include <algorithm>
#include <array>
#include <iterator>
#include <iostream>
#include <stdexcept>
#include <type_traits>
#include <vector>


using namespace narl;


TEST_CASE( "Range is created from vector source", "[narl][create][vector]" )
{
	std::vector< int > src { 1, 2, 3 };
	auto r = from( src );

	REQUIRE( ( std::is_same< int, decltype( *r ) >() ) );
	REQUIRE( !!r );
}


TEST_CASE( "Range is created from built in array source", "[narl][create][builtinarray]" )
{
	int src[] = { 1, 2, 3 };
	auto r = from( src );

	REQUIRE( ( std::is_same< int, decltype( *r ) >() ) );
	REQUIRE( !!r );
}


TEST_CASE( "Range is created from static array source", "[narl][create][staticarray]" )
{
	std::array< int, 3 > src { 1, 2, 3 };
	auto r = from( src );

	REQUIRE( ( std::is_same< int, decltype( *r ) >() ) );
	REQUIRE( !!r );
}


TEST_CASE( "Range is created from initializer list source", "[narl][create][initializer_list]" )
{
	auto r = from( { 1, 2, 3 } );

	REQUIRE( ( std::is_same< int, decltype( *r ) >() ) );
	REQUIRE( !!r );
}


TEST_CASE( "Range created from single element initializer list has one element", "[narl][create][initializer_list][single]" )
{
	auto r = from( { 1 } );
	REQUIRE( !!r++ );
	REQUIRE( !r );
}


TEST_CASE( "Range created from empty source is immediately invalid", "[narl][create][empty][invalid]" )
{
	std::vector< int > src;
	auto r = from( src );

	REQUIRE( !r );
}


TEST_CASE( "Range can be incremented to yield next value", "[narl][increment]" )
{
	auto r = from( { 1, 2 } );
	
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
}


TEST_CASE( "Range can be pre-incremented to yield next value", "[narl][preincrement]" )
{
	auto r = from( { 1, 2 } );

	++r;
	REQUIRE( *r == 2 );
}


TEST_CASE( "Range is invalid after incrementing past last element", "[narl][increment][invalid]" )
{
	auto r = from( { 1 } );

	REQUIRE( !!r );
	++r;
	REQUIRE( !r );
}


TEST_CASE( "Dereferencing an invalid range produces error", "[narl][dereference][invalid][error]" )
{
	auto r = from( { 1 } );
	++r;

	REQUIRE_THROWS_AS( *r, std::out_of_range );
}


TEST_CASE( "Range operates with range-based for", "[narl][range_for]" )
{
	std::vector< int > src { 1, 2, 3 };
	auto r = from( src );

	int index = 0;
	for( auto i : r )
	{
		REQUIRE( i == src[ index++ ] );
	}
}


template< typename R >
struct dummy_range_1
{
	dummy_range_1( R r, int v ) : r{ r }, t{ v } { }
	R r;
	int t;
};

template< typename R, typename T >
struct dummy_range_2
{
	dummy_range_2( R r, T t ) : r{ r }, t{ t } { }
	R r;
	T t;
};

auto make_dummy_1( int v ) -> range_1_factory< dummy_range_1, int >
{
	return range_1_factory< dummy_range_1, int >{ v };
}

auto make_dummy_2( int v ) -> range_2_factory< dummy_range_2, int >
{
	return range_2_factory< dummy_range_2, int >{ v };
}

TEST_CASE( "Ranges are connected together with unary factory", "[narl][connect][factory][unary]" )
{
	auto r = from( { 1 } ) | make_dummy_1( 2 );

	REQUIRE( *( r.r ) == 1 );
	REQUIRE( r.t == 2 );
}


TEST_CASE( "Ranges are connected together with binary factory", "[narl][connect][factory][binary]" )
{
	auto r = from( { 1 } ) | make_dummy_2( 2 );

	REQUIRE( *( r.r ) == 1 );
	REQUIRE( r.t == 2 );
}


TEST_CASE( "Range generated produces an incrementing range for int", "[narl][generate]" )
{
	auto r = make_range< int >();

	REQUIRE( *r++ == 0 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );

	REQUIRE( !!r );
}


TEST_CASE( "Skip ignores specified count of elements", "[narl][skip]" )
{
	auto r = make_range< int >() | skip( 3 );

	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 5 );

	REQUIRE( !!r );
}


TEST_CASE( "Take contains the specified count of elements", "[narl][take]" )
{
	auto r = make_range< int >() | take( 3 );

	REQUIRE( *r++ == 0 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );

	REQUIRE( !r );
}


TEST_CASE( "Skipwhile ignores elements matching the expression", "[narl][skip][while]" )
{
	auto r = make_range< int >() | skip_while( []( int i ) { return i < 5; } );
	
	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 6 );
	REQUIRE( *r++ == 7 );
}


TEST_CASE( "Takewhile contains elements until the expression is false", "[narl][take][while]" )
{
	auto r = make_range< int >() | take_while( []( int i ) { return i < 3; } );

	REQUIRE( *r++ == 0 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );

	REQUIRE( !r );
}


TEST_CASE( "Select produces required elements and leaves original range intact", "[narl][select]" )
{
	std::vector< int > src { 1, 2, 3 };
	std::vector< int > original { src };
	std::vector< int > expected;

	std::transform( std::begin( src ), std::end( src ), std::back_inserter( expected ), []( int x ) { return x * 10; } );
	
	auto r = from( src ) | select( []( int x ) { return x * 10; } );
	
	int index = 0;
	for( auto i : original )
	{
		REQUIRE( !!r );
		REQUIRE( src[ index ] == i );
		REQUIRE( expected[ index ] == *r++ );
		++index;
	}
}


TEST_CASE( "Select doesn't evaluate the range", "[narl][select][deferred]" )
{
	auto r = throwing_range< int >{ } | select( []( int i ) { return i * 10; } );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Where produces only elements that match the condition", "[narl][where]" )
{
	auto r = from( { 1, 2, 3, 4, 5 } ) | where( []( int i ) { return i % 2 == 0; } );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}



