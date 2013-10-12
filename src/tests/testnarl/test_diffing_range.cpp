#include "testnarl.h"

#include <diffing_range.h>
#include <iterable_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Diffing range is lazy evaluated", "[narl][diffing_range][lazy]" )
{
	auto r1 = make_test_range< diffing_range_default >( throwing_range< int > {}, from( { 1 } ) );
	auto r2 = make_test_range< diffing_range_default >( from( { 1 } ), throwing_range< int >() );

	REQUIRE_THROWS_AS( ++r1, range_access_exception );
	REQUIRE_THROWS_AS( --r1, range_access_exception );
	REQUIRE_THROWS_AS( r1++, range_access_exception );
	REQUIRE_THROWS_AS( r1--, range_access_exception );
	REQUIRE_THROWS_AS( *r1, range_access_exception );
	REQUIRE_THROWS_AS( !r1, range_access_exception );
	REQUIRE_THROWS_AS( r1.goto_end(), range_access_exception );

	REQUIRE_THROWS_AS( ++r2, range_access_exception );
	REQUIRE_THROWS_AS( --r2, range_access_exception );
	REQUIRE_THROWS_AS( r2++, range_access_exception );
	REQUIRE_THROWS_AS( r2.goto_end(), range_access_exception );
}


TEST_CASE( "Diffing range is dereferencable at start", "[narl][diffing_range][deref]" )
{
	auto r1 = make_test_range< diffing_range_default >( from( { 1, 2 } ), from( { 3, 4 } ) );

	REQUIRE( *r1 == 1 );
}


TEST_CASE( "Diffing range finds first mismatching element", "[narl][diffing_range][firstmismatch]" )
{
	auto r1 = make_test_range< diffing_range_default >( from( { 1, 2 } ), from( { 1, 4 } ) );

	REQUIRE( *r1 == 2 );
}


TEST_CASE( "Diffing range is invalid if no mismatching elements", "[narl][diffing_range][invalid]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3 } ), from( { 1, 2, 3 } ) );
	REQUIRE( !r );
}


TEST_CASE( "Diffing range skips first match when last in exception range", "[narl][skipfirst][lastexception]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 3, 4, 5 } ), from( { 1, 2, 3 } ) );
	REQUIRE( *r == 4 );
}


TEST_CASE( "Diffing range skips final match when first in exception range", "[narl][diffing_range][firstexception]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3 } ), from( { 3, 4, 5 } ) );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
}


TEST_CASE( "Diffing range is invalid after last mismatch when last is match", "[narl][diffing_range][invalid][lastmatch]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3, 4 } ), from( { 2, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Diffing range is invalid after last mismatch when last is mismatch", "[narl][diffing_range][invalid][lastmismatch]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3, 4, 5 } ), from( { 2, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 5 );
	REQUIRE( !r );
}


TEST_CASE( "Diffing range finds all mismatches", "[narl][diffing_range][all]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3, 4, 5 } ), from( { 1, 3, 5 } ) );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Diffing range skips duplicates in mismatches", "[narl][diffing_range][dedupe]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 2, 3, 4, 5 } ), from( { 1, 3, 5 } ) );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Diffing range skips multiple duplicates in mismatches", "[narl][diffing_range][dedupe][multiple]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 2, 2, 2, 3, 4, 5 } ), from( { 1, 3, 5 } ) );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Diffing range be moved to end and is invalid", "[narl][diffing_range][goto_end][invalid]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3 } ), from( { 2, 4, 6 } ) );
	r.goto_end();
	REQUIRE( !r );
}


TEST_CASE( "Diffing range finds last mismatch in reverse", "[narl][diffing_range][lastmismatch][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2 } ), from( { 1, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 2 );
}


TEST_CASE( "Diffing range is invalid if no mismatching elements in reverse", "[narl][diffing_range][invalid][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3 } ), from( { 1, 2, 3 } ) );
	r.goto_end();
	REQUIRE( !--r );
}


TEST_CASE( "Diffing range skips first match when last in exception range in reverse", "[narl][skipfirst][lastexception][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 3, 4, 5 } ), from( { 1, 2, 3 } ) );
	r.goto_end();
	REQUIRE( *--r == 5 );
}


TEST_CASE( "Diffing range skips final match when first in exception range in reverse", "[narl][diffing_range][firstexception][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3 } ), from( { 3, 4, 5 } ) );
	r.goto_end();
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
}


TEST_CASE( "Diffing range is invalid after last mismatch when last is match in reverse", "[narl][diffing_range][invalid][lastmatch][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3, 4 } ), from( { 2, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Diffing range is invalid after last mismatch when last is mismatch in reverse", "[narl][diffing_range][invalid][lastmismatch][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3, 4, 5 } ), from( { 2, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 5 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Diffing range skips duplicates in mismatches in reverse", "[narl][diffing_range][dedupe][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 2, 3, 4, 5 } ), from( { 1, 3, 5 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 2 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Diffing range skips multiple duplicates in mismatches in reverse", "[narl][diffing_range][dedupe][multiple][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 2, 2, 2, 3, 4, 5 } ), from( { 1, 3, 5 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 2 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Diffing range skips duplicates in mismatches on right in reverse", "[narl][diffing_range][dedupe][right][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 2, 3, 4, 5 } ), from( { 3, 4, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 5 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Diffing range skips duplicates in mismatches at end in reverse", "[narl][diffing_range][dedupe][end][reverse]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 2, 3, 4, 4 } ), from( { 1, 3, 5 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 2 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Diffing range can be incremented and valid from before begin", "[narl][diffing_range][beforebegin][increment]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3 } ), from( { 2, 3, 4 } ) );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
}


TEST_CASE( "Diffing range finds first diff when incremented from before begin", "[narl][diffing_range][beforebegin][firstdiff]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3 } ), from( { 2, 3, 4 } ) );
	--r;
	REQUIRE( *++r == 1 );
}


TEST_CASE( "Diffing range can be post-decremented to find all matches", "[narl][diffing_range][all][postdecrement]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 2, 3, 4, 4 } ), from( { 1, 3, 3, 5 } ) );
	r.goto_end();
	--r;

	REQUIRE( *r-- == 4 );
	REQUIRE( *r-- == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Diffing range can be given custom comparer", "[narl][diffing_range][customcmp]" )
{
	auto r = make_test_range< diffing_range >( from( { 4, 4, 3, 2, 2, 1 } ), from( { 5, 3, 1 } ), []( int l, int r ) { return l > r; } );

	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Diffing range finds last diff when decremented from end", "[narl][diffing_range][end][lastdiff]" )
{
	auto r = make_test_range< diffing_range_default >( from( { 1, 2, 3 } ), from( { 0, 1, 2 } ) );
	r.goto_end();
	REQUIRE( *--r == 3 );
}


