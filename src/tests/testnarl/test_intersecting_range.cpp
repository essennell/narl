#include "testnarl.h"

#include <intersecting_range.h>
#include <iterable_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Intersecting range is lazy evaluated", "[narl][intersecting_range][lazy]" )
{
	auto r1 = make_test_range< intersecting_range_default >( throwing_range< int > {}, from( { 1 } ) );
	auto r2 = make_test_range< intersecting_range_default >( from( { 1 } ), throwing_range< int >() );

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
	REQUIRE_THROWS_AS( *r2, range_access_exception );
	REQUIRE_THROWS_AS( !r2, range_access_exception );
	REQUIRE_THROWS_AS( r2.goto_end(), range_access_exception );
}


TEST_CASE( "Intersecting range has first value at start if it's a match", "[narl][intersecting_range][firstmatch]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 3 } ), 
		from( { 1, 2, 3 } ) );

	REQUIRE( *r == 1 );
}


TEST_CASE( "Intersecting range returns fist matching element if not 1st on left", "[narl][intersecting_range][notfirstmatch][left]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 0, 1, 3 } ), 
		from( { 1, 2, 3 } ) );

	REQUIRE( *r == 1 );
}


TEST_CASE( "Intersecting range returns fist matching element if not 1st on right", "[narl][intersecting_range][notfirstmatch][right]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 0, 1, 3 } ), 
		from( { 1, 2, 3 } ) );

	REQUIRE( *r == 1 );
}


TEST_CASE( "Intersecting range returns fist matching element if not 1st", "[narl][intersecting_range][notfirstmatch]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 0, 2, 3 } ), 
		from( { 1, 2, 3 } ) );

	REQUIRE( *r == 2 );
}


TEST_CASE( "Intersecting range returns match when found in last position on left", "[narl][intersecting_range][lastismatch][left]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 0, 1, 4 } ), 
		from( { 2, 4, 5 } ) );

	REQUIRE( *r == 4 );
}


TEST_CASE( "Intersecting range returns match when found in last position on right", "[narl][intersecting_range][lastismatch][right]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 0, 3, 4 } ), 
		from( { 1, 2, 3 } ) );

	REQUIRE( *r == 3 );
}


TEST_CASE( "Intersecting range returns match when found in last position", "[narl][intersecting_range][lastismatch]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 0, 1, 4 } ), 
		from( { 2, 3, 4 } ) );

	REQUIRE( *r == 4 );
}


TEST_CASE( "Intersecting range finds second element after initial increment", "[narl][intersecting_range][increment][start]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 3 } ), 
		from( { 1, 2, 3 } ) );

	++r;
	REQUIRE( *r == 2 );
}


TEST_CASE( "Intersecting range is immediately invalid if no matches", "[narl][intersecting_range][nomatch]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 0, 1, 2 } ), 
		from( { 3, 4, 5 } ) );

	REQUIRE( !r );
}


