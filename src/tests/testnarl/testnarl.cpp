#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "throwing_range.h"

#include <narl.h>

#include <array>
#include <list>
#include <string>
#include <vector>


using namespace narl;


TEST_CASE( "Range can be initialised from named container", "[narl][container][reference]" )
{
	std::vector< int > data { 1, 2, 3 };
	std::vector< int > original { data };
	
	SECTION( "Scope" )
	{
		auto r = from( data );
		REQUIRE( *r++ == 1 );
		REQUIRE( *r++ == 2 );
		REQUIRE( *r++ == 3 );
		REQUIRE( !r );
	}
	REQUIRE( std::equal( std::begin( data ), std::end( data ), std::begin( original ) ) );
}


TEST_CASE( "Range can be initialised from temp container", "[narl][container][temp]" )
{
	auto r = from( std::vector< int > { 1, 2, 3 } );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Range can be initialised from named init list", "[narl][init_list][reference]" )
{
	auto data = { 1, 2, 3 };
	std::vector< int > original{ data };

	SECTION( "Scope" )
	{
		auto r = from( data );
		REQUIRE( *r++ == 1 );
		REQUIRE( *r++ == 2 );
		REQUIRE( *r++ == 3 );
		REQUIRE( !r );
	}
	REQUIRE( std::equal( std::begin( data ), std::end( data ), std::begin( original ) ) );
}


TEST_CASE( "Range can be initialised from temp init list", "[narl][init_list][temp]" )
{
	auto r = from( { 1, 2, 3 } );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}


TEST_CASE( "Range can be initialised from array", "[narl][array]" )
{
	int data [] = { 1, 2, 3 };
	std::vector< int > original{ std::begin( data ), std::end( data ) };

	SECTION( "Scope" )
	{
		auto r = from( data );
		REQUIRE( *r++ == 1 );
		REQUIRE( *r++ == 2 );
		REQUIRE( *r++ == 3 );
		REQUIRE( !r );
	}
	REQUIRE( std::equal( std::begin( data ), std::end( data ), std::begin( original ) ) );
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


TEST_CASE( "Range can be generated with callback", "[narl][generate][callback]" )
{
	auto r = make_range< int >( 1, []( int i ) { return i * 2; } );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 8 );
	REQUIRE( *r++ == 16 );
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


TEST_CASE( "Skip from a filtered range ignores the specified count of elements", "[narl][skip][where]" )
{
	auto r = from( { 1, 2, 3, 4, 5, 6 } ) | where( []( int i ) { return i % 2 == 0; } ) | skip( 2 );

	REQUIRE( *r++ == 6 );
	REQUIRE( !r );
}


TEST_CASE( "Take from a filtered range contains the specified count of elements", "[narl][take][where]" )
{
	auto r = from( { 1, 2, 3, 4, 5, 6 } ) | where( []( int i ) { return i % 2 == 0; } ) | take( 2 );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 4 );
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
	REQUIRE_THROWS_AS( r && true, range_access_exception );
}


TEST_CASE( "Where produces only elements that match the condition", "[narl][where]" )
{
	auto r = from( { 1, 2, 3, 4, 5 } ) | where( []( int i ) { return i % 2 == 0; } );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}

TEST_CASE( "Sort produces elements in order according to comparer", "[narl][sorted]" )
{
	auto r = from( { 2, 3, 1, 4, 0 } ) | sorted( []( int l, int r ) { return l < r; } );
	auto expected = make_range< int >();
	
	REQUIRE( *r++ == *expected++ );
	REQUIRE( *r++ == *expected++ );
	REQUIRE( *r++ == *expected++ );
	REQUIRE( *r++ == *expected++ );
	REQUIRE( *r++ == *expected++ );
	REQUIRE( !r );
}


TEST_CASE( "Sort without a comparer just uses less than", "[narl][sorted][default]" )
{
	auto r = from( { 2, 3, 1, 4, 0 } ) | sorted();
	auto expected = make_range< int >();
	
	REQUIRE( *r++ == *expected++ );
	REQUIRE( *r++ == *expected++ );
	REQUIRE( *r++ == *expected++ );
	REQUIRE( *r++ == *expected++ );
	REQUIRE( *r++ == *expected++ );
	REQUIRE( !r );
}


TEST_CASE( "Reverse produces elements in reverse order", "[narl][reverse]" )
{
	auto r = from( { 2, 1, 0 } ) | reverse();
	auto expected = make_range< int >();

	for( auto i : r )
	{
		REQUIRE( i == *expected++ );
	}
}


TEST_CASE( "Reverse reverses a transformed range", "[narl][reverse][transforming_range]" )
{
	auto r = from( { 1, 2, 3 } ) | select( []( int i ) { return i * 5; } ) | reverse();
	auto expected = from( { 15, 10, 5 } );

	for( auto i : r )
	{
		REQUIRE( i == *expected++ );
	}
}


TEST_CASE( "Reverse reverses a filter range", "[narl][reverse][where]" )
{
	auto r = from( { 1, 2, 3, 4, 5 } )
				| where( []( int i ) { return i % 2 == 0; } )
				| reverse();
	auto expected = from( { 4, 2 } );

	for( auto i : r )
	{
		REQUIRE( i == *expected++ );
	}
}


TEST_CASE( "Reversing a reversed range produces original range", "[narl][reverse][reversed]" )
{
	std::vector< int > src { 1, 2, 3, 4 };
	auto r = from( src )
			| reverse()
			| reverse();

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}



TEST_CASE( "Reverse produces elements in reverse order from take", "[narl][reverse][take]" )
{
	auto r = from( { 1, 2, 3, 4, 5 } )
			| take( 2 )
			| reverse();
	auto expected = from( { 2, 1 } );

	for( auto i : r )
	{
		REQUIRE( i == *expected++ );
	}
}


/*
TEST_CASE( "Reverse fails to compile for infinite range", "[narl][reverse][infinite]" )
{
	auto r = make_range< int >() | reverse();
	//REQUIRE( !r );
}
*/

TEST_CASE( "Reverse produces elements in reverse order from skip", "[narl][reverse][skip]" )
{
	auto r = from( { 1, 2, 3, 4, 5 } )
			| skip( 3 )
			| reverse();
	auto expected = from( { 5, 4 } );

	for( auto i : r )
	{
		REQUIRE( i == *expected++ );
	}
}


TEST_CASE( "Select many produces flattened range of provided ranges", "[narl][selectmany]" )
{
	struct item
	{
		int id;
		std::vector< int > items;
	};
	std::vector< item > src
	{
		item{ 0, { 10, 20 } },
		item{ 1, { 11, 21 } },
		item{ 2, { 12, 22 } },
	};

	auto r = from( src ) | selectmany( []( const item & i ) { return from( i.items ); } );

	REQUIRE( *r++ == src[ 0 ].items[ 0 ] );
	REQUIRE( *r++ == src[ 0 ].items[ 1 ] );
	REQUIRE( *r++ == src[ 1 ].items[ 0 ] );
	REQUIRE( *r++ == src[ 1 ].items[ 1 ] );
	REQUIRE( *r++ == src[ 2 ].items[ 0 ] );
	REQUIRE( *r++ == src[ 2 ].items[ 1 ] );
	REQUIRE( !r );
}


/*
TEST_CASE( "Reverse fails to compile for skip from infinite range", "[narl][reverse][skip][infinite]" )
{
	auto r = make_range< int >() | skip( 20 ) | reverse();
	//REQUIRE( !r );
}
*/

TEST_CASE( "Any returns true if any elements match", "[narl][any][expression][true]" )
{
	auto r = from( { 1, 2 } ) | any( []( int i ) { return i == 2; } );
	REQUIRE( !!r );
}


TEST_CASE( "Any returns false if no elements match", "[narl][any][expression][false]" )
{
	auto r = from( { 1, 2 } ) | any( []( int i ) { return i == 3; } );
	REQUIRE( !r );
}


TEST_CASE( "Any with no expression returns true if range has elements", "[narl][any][true]" )
{
	auto r = from( { 1 } ) | any();
	REQUIRE( !!r );
}


TEST_CASE( "Any with no expression returns false if range has no elements", "[narl][any][false]" )
{
	std::vector< int > src;
	auto r = from( src ) | any();
	REQUIRE( !r );
}


TEST_CASE( "All returns true if all elements match", "[narl][all][expression][true]" )
{
	auto r = from( { 1, 2 } ) | all( []( int i ) { return i < 3; } );
	REQUIRE( !!r );
}


TEST_CASE( "All returns false if any elements mismatch", "[narl][all][expression][true]" )
{
	auto r = from( { 1, 2 } ) | all( []( int i ) { return i == 1; } );
	REQUIRE( !r );
}


TEST_CASE( "All returns true for range with no elements", "[narl][all][true][empty]" )
{
	std::vector< int > src;
	auto r = from( src ) | all( []( int ) { return false; } );
	REQUIRE( !!r );
}


TEST_CASE( "Sequence equal is false if ranges mismatch", "[narl][sequence_equal][mismatch]" )
{
	auto result = from( { 1, 5, 3, 0 } ) | sequence_equal( from( { 0, 1, 3, 5 } ) );
	REQUIRE( !result );
}


TEST_CASE( "Sequence equal is true if ranges match", "[narl][sequence_equal][match]" )
{
	auto result = from( { 1, 5, 3, 0 } ) | sorted() | sequence_equal( from( { 0, 1, 3, 5 } ) );
	REQUIRE( !!result );
}


TEST_CASE( "Sequence equal can be given a custom comparer", "[narl][sequence_equal][customcmp]" )
{
	auto result = from( { 1, 2, 3 } ) | sequence_equal( from( { 2, 4, 6 } ), []( int l, int r ) { return r == l * 2; } );
	REQUIRE( !!result );
}


TEST_CASE( "Concat produces 2 ranges", "[narl][concat]" )
{
	auto r = from( { 1, 2 } ) | concat( from( { 3, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


TEST_CASE( "Concat can be reversed", "[narl][concat][reverse]" )
{
	auto r = from( { 1, 2 } ) | concat( from( { 3, 4 } ) ) | reverse();

	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Except produces set of all results not in second range", "[narl][except]" )
{
	auto r = from( { 1, 2, 2, 3, 4, 5 } ) | except( from( { 3, 4, 4 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 5 );
	REQUIRE( !r );
}


TEST_CASE( "Reversed except produces reversed list of all results not in second range", "[narl][except][reverse]" )
{
	auto r = from( { 1, 2, 2, 3, 4, 5 } ) | except( from( { 3, 4, 4 } ) ) | reverse();

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Except operates on a sorted range", "[narl][except][sorted]" )
{
	auto r = from( { 4, 3, 5, 2, 1, 0 } ) | sorted( []( int l, int r ) { return l < r; } ) | except( from( { 2, 4 } ) );

	REQUIRE( *r++ == 0 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 5 );
	REQUIRE( !r );
}


TEST_CASE( "Except operates on a sorted range in reverse", "[narl][except][sorted][reverse]" )
{
	auto r = from( { 4, 3, 5, 2, 1, 0 } ) | sorted( []( int l, int r ) { return l < r; } ) | except( from( { 2, 4 } ) ) | reverse();

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 0 );
	REQUIRE( !r );
}


TEST_CASE( "Except can be used with custom comparer", "[narl][except][comparer]" )
{
	auto r = from( { 5, 4, 3, 2, 2, 1 } ) | except( from( { 4, 4, 3 } ), []( int l, int r ) { return l > r; } );

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Intersect produces unique set of matched results", "[narl][intersect]" )
{
	auto r = from( { 2, 4, 4, 8, 16, 32, 32, 64, 128 } ) | intersect_with( from( { 1, 2, 3, 4, 5, 6, 7, 8 } ) );

	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 8 );
	REQUIRE( !r );
}


TEST_CASE( "Reversed intersect produces reversed list of matched results", "[narl][intersect][reverse]" )
{
	auto r = from( { 2, 4, 4, 8, 16, 32, 32, 64, 128 } ) | intersect_with( from( { 1, 2, 3, 4, 5, 6, 7, 8 } ) ) | reverse();

	REQUIRE( *r++ == 8 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 2 );
	REQUIRE( !r );
}


TEST_CASE( "Intersect operates with custom comparer", "[narl][intersect][comparer]" )
{
	auto r = from( { 128, 64, 32, 32, 16, 8, 4, 4, 2 } ) | intersect_with( from( { 8, 7, 6, 5, 4, 3, 2, 1 } ), []( int l, int r ) { return l > r; } );

	REQUIRE( *r++ == 8 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 2 );

	REQUIRE( !r );
}


TEST_CASE( "Union produces set of all results", "[narl][union]" )
{
	auto r = from( { 1, 2, 2, 3, 4, 5 } ) | union_with( from( { 2, 3, 3, 4, 5 } ) );

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 5 );
	REQUIRE( !r );
}


TEST_CASE( "Union operates with custom comparer", "[narl][union][comparer]" )
{
	auto r = from( { 5, 4, 3, 2, 2, 1 } ) | union_with( from( { 5, 4, 3, 3, 2 } ), []( int l, int r ) { return l > r; } );

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Reversed union produces reversed list of matched results", "[narl][union][reverse]" )
{
	auto r = from( { 1, 2, 2, 3, 4, 5 } ) | union_with( from( { 2, 3, 3, 4, 5 } ) ) | reverse();

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Groupby produces range grouped by key", "[narl][groupby]" )
{
	auto r = from( { 1, 1, 2 } ) | groupby( []( int i ) { return i; } );

	auto i = *r++;
	auto v = i.values;
	REQUIRE( i.key == 1 );
	REQUIRE( *v++ == 1 );
	REQUIRE( *v++ == 1 );
	REQUIRE( !v );
	
	i = *r++;
	v = i.values;
	REQUIRE( i.key == 2 );
	REQUIRE( *v++ == 2 );
	REQUIRE( !v );
	REQUIRE( !r );
}


TEST_CASE( "Zipwith single rhs returns tuple of 2 items", "[narl][zipwith][2]" )
{
	auto r = from( { 1, 2, 3 } ) | zipwith( from( { 10, 20, 30 } ) );

	auto v = *r;
	CAPTURE( std::get< 0 >( v ) );
	CAPTURE( std::get< 1 >( v ) );
	REQUIRE( *r++ == std::make_tuple( 1, 10 ) );
	REQUIRE( *r++ == std::make_tuple( 2, 20 ) );
	REQUIRE( *r++ == std::make_tuple( 3, 30 ) );

	REQUIRE( !r );
}

#ifndef _MSC_VER

TEST_CASE( "Zipwith two rhs returns tuple of 3 items", "[narl][zipwith][3]" )
{
	auto r = from( { 1, 2, 3 } ) | zipwith( from( { "10", "20", "30" } ), from( { 100, 200, 300 } ) );

	REQUIRE( *r++ == std::make_tuple( 1, "10", 100 ) );
	REQUIRE( *r++ == std::make_tuple( 2, "20", 200 ) );
	REQUIRE( *r++ == std::make_tuple( 3, "30", 300 ) );

	REQUIRE( !r );
}


TEST_CASE( "Zipwith three rhs returns tuple of 4 items", "[narl][zipwith][4]" )
{
	auto r = from( { 1, 2, 3 } ) | zipwith( from( { 10, 20, 30 } ), from( { std::string( "100" ), std::string( "200" ), std::string( "300" ) } ), from( { 1000, 2000, 3000 } ) );

	REQUIRE( *r++ == std::make_tuple( 1, 10, std::string( "100" ), 1000 ) );
	REQUIRE( *r++ == std::make_tuple( 2, 20, std::string( "200" ), 2000 ) );
	REQUIRE( *r++ == std::make_tuple( 3, 30, std::string( "300" ), 3000 ) );

	REQUIRE( !r );
}

#endif

TEST_CASE( "Aggregate produces reduction value of a range given a seed as starting point", "[narl][aggregate][withseed]" )
{
	auto r = from( { 2, 3, 4 } ) | aggregate( 2, []( int c, int i ) { return c * i; } );
	REQUIRE( r == 48 );
}


TEST_CASE( "Aggregate produces reduction value of a range using first element as starting point", "[narl][aggregate][noseed]" )
{
	auto r = from( { 2, 3, 4 } ) | aggregate( []( int c, int i ) { return c * i; } );
	REQUIRE( r == 24 );
}


TEST_CASE( "Count produces the number of elements in a collection", "[narl][count]" )
{
	auto r = from( { 1, 2, 3 } ) | count();
	REQUIRE( r == 3 );
}


TEST_CASE( "Count produces zero for empty range", "[narl][count][empty]" )
{
	std::vector< int > src;
	auto r = from( src ) | count();
	REQUIRE( r == 0 );
}


TEST_CASE( "Count produces zero for invalid range", "[narl][count][empty]" )
{
	auto r = from( { 1 } );
	++r;
	REQUIRE( ( r | count() ) == 0 );
}


TEST_CASE( "Count of a filtered range only counts matches", "[narl][count][where]" )
{
	auto r = from( { 1, 2, 3, 4, 5 } ) | where( []( int i ) { return i % 2 != 0; } ) | count();
	REQUIRE( r == 3 );
}

TEST_CASE( "Count of a distinct range only counts non duplicates", "[narl][distinct][count]" )
{
	auto r = from( { 1, 1, 2, 3, 3, 4, 5, 5 } ) | distinct() | reverse() | count();

	REQUIRE( r == 5 );
}


TEST_CASE( "Range can be make distinct", "[narl][distinct]" )
{
	auto r = from( { 1, 1, 2, 3, 3, 4, 5, 5 } ) | distinct();

	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 5 );
	REQUIRE( !r );
}


TEST_CASE( "Distinct range can be reversed", "[narl][distinct][reverse]" )
{
	auto r = from( { 1, 1, 2, 3, 3, 4, 5, 5 } ) | distinct() | reverse();

	REQUIRE( *r++ == 5 );
	REQUIRE( *r++ == 4 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}


TEST_CASE( "Ranges can be joined to produce new types", "[narl][join]" )
{
	auto r1 = from( { 0, 1, 2, 3, 4 } ) | zipwith( from( { "Zero", "One", "Two", "Three", "Four" } ) );
	auto r2 = from( { 1, 2, 3, 4, 5 } ) | zipwith( from( { 10.1, 20.2, 30.3, 40.4, 50.5 } ) ) ;

	//typedef std::tuple< int, std::string > left;
	//typedef std::tuple< int, double > right;
	//typedef std::tuple< int, std::string, double > result;

	//auto r = r1 | join( r2, 
	//	[]( const left & l ) { return std::get< 0 >( l ); }, 
	//	[]( const right & r ) { return std::get< 0 >( r ); }, 
	//	[]( const left & l, const right & r ) 
	//		{ return std::make_tuple( std::get< 0 >( l ), std::get< 1 >( l ), std::get< 1 >( r ) ); } );

	//std::vector< result > expected
	//	{
	//		std::make_tuple( 1, "One", 10.1 ),
	//		std::make_tuple( 2, "Two", 20.2 ),
	//		std::make_tuple( 3, "Three", 30.3 ),
	//		std::make_tuple( 4, "Four", 40.4 ),
	//	};
	//for( const auto & v : expected )
	//{
	//	REQUIRE( v == *r++ );
	//}
	//REQUIRE( !r );
}


TEST_CASE( "Range can be used as default constructed local", "[narl][range][local]" )
{
	range< int > r;

	REQUIRE( !r );

	r = from( { 1, 2, 3 } );
	REQUIRE( !!r );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( !r );
}

#ifndef _MSC_VER

TEST_CASE( "Range can be assigned to expression range", "[narl][range][local][expression]" )
{
	range< int > r;
	auto src = from( { 1, 2, 3, 4, 5 } ) | where( []( int i ) { return i <= 3; } ) | reverse();

	r = src;

	REQUIRE( !!r );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 1 );
	REQUIRE( !r );
}

TEST_CASE( "Range can participate in further expressions", "[narl][range][local][source]" )
{
	range< int > tmp;
	auto src = from( { 1, 2, 3, 4, 5 } ) | where( []( int i ) { return i >= 3; } );

	tmp = src;

	auto r = tmp | select( []( int i ) { return i * 2; } ) | reverse();

	REQUIRE( !!r );
	REQUIRE( *r++ == 10 );
	REQUIRE( *r++ == 8 );
	REQUIRE( *r++ == 6 );
	REQUIRE( !r );
}

#endif


TEST_CASE( "Range can be copied and re-assigned to other range", "[narl][range][copy][assign]" )
{
	range< int > r1 = from( { 1, 2, 3 } );
	range< int > r2 = r1;

	REQUIRE( *r1 == *r2++ );
	REQUIRE( *r1 != *r2 );
	r2 = range< int >{};
	REQUIRE( !r2 );
	REQUIRE( !!r1 );
}


TEST_CASE( "Range can be used to create a list", "[narl][range][container][list]" )
{
	std::vector< int > src { 1, 2, 3 };
	auto r = from( src );

	auto v = r | to< std::list >();

	REQUIRE( std::equal( std::begin( src ), std::end( src ), std::begin( v ) ) );
}


TEST_CASE( "Merge produces a sorted range from 2 input ranges", "[narl][merge]" )
{
	auto r = from( { 1, 2, 3 } ) | merge_with( from( { 0, 2, 4 } ) );

	REQUIRE( *r++ == 0 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 2 );
	REQUIRE( *r++ == 3 );
	REQUIRE( *r++ == 4 );
	REQUIRE( !r );
}


