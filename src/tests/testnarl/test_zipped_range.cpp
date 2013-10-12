#include "testnarl.h"

#include <iterable_range.h>
#include <zipping_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Zipping range of 2 is evaluated lazily", "[narl][zipped][lazy][2]" )
{
	auto r = make_test_range< zipping_range >( throwing_range< int >{}, from( { 1 } ) );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Zipping range of 3 is evaluated lazily", "[narl][zipped][lazy][3]" )
{
	auto r = make_test_range< zipping_range >( throwing_range< int >{}, from( { 2 } ), from( { 1 } ) );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Zipping range of 4 is evaluated lazily", "[narl][zipped][lazy][4]" )
{
	auto r = make_test_range< zipping_range >( throwing_range< int >{}, from( { 2 } ), from( { 1 } ), from( { 1 } ) );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ( !r ), range_access_exception );
}


TEST_CASE( "Zipping range of 2 produces tuple of each element in turn", "[narl][zipped][2]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ) );

	REQUIRE( *r++ == std::make_tuple( 1, 10 ) );
	REQUIRE( *r++ == std::make_tuple( 2, 20 ) );
	REQUIRE( *r++ == std::make_tuple( 3, 30 ) );

	REQUIRE( !r );
}


TEST_CASE( "Zipping range of 2 can be decremented", "[narl][zipped][2][decrement]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ) );

	++r;
	REQUIRE( *r-- == std::make_tuple( 2, 20 ) );
	REQUIRE( *r-- == std::make_tuple( 1, 10 ) );

	REQUIRE( !r );
}


TEST_CASE( "Zipping range of 2 can be pre-decremented", "[narl][zipped][2][predecrement]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ) );

	++r;
	--r;
	REQUIRE( *r == std::make_tuple( 1, 10 ) );

	--r;
	REQUIRE( !r );

	++r;
	REQUIRE( !!r );
}


TEST_CASE( "Zipping range of 2 can be moved to end and decremented", "[narl][zipped][2][goto_end][decrement]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ) );

	r.goto_end();
	REQUIRE( !r-- );

	REQUIRE( *r == std::make_tuple( 3, 30 ) );
}


TEST_CASE( "Zipping range of 2 with different lengths is invalid at end of shortest", "[narl][zipped][2][shorteest]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 30 } ) );

	++r;
	++r;
	REQUIRE( !r );
}


TEST_CASE( "Zipping range of 2 with different lengths is invalid at end of shortest in 1st", "[narl][zipped][2][shorteest][first]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 3 } ), from( { 10, 20, 30 } ) );

	++r;
	++r;
	REQUIRE( !r );
}


TEST_CASE( "Zipping range of 3 produces tuple of each element in turn", "[narl][zipped][3]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ), from( { 100, 200, 300 } ) );

	REQUIRE( *r++ == std::make_tuple( 1, 10, 100 ) );
	REQUIRE( *r++ == std::make_tuple( 2, 20, 200 ) );
	REQUIRE( *r++ == std::make_tuple( 3, 30, 300 ) );

	REQUIRE( !r );
}


TEST_CASE( "Zipping range of 3 can be decremented", "[narl][zipped][3][decrement]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ), from( { 100, 200, 300 } ) );

	++r;
	REQUIRE( *r-- == std::make_tuple( 2, 20, 200 ) );
	REQUIRE( *r-- == std::make_tuple( 1, 10, 100 ) );

	REQUIRE( !r );
}


TEST_CASE( "Zipping range of 3 can be pre-decremented", "[narl][zipped][3][predecrement]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ), from( { 100, 200, 300 } ) );

	++r;
	REQUIRE( *r == std::make_tuple( 2, 20, 200 ) );
	--r;
	REQUIRE( *r == std::make_tuple( 1, 10, 100 ) );

	--r;
	REQUIRE( !r );

	++r;
	REQUIRE( !!r );
}


TEST_CASE( "Zipping range of 3 can be moved to end and decremented", "[narl][zipped][3][goto_end][decrement]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ), from( { 100, 200, 300 } ) );

	r.goto_end();
	REQUIRE( !r-- );

	REQUIRE( *r == std::make_tuple( 3, 30, 300 ) );
}


TEST_CASE( "Zipping range of 3 with different lengths is invalid at end of shortest in 1st", "[narl][zipped][3][shorteest][first]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2 } ), from( { 10, 20, 30 } ), from( { 100, 200, 300 } ) );

	++r;
	++r;
	REQUIRE( !r );
}



TEST_CASE( "Zipping range of 3 with different lengths is invalid at end of shortest in 2nd", "[narl][zipped][3][shorteest][second]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 30 } ), from( { 100, 200, 300 } ) );

	++r;
	++r;
	REQUIRE( !r );
}



TEST_CASE( "Zipping range of 3 with different lengths is invalid at end of shortest in last", "[narl][zipped][3][shorteest][last]" )
{
	auto r = make_test_range< zipping_range >( from( { 1, 2, 3 } ), from( { 10, 20, 30 } ), from( { 100, 200 } ) );

	++r;
	++r;
	REQUIRE( !r );
}



