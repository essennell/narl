#include "testnarl.h"

#include <unioning_range.h>
#include <iterable_range.h>

#include <catch.hpp>


using namespace narl;


TEST_CASE( "Unioning range is lazy evaluated", "[narl][unioning_range][lazy]" )
{
	auto r1 = make_test_range< unioning_range_default >( throwing_range< int > {}, from( { 1 } ) );
	auto r2 = make_test_range< unioning_range_default >( from( { 1 } ), throwing_range< int >() );

	REQUIRE_THROWS_AS( ++r1, range_access_exception );
	REQUIRE_THROWS_AS( --r1, range_access_exception );
	REQUIRE_THROWS_AS( r1++, range_access_exception );
	REQUIRE_THROWS_AS( r1--, range_access_exception );
	REQUIRE_THROWS_AS( *r1, range_access_exception );
	REQUIRE_THROWS_AS( !r1, range_access_exception );
	REQUIRE_THROWS_AS( r1.goto_end(), range_access_exception );

	REQUIRE_THROWS_AS( ++r2, range_access_exception );
	REQUIRE_THROWS_AS( r2++, range_access_exception );
	REQUIRE_THROWS_AS( *r2, range_access_exception );
	REQUIRE_THROWS_AS( r2.goto_end(), range_access_exception );
}


TEST_CASE( "Unioning range can be dereferecned at first element on left", "[narl][unioning_range][deref][left]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );
	REQUIRE( *r == 1 );
}


TEST_CASE( "Unioning range can be dereferecned at first element on right", "[narl][unioning_range][deref][right]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 2, 4 } ), from( { 1, 3 } ) );
	REQUIRE( *r == 1 );
}


TEST_CASE( "Unioning range can be incremented at first element", "[narl][unioning_range][deref][left]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range can be incremented at first element from right", "[narl][unioning_range][deref][right]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 2, 4 } ), from( { 1, 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range can be incremented over left then right", "[narl][unioning_range][deref][chained]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2 } ), from( { 3, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range is valid at end of left range", "[narl][unioning_range][valid][left]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1 } ), from( { 2, 3, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range is valid at end of right range", "[narl][unioning_range][valid][right]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 3 } ), from( { 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range is valid at end of right range when element remain on left", "[narl][unioning_range][valid][right][leftremaining]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 4 } ), from( { 3 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range is valid at end of left range when element remain on right", "[narl][unioning_range][valid][left][rightremaining]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 2 } ), from( { 1, 3, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range is invalid after goto_end and valid after decrement", "[narl][unioning_range][goto_end]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );
	r.goto_end();

	REQUIRE( !r );
	REQUIRE( !!--r );
}


TEST_CASE( "Unioning range includes last match on left after decrement from end", "[narl][unioning_range][end][lastmatch][left]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 2, 4 } ), from( { 1, 3 } ) );
	r.goto_end();
	
	REQUIRE( *--r == 4 );
}


TEST_CASE( "Unioning range includes last match on right after decrement from end", "[narl][unioning_range][end][lastmatch][right]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );
	r.goto_end();
	
	REQUIRE( *--r == 4 );
}


TEST_CASE( "Unioning range can be decremented at last element", "[narl][unioning_range][deref]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range can be decremented at last element from left", "[narl][unioning_range][deref][left]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 2, 4 } ), from( { 1, 3 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range can be decremented over right then left", "[narl][unioning_range][deref][chained]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2 } ), from( { 3, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range is valid at start of left range in reverse", "[narl][unioning_range][valid][reverse]][left]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1 } ), from( { 2, 3, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range is valid at start of right range in reverse", "[narl][unioning_range][valid][reverse][right]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 3 } ), from( { 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range is valid at end of right range when element remain on left in reverse", "[narl][unioning_range][valid][reverse][right][leftremaining]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 4 } ), from( { 3 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range is valid at end of left range when element remain on right in reverse", "[narl][unioning_range][valid][reverse][left][rightremaining]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 2 } ), from( { 1, 3, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range keeps duplicates on left", "[narl][unioning_range][dedupe][left]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 2, 3 } ), from( { 2, 3, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range keeps duplicates on right", "[narl][unioning_range][dedupe][right]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 3 } ), from( { 2, 3, 3, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Unioning range keeps duplicates on left in reverse", "[narl][unioning_range][dedupe][left][reverse]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 2, 3 } ), from( { 2, 3, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range keeps duplicates on right in reverse", "[narl][unioning_range][dedupe][right][reverse]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 3 } ), from( { 2, 3, 3, 4 } ) );
	r.goto_end();

	REQUIRE( *--r == 4 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
	REQUIRE( !--r );
}


TEST_CASE( "Unioning range can be incremented and valid from before begin", "[narl][unioning_range][beforebegin][increment]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 2, 3 } ), from( { 2, 3, 4 } ) );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
}


TEST_CASE( "Unioning range includes first match on left after increment from beforebegin", "[narl][unioning_range][beforebegin][firstmatch][left]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 1, 3 } ), from( { 2, 4 } ) );
	--r;
	REQUIRE( *++r == 1 );
}


TEST_CASE( "Unioning range includes first match on right after increment from beforebegin", "[narl][unioning_range][beforebegin][firstmatch][right]" )
{
	auto r = make_test_range< unioning_range_default >( from( { 2, 4 } ), from( { 1, 3 } ) );
	--r;
	REQUIRE( *++r == 1 );
}


TEST_CASE( "Unioning range uses custom comparer", "[narl][unioning_range][comparer]" )
{
	auto r = make_test_range< unioning_range >( from( { 1 } ), from( { 4, 3, 2 } ), []( int l, int r ) { return l > r; } );

	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}



