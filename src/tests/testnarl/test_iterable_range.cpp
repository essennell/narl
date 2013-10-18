#include "testnarl.h"

#include <range_predicate.h>

#include <catch.hpp>

#include <array>
#include <list>
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
	std::array< int, 3 > src { { 1, 2, 3 } };
	auto r = from( src );

	REQUIRE( ( std::is_same< int, decltype( *r ) >() ) );
	REQUIRE( !!r );
}


TEST_CASE( "Range is created from named initializer list source", "[narl][create][named][initializer_list]" )
{
	auto data = { 1, 2, 3 };
	auto r = from( data );

	REQUIRE( ( std::is_same< iterable_range< std::initializer_list< int >::const_iterator, int >, decltype( r ) >() ) );
	REQUIRE( ( std::is_same< int, decltype( *r ) >() ) );
	REQUIRE( !!r );
}


TEST_CASE( "Range is created from initializer list source", "[narl][create][initializer_list]" )
{
	auto r = from( { 1, 2, 3 } );

	REQUIRE( ( std::is_same< own_container_range< std::vector, int >, decltype( r ) >() ) );
	REQUIRE( ( std::is_same< int, decltype( *r ) >() ) );
	REQUIRE( !!r );
}


TEST_CASE( "Range is created from initializer list of string source", "[narl][create][initializer_list][string]" )
{
	auto r = from( { std::string( "1" ), std::string( "2" ), std::string( "3" ) } );

	REQUIRE( ( std::is_same< own_container_range< std::vector, std::string >, decltype( r ) >() ) );

	REQUIRE( !!r );
	REQUIRE( ( std::is_same< std::string, decltype( *r ) >() ) );

	REQUIRE( *r++ == "1" );
	REQUIRE( *r++ == "2" );
	REQUIRE( *r++ == "3" );
	REQUIRE( !r );
}


TEST_CASE( "Range is created from external initializer list of string source", "[narl][create][initializer_list][external][string]" )
{
	auto src { std::string( "1" ), std::string( "2" ), std::string( "3" ) };
	auto r = from( src );
	REQUIRE( ( std::is_same< iterable_range< std::initializer_list< std::string >::const_iterator, std::string >, decltype( r ) >() ) );

	REQUIRE( !!r );
	REQUIRE( ( std::is_same< std::string, decltype( *r ) >() ) );

	REQUIRE( *r++ == "1" );
	REQUIRE( *r++ == "2" );
	REQUIRE( *r++ == "3" );
	REQUIRE( !r );
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


TEST_CASE( "Range is created from temporary collection", "[narl][create][temporary][collection]" )
{
	auto r = from( std::list< int > { 1, 2, 3 } );

	REQUIRE( ( std::is_same< own_container_range< std::list, int >, decltype( r ) >() ) );
	REQUIRE( ( std::is_same< int, decltype( *r ) >() ) );
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


TEST_CASE( "Range cen be decremented to yield previous value", "[narl][predecrement]" )
{
	auto r = from( { 1, 2 } );
	++r;
	REQUIRE( *r == 2 );
	--r;
	REQUIRE( *r == 1 );
}


TEST_CASE( "Range can be post-decremented to yield previous value", "[narl][decrement]" )
{
	auto r = from( { 1, 2 } );

	++r;

	REQUIRE( *r-- == 2 );
	REQUIRE( *r == 1 );
}


TEST_CASE( "Range can be decremented to be valid after going past the end", "[narl][decrement][pasttheend]" )
{
	auto r = from( { 1, 2 } );

	++r; ++r;
	REQUIRE( !r );
	--r;
	REQUIRE( !!r );
	REQUIRE( *r == 2 );
}


TEST_CASE( "Range is invalid after going past begin", "[narl][decrement][invalid]" )
{
	auto r = from( { 1 } );
	--r;

	REQUIRE( !r );
}


TEST_CASE( "Range can be incremented to be valid after going past begin", "[narl][increment][pastbegin]" )
{
	auto r = from( { 1 } );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
	REQUIRE( *r == 1 );
}


TEST_CASE( "Range can be moved to the end", "[narl][gotoend][invalid]" )
{
	auto r = from( { 1, 2 } );
	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r == 2 );
}