TEST_CASE( "Intersecting range finds every element when they all match", "[narl][intersecting_range][allmatch]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 3 } ), 
		from( { 1, 2, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range removes duplicates in left from output", "[narl][intersecting_range][dedupe][left]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 1, 2, 3, 3 } ), 
		from( { 1, 2, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range removes multiple symmetrical duplicates from output", "[narl][intersecting_range][dedupe][symmetrical][multiple]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 1, 1, 2, 3, 3, 3 } ), 
		from( { 1, 1, 1, 2, 3, 3, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range removes multiple duplicates in left from output", "[narl][intersecting_range][dedupe][left][multiple]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 1, 1, 2, 3, 3, 3 } ), 
		from( { 1, 2, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range removes duplicates in right from output", "[narl][intersecting_range][dedupe][right]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 3 } ), 
		from( { 1, 1, 2, 3, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range removes multiple duplicates in right from output", "[narl][intersecting_range][dedupe][right][multiple]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 3 } ), 
		from( { 1, 1, 1, 2, 3, 3, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range removes duplicates from output", "[narl][intersecting_range][dedupe]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 2, 3 } ), 
		from( { 1, 1, 2, 3, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range removes multiple duplicates from output", "[narl][intersecting_range][dedupe][multiple]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 2, 2, 3 } ), 
		from( { 1, 1, 1, 2, 3, 3, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range removes symmetrical duplicates from output", "[narl][intersecting_range][dedupe][symmetrica]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 1, 2, 3, 3 } ), 
		from( { 1, 1, 2, 3, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range finds interleaved matches", "[narl][intersecting_range][interleaved]" )
{
	auto r = make_test_range< intersecting_range_default >(
		from( { 1, 3, 5, 7, 9, 11 } ),
		from( { 2, 4, 5, 6, 8, 9, 10 } ) );

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 9 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range finds previous match", "[narl][intersecting_range][prev]" )
{
	auto r = make_test_range< intersecting_range_default >(
		from( { 1, 3, 5, 7, 9, 11 } ),
		from( { 2, 4, 5, 6, 8, 9, 10 } ) );

	REQUIRE( *r++ == 5 );
	REQUIRE( *r == 9 );
	--r;
	REQUIRE( *r == 5 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range can be incremented from before begin", "[narl][intersecting_range][beforebegin]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 3 } ),
		from( { 0, 2, 3 } ) );

	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( *r == 2 );
}


TEST_CASE( "Intersecting range can be decrmented from end", "[narl][intersecting_range][decrmented][end]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 3 } ),
		from( { 0, 2, 3 } ) );

	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r-- == 3 );
	REQUIRE( *r-- == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range can be decremented over interleaved matches", "[narl][intersecting_range][decrmented][interleaved]" )
{
	auto r = make_test_range< intersecting_range_default >(
		from( { 1, 3, 5, 7, 9, 11 } ),
		from( { 2, 4, 5, 6, 8, 9, 10 } ) );

	r.goto_end();
	--r;
	REQUIRE( *r == 9 );
	--r;
	REQUIRE( *r == 5 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range can be post-decremented over interleaved matches", "[narl][intersecting_range][postdecrmented][interleaved]" )
{
	auto r = make_test_range< intersecting_range_default >(
		from( { 1, 3, 5, 7, 9, 11 } ),
		from( { 2, 4, 5, 6, 8, 9, 10 } ) );

	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r-- == 9 );
	REQUIRE( *r-- == 5 );
	REQUIRE( !r );
}


TEST_CASE( "Intersecting range finds last match when in last place with decrement", "[narl][intersecting_range][decremented][lastismatch]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 2, 3 } ),
		from( { 1, 2, 3 } ) );
	r.goto_end();
	REQUIRE( *--r == 3 );
}


TEST_CASE( "Intersecting range finds last match when in last place on left", "[narl][intersecting_range][decrement][lastismatch][left]" )
{
	auto r = make_test_range< intersecting_range_default >(
		from( { 1, 2, 3 } ),
		from( { 1, 2, 3, 4, 5 } ) );
	r.goto_end();
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
}


TEST_CASE( "Intersecting range removes duplicates on right in reverse", "[narl][intersecting_range][decrement][dedupe][right]" )
{
	auto r = make_test_range< intersecting_range_default >(
		from( { 1, 2, 3, 4, 5 } ),
		from( { 1, 2, 2, 3 } ) );
	r.goto_end();
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
}


TEST_CASE( "Intersecting range removes duplicates on left in reverse", "[narl][intersecting_range][decrement][dedupe][left]" )
{
	auto r = make_test_range< intersecting_range_default >(
		from( { 1, 2, 2, 3, 4, 5 } ),
		from( { 1, 2, 3 } ) );
	r.goto_end();
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
}


TEST_CASE( "Intersecting range removes multiple duplicates on left in reverse", "[narl][intersecting_range][decrement][dedupe][multiple][left]" )
{
	auto r = make_test_range< intersecting_range_default >(
		from( { 1, 2, 2, 2, 3, 4, 5 } ),
		from( { 1, 2, 3 } ) );
	r.goto_end();
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
}


TEST_CASE( "Intersecting range removes symmetrical duplicates from output in reverse", "[narl][intersecting_range][dedupe][symmetrica][reverse]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 1, 2, 3, 3 } ), 
		from( { 1, 1, 2, 3, 3 } ) );
	r.goto_end();

	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Intersecting range removes multiple symmetrical duplicates from output in reverse", "[narl][intersecting_range][dedupe][symmetrica][reverse][multiple]" )
{
	auto r = make_test_range< intersecting_range_default >( 
		from( { 1, 1, 1, 2, 3, 3, 3 } ), 
		from( { 1, 1, 1, 2, 3, 3, 3 } ) );
	r.goto_end();

	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Intersecting range can be incremented and valid from before begin", "[narl][intersecting_range][beforebegin][increment]" )
{
	auto r = make_test_range< intersecting_range_default >( from( { 1, 2, 3 } ), from( { 2, 3, 4 } ) );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
	REQUIRE( *r == 2 );
}


TEST_CASE( "Intersecting range incremented from before begin finds first match at start", "[narl][intersecting_range][beforebegin][matchatstart]" )
{
	auto r = make_test_range< intersecting_range_default >( from( { 1, 2, 3 } ), from( { 1, 3, 4 } ) );
	--r;
	REQUIRE( !r++ );
	REQUIRE( !!r );
	REQUIRE( *r == 1 );
}


TEST_CASE( "Intersecting range can accept a custom comparer", "[narl][intersecting_range][custom]" )
{
	auto r = make_test_range< intersecting_range >( from( { 3, 2, 1 } ), from( { 4, 3, 2 } ), []( int l, int r ) { return l > r; } );

	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Intersection of an empty range with non-empty produces invalid range", "[narl][intersecting_range][empty][nonemtpy]" )
{
	std::vector< int > src;
	auto r = make_test_range< intersecting_range >( from( src ), from( { 1 } ), []( int l, int r ) { return l < r; } );

	REQUIRE( !r );
}


TEST_CASE( "Intersection of range with empty produces invalid range", "[narl][intersecting_range][nonempty][emtpy]" )
{
	std::vector< int > src;
	auto r = make_test_range< intersecting_range >( from( { 1 } ), from( src ), []( int l, int r ) { return l < r; } );

	REQUIRE( !r );
}


TEST_CASE( "Intersection of empty range with empty produces invalid range", "[narl][intersecting_range][empty][emtpy]" )
{
	std::vector< int > src1, src2;
	auto r = make_test_range< intersecting_range >( from( src1 ), from( src2 ), []( int l, int r ) { return l < r; } );

	REQUIRE( !r );
}


TEST_CASE( "Intersection skips duplicate non-matches", "[narl][intersecting_range][nonmatch][duplicates]" )
{
	auto r = make_test_range< intersecting_range >( from( { 1, 2, 2, 2, 3, 3, 3, 4 } ), from( { 1, 4 } ), []( int l, int r ) { return l < r; } );

	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Intersection skips duplicate non-matches in reverse", "[narl][intersecting_range][nonmatch][duplicates][reversed]" )
{
	auto r = make_test_range< intersecting_range >( from( { 1, 2, 2, 2, 3, 3, 3, 4 } ), from( { 1, 4 } ), []( int l, int r ) { return l < r; } );
	r.goto_end();

	REQUIRE( !r-- );
	REQUIRE( *r-- == 4 );
	REQUIRE( *r-- == 1 );
	REQUIRE( !r );
}





