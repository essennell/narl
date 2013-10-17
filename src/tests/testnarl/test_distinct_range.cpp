#include "testnarl.h"

#include <distinct_range.h>
#include <iterable_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Distinct range is lazy evaluated", "[narl][distinct][lazy]" )
{
	auto r = make_test_range< distinct_range_default >( throwing_range< int >() );

	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( --r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( r--, range_access_exception );
	REQUIRE_THROWS_AS( !r, range_access_exception );
}


TEST_CASE( "Distinct range returns item if its the only one", "[narl][distinct][only]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1 } ) );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range skips duplicates from start", "[narl][distinct][duplicates][start]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 1, 2, 3 } ) );
	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range skips duplicates at end", "[narl][distinct][duplicates][end]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 2, 3, 3 } ) );
	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range skips duplicates in middle", "[narl][distinct][duplicates][middle]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 2, 2, 3 } ) );
	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range skips all duplicates", "[narl][distinct][duplicates][all]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 1, 2, 2, 3, 3 } ) );
	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range skips all duplicates multiply defined", "[narl][distinct][duplicates][all][multiple]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 1, 1, 2, 2, 2, 3, 3, 3 } ) );
	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range can be moved to end and is invalid", "[narl][distinct][gotoend][invalid]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 2 } ) );
	r.goto_end();
	REQUIRE( !r );
}


TEST_CASE( "Distinct range can be decremented from end", "[narl][distinct][decrement][fromend]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 2 } ) );
	r.goto_end();
	--r;
	REQUIRE( !!r );
	REQUIRE( *r-- == 2 );
	REQUIRE( *r-- == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range can be incremented from before begin", "[narl][distinct][increment][beforebegin]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 2 } ) );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
}


TEST_CASE( "Distinct range skips duplicates from start in reverse", "[narl][distinct][duplicates][start][reverse]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 1, 2, 3 } ) );
	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r-- == 3 );
	REQUIRE( *r-- == 2 );
	REQUIRE( *r-- == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range skips duplicates at end in reverse", "[narl][distinct][duplicates][end][reverse]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 2, 3, 3 } ) );
	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r-- == 3 );
	REQUIRE( *r-- == 2 );
	REQUIRE( *r-- == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range skips duplicates in middle in reverse", "[narl][distinct][duplicates][middle][reverse]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 2, 2, 3 } ) );
	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r-- == 3 );
	REQUIRE( *r-- == 2 );
	REQUIRE( *r-- == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range skips all duplicates in reverse", "[narl][distinct][duplicates][all][reverse]" )
{
	auto r = make_test_range< distinct_range_default >( from( { 1, 1, 2, 2, 3, 3 } ) );
	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r-- == 3 );
	REQUIRE( *r-- == 2 );
	REQUIRE( *r-- == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range can use supplied comparer", "[narl][distinct][customcomparer]" )
{
	auto r = make_test_range< distinct_range >( from( { 5, 4, 3, 3, 3, 1 } ), []( int l, int r ) { return l == r; } );

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}



