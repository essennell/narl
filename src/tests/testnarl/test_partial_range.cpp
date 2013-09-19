#include "testnarl.h"

#include <narl.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Partial range with taker is evaluated lazily", "[narl][partial_range][taker][deferred]" )
{
	auto r = make_test_range< partial_range >( throwing_range< int >{}, taker( 2 ) );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Partial range with skipper is evaluated lazily", "[narl][partial_range][skipper][deferred]" )
{
	auto r = make_test_range< partial_range >( throwing_range< int >{}, skipper( 2 ) );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Partial range with taker produces that count of elements", "[narl][partial_range][taker]" )
{
	auto r = make_test_range< partial_range >( from( { 1, 2, 3 } ), taker( 2 ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Partial range with taker is invalid when shorter range is ended", "[narl][partial_range][taker][shorter]" )
{
	auto r = make_test_range< partial_range >( from( { 1, 2 } ), taker( 3 ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Partial range with skipper begins after that count of elements", "[narl][partial_range][skipper]" )
{
	auto r = make_test_range< partial_range >( from( { 1, 2, 3, 4 } ), skipper( 2 ) );

	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Partial range with skipper is invalid when range is shorter", "[narl][partial_range][skipper][shorter]" )
{
	auto r = make_test_range< partial_range >( from( { 1 } ), skipper( 2 ) );
	REQUIRE( !r );
}


TEST_CASE( "Partial range with skipper is valid if there are range items remaining", "[narl][partial_range][skipper][valid]" )
{
	auto r = make_test_range< partial_range >( from( { 1, 2, 3, 4 } ), skipper( 2 ) );
	REQUIRE( !!r );
}


TEST_CASE( "Partial range with taker is valid if there are range items remaining", "[narl][partial_range][taker][valid]" )
{
	auto r = make_test_range< partial_range >( from( { 1, 2, 3, 4 } ), taker( 2 ) );
	REQUIRE( !!r );
}


TEST_CASE( "Partial range with skipping_while is evaluated lazily", "[narl][partial_range][skip_while]][deferred]" )
{
	auto r = make_test_range< partial_range >( throwing_range< int >{}, make_skipping_while( []( int ) { return true; } ) );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Partial range with skipping_while is immediately invalid if underlying range is invalid", "[narl][partial_range][skip_while][invalid]" )
{
	auto src = from( { 1 } );
	auto r = make_test_range< partial_range >( src, make_skipping_while( []( int ) { return true; } ) );
	REQUIRE( !r );
}


TEST_CASE( "Partial range with skipping while is valid if underlying range is valid", "[narl][partial_range][skip_while][valid]" )
{
	auto r = make_test_range< partial_range >( from( { 1, 2, 3 } ), make_skipping_while( []( int i ) { return i == 1; } ) );

	REQUIRE( !!r );
}


TEST_CASE( "Partial range with skipping while skips until expression is false", "[narl][partial_range][skip_while][untilfalse]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< partial_range >( src, make_skipping_while( []( int i ) { return i < 2; } ) );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Partial range with skipping while continues even if expression becomes true", "[narl][skipping_range][continue]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< partial_range >( src, make_skipping_while( []( int i ) { return i == 1 || i == 3; } ) );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Partial range with taking while is evaluated lazily", "[narl][partial_range][take_while][deferred]" )
{
	auto r = make_test_range< partial_range >( throwing_range< int >{}, make_taking_while( []( int ) { return true; } ) );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Partial range with taking while is valid if underlying range is valid", "[narl][partial_range][take_while][valid]" )
{
	auto r = make_test_range< partial_range >( from( { 1, 2 } ), make_taking_while( []( int i ) { return i < 3; } ) );

	REQUIRE( !!r );
}


TEST_CASE( "Partial range with taking while becomes invalid if underlying range is invalid", "[narl][partial_range][take_while][invalid]" )
{
	auto src = from( { 1 } );
	auto r = make_test_range< partial_range >( src, make_taking_while( []( int ) { return true; } ) );
	REQUIRE( !++r );
}


TEST_CASE( "Partial range with taking while takes until expression is false", "[narl][partial_range][take_while][untilfalse]" )
{
	auto src = from( { 1, 2, 3 } );
	auto r = make_test_range< partial_range >( src, make_taking_while( []( int i ) { return i < 3; } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Partial range with taking while never becomes valid after stopping", "[narl][partial_range][take_while][done_is_done]" )
{
	auto r = make_test_range< partial_range >( from( { 1, 2, 3 } ), make_taking_while( []( int i ) { return i % 2 != 0; } ) );
	REQUIRE( !!r++ );
	REQUIRE( !r++ );
	REQUIRE( !r++ );
	REQUIRE( !r++ );
}



