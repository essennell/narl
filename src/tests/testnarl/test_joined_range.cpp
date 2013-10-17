#include "testnarl.h"

#include <iterable_range.h>
#include <joined_range.h>

#include <catch.hpp>

#include <functional>
#include <string>


using namespace narl;


TEST_CASE( "Joined range is lazy evaluated", "[narl][join][lazy]" )
{
	auto r = make_test_range< joined_range_default >( throwing_range< int >(), from( { 3, 4 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int, int ) { return 0; } );

	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( --r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( r--, range_access_exception );
	REQUIRE_THROWS_AS( !r, range_access_exception );
}


TEST_CASE( "Joining range returns fist matching element if not 1st on left", "[narl][joined_range][notfirstmatch][left]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 0, 1, 3 } ), 
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r == 1 );
}


TEST_CASE( "Joining range returns fist matching element if not 1st on right", "[narl][joined_range][notfirstmatch][right]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 0, 1, 3 } ), 
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r == 1 );
}


TEST_CASE( "Joining range returns fist matching element if not 1st", "[narl][joined_range][notfirstmatch]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 0, 2, 3 } ), 
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r == 2 );
}


TEST_CASE( "Joining range returns match when found in last position on left", "[narl][joined_range][lastismatch][left]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 0, 1, 4 } ), 
		from( { 2, 4, 5 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r == 4 );
}


TEST_CASE( "Joining range returns match when found in last position on right", "[narl][joined_range][lastismatch][right]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 0, 3, 4 } ), 
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r == 3 );
}


TEST_CASE( "Joining range returns match when found in last position", "[narl][joined_range][lastismatch]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 0, 1, 4 } ), 
		from( { 2, 3, 4 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r == 4 );
}


TEST_CASE( "Joining range finds second element after initial increment", "[narl][joined_range][increment][start]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 2, 3 } ), 
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	++r;
	REQUIRE( *r == 2 );
}


TEST_CASE( "Joining range is immediately invalid if no matches", "[narl][joined_range][nomatch]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 0, 1, 2 } ), 
		from( { 3, 4, 5 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( !r );
}


TEST_CASE( "Joining range finds every element when they all match", "[narl][joined_range][allmatch]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 2, 3 } ), 
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Joining range keeps duplicates in left from output", "[narl][joined_range][keepdupe][left]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 1, 2, 3, 3 } ), 
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Joining range keeps duplicates in right from output", "[narl][joined_range][keepdupe][right]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 2, 3 } ), 
		from( { 1, 1, 2, 3, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Joining range keeps duplicates from output", "[narl][joined_range][keepdupe]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 2, 2, 3 } ), 
		from( { 1, 1, 2, 3, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Joining range keeps symmetrical duplicates from output", "[narl][joined_range][keepdupe][symmetrica]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 1, 2, 3, 3 } ), 
		from( { 1, 1, 2, 3, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Joining range finds interleaved matches", "[narl][joined_range][interleaved]" )
{
	auto r = make_test_range< joined_range_default >(
		from( { 1, 3, 5, 7, 9, 11 } ),
		from( { 2, 4, 5, 6, 8, 9, 10 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 9 );
	REQUIRE( !r );
}


TEST_CASE( "Joining range finds previous match", "[narl][joined_range][prev]" )
{
	auto r = make_test_range< joined_range_default >(
		from( { 1, 3, 5, 7, 9, 11 } ),
		from( { 2, 4, 5, 6, 8, 9, 10 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	REQUIRE( *r++ == 5 );
	REQUIRE( *r == 9 );
	--r;
	REQUIRE( *r == 5 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Joining range can be incremented from before begin", "[narl][joined_range][beforebegin]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 2, 3 } ),
		from( { 0, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( *r == 2 );
}


TEST_CASE( "Joining range can be decrmented from end", "[narl][joined_range][decrmented][end]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 2, 3 } ),
		from( { 0, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r-- == 3 );
	REQUIRE( *r-- == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Joining range can be decremented over interleaved matches", "[narl][joined_range][decrmented][interleaved]" )
{
	auto r = make_test_range< joined_range_default >(
		from( { 1, 3, 5, 7, 9, 11 } ),
		from( { 2, 4, 5, 6, 8, 9, 10 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	r.goto_end();
	--r;
	REQUIRE( *r == 9 );
	--r;
	REQUIRE( *r == 5 );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Joining range can be post-decremented over interleaved matches", "[narl][joined_range][postdecrmented][interleaved]" )
{
	auto r = make_test_range< joined_range_default >(
		from( { 1, 3, 5, 7, 9, 11 } ),
		from( { 2, 4, 5, 6, 8, 9, 10 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );

	r.goto_end();
	REQUIRE( !r-- );
	REQUIRE( *r-- == 9 );
	REQUIRE( *r-- == 5 );
	REQUIRE( !r );
}


TEST_CASE( "Joining range finds last match when in last place with decrement", "[narl][joined_range][decremented][lastismatch]" )
{
	auto r = make_test_range< joined_range_default >( 
		from( { 1, 2, 3 } ),
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );
	r.goto_end();
	REQUIRE( *--r == 3 );
}


TEST_CASE( "Joining range finds last match when in last place on left", "[narl][joined_range][decrement][lastismatch][left]" )
{
	auto r = make_test_range< joined_range_default >(
		from( { 1, 2, 3 } ),
		from( { 1, 2, 3, 4, 5 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );
	r.goto_end();
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
}


TEST_CASE( "Joining range keeps duplicates on right in reverse", "[narl][joined_range][decrement][keepdupe][right]" )
{
	auto r = make_test_range< joined_range_default >(
		from( { 1, 2, 3, 4, 5 } ),
		from( { 1, 2, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );
	r.goto_end();
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
}


TEST_CASE( "Joining range keeps duplicates on left in reverse", "[narl][joined_range][decrement][keepdupe][left]" )
{
	auto r = make_test_range< joined_range_default >(
		from( { 1, 2, 2, 3, 4, 5 } ),
		from( { 1, 2, 3 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );
	r.goto_end();
	REQUIRE( *--r == 3 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 2 );
	REQUIRE( *--r == 1 );
}


TEST_CASE( "Joining range can be incremented and valid from before begin", "[narl][joined_range][beforebegin][increment]" )
{
	auto r = make_test_range< joined_range_default >( from( { 1, 2, 3 } ), from( { 2, 3, 4 } ),
		[]( int i ) { return i; },
		[]( int i ) { return i; },
		[]( int l, int ) { return l; } );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
}

#ifndef _MSC_VER

TEST_CASE( "Joined range can produce a different result type from 2 inputs", "[narl][joined_range][differenttypes]" )
{
	struct result
	{
		std::string s;
		size_t l;
		int i;
	};

	std::vector< std::string > left { "2", "4", "6" };
	auto r = make_test_range< joined_range_default >(
		from( left ),
		from( { 2, 4, 6 } ),
		[]( const std::string & l ) { return std::atoi( l.c_str() ); },
		[]( int i ) { return i; },
		[]( const std::string & l, int r ) { return result{ "ID:" + l, l.size(), r * 2 }; } );

	REQUIRE( ( *r ).s == "ID:2" );
	REQUIRE( ( *r ).l == 1 );
	REQUIRE( ( *r ).i == 4 );
}

#endif
