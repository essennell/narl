#include "narl.h"

#include <catch.hpp>

#include <algorithm>
#include <numeric>


using namespace narl;


TEST_CASE( "Not implemented", "[narl][stl]" )
{
	INFO( "find_end" );
	INFO( "adjacent_find" );
	WARN( "search" );
	WARN( "search_n" );
	WARN( "replace" );
	WARN( "replace_if" );
	WARN( "replace_copy" );
	WARN( "replace_copy_if" );
	WARN( "swap" );
	WARN( "swap_ranges" );
	WARN( "iter_swap" );
	WARN( "rotate" );
	WARN( "rotate_copy" );
	WARN( "random_shuffle" );
	WARN( "shuffle" );
	WARN( "is_partitioned" );
	WARN( "partition" );
	WARN( "partition_copy" );
	WARN( "stable_partition" );
	WARN( "partition_point" );
	WARN( "nth_element" );
	WARN( "binary_search" );
	WARN( "equal_range" );
	WARN( "inplace_merge" );
	WARN( "includes" );
	WARN( "is_heap" );
	WARN( "is_heap_until" );
	WARN( "make_heap" );
	WARN( "push_heap" );
	WARN( "pop_heap" );
	WARN( "sort_heap" );
	WARN( "max" );
	WARN( "min" );
	WARN( "minmax" );
	WARN( "minmax_element" );
	WARN( "lexicographical_compare" );
	WARN( "is_permutation" );
	WARN( "next_permutation" );
	WARN( "prev_permutation" );
	WARN( "inner_product" );
	WARN( "adjacent_difference" );
	WARN( "partial_sum" );
}


TEST_CASE( "all_of", "[narl][stl][all_of]" )
{
	std::vector< int > data { 2, 4, 6 };

	auto stl_result = std::all_of( std::begin( data ), std::end( data ), []( int i ) { return i % 2 == 0; } );

	auto narl_result = from( data ) | all( []( int i ) { return i % 2 == 0; } );

	REQUIRE( narl_result == stl_result );
}


TEST_CASE( "any_of", "[narl][stl][any_of]" )
{
	std::vector< int > data { 2, 4, 6 };

	auto stl_result = std::any_of( std::begin( data ), std::end( data ), []( int i ) { return i == 4; } );

	auto narl_result = from( data ) | any( []( int i ) { return i == 4; } );

	REQUIRE( narl_result == stl_result );
}


TEST_CASE( "none_of", "[narl][stl][none_of]" )
{
	std::vector< int > data { 2, 4, 6 };

	auto stl_result = std::none_of( std::begin( data ), std::end( data ), []( int i ) { return i == 3; } );

	auto narl_result = !( from( data ) | any( []( int i ) { return i == 3; } ) );

	REQUIRE( narl_result == stl_result );
}


TEST_CASE( "count", "[narl][stl][count]" )
{
	std::vector< int > data { 2, 4, 6 };

	auto stl_result = std::count( std::begin( data ), std::end( data ), 4 );

	auto narl_result = from( data ) 
		| where( []( int i ) { return i == 4; } ) 
		| count();

	REQUIRE( narl_result == stl_result );
}


TEST_CASE( "count_if", "[narl][stl][count_if]" )
{
	std::vector< int > data { 2, 4, 6 };

	auto stl_result = std::count_if( std::begin( data ), std::end( data ), []( int i ) { return i < 5; } );

	auto narl_result = from( data ) 
		| where( []( int i ) { return i < 5; } ) 
		| count();

	REQUIRE( narl_result == stl_result );
}


TEST_CASE( "mismatch", "[narl][stl][mismatch]" )
{
	std::vector< int > left { 1, 2, 3 };
	std::vector< int > right { 1, 3, 5 };

	auto stl_result = std::mismatch( std::begin( left ), std::end( left ), std::begin( right ) );

	auto narl_result = from( left ) 
		| zipwith( from( right ) ) 
		| skip_while( []( const std::tuple< int, int > & v ) { return std::get< 0 >( v ) == std::get< 1 >( v ); } );

	REQUIRE( std::get< 0 >( *narl_result ) == *stl_result.first );
	REQUIRE( std::get< 1 >( *narl_result ) == *stl_result.second );
}


TEST_CASE( "mismatch predicate", "[narl][stl][mismatch][predicate]" )
{
	std::vector< int > left { 1, 2, 3 };
	std::vector< int > right { 1, 3, 5 };

	auto stl_result = std::mismatch( std::begin( left ), std::end( left ), std::begin( right ), 
		[]( int l, int r ) { return l == r; } );

	auto narl_result = from( left ) 
		| zipwith( from( right ) ) 
		| skip_while( []( const std::tuple< int, int > & v ) { return std::get< 0 >( v ) == std::get< 1 >( v ); } );

	REQUIRE( std::get< 0 >( *narl_result ) == *stl_result.first );
	REQUIRE( std::get< 1 >( *narl_result ) == *stl_result.second );
}


TEST_CASE( "equal", "[narl][stl][equal]" )
{
	std::vector< int > left { 1, 2, 3 };
	std::vector< int > right { 1, 2, 3 };

	auto stl_result = std::equal( std::begin( left ), std::end( left ), std::begin( right ) );

	auto narl_result = from( left ) | sequence_equal( from( right ) );

	REQUIRE( narl_result == stl_result );
}


TEST_CASE( "equal predicate", "[narl][stl][equal][predicate]" )
{
	std::vector< int > left { 1, 2, 3 };
	std::vector< int > right { 1, 0, 3 };

	auto stl_result = std::equal( std::begin( left ), std::end( left ), std::begin( right ),
		[]( int l, int r ) { return l == r; } );

	auto narl_result = from( left ) | sequence_equal( from( right ), []( int l, int r ) { return l == r; } );

	REQUIRE( narl_result == stl_result );
}


TEST_CASE( "find", "[narl][stl][find]" )
{
	std::vector< int > data { 1, 2, 3 };

	auto stl_result = std::find( std::begin( data ), std::end( data ), 2 );

	auto narl_result = from( data ) | skip_while( []( int i ) { return i != 2; } );

	REQUIRE( *narl_result == *stl_result );
}


TEST_CASE( "find_if", "[narl][stl][find_if]" )
{
	std::vector< int > data { 1, 2, 3 };

	auto stl_result = std::find_if( std::begin( data ), std::end( data ), []( int i ) { return i == 2; } );

	auto narl_result = from( data ) | skip_while( []( int i ) { return i != 2; } );

	REQUIRE( *narl_result == *stl_result );
}


TEST_CASE( "find_if_not", "[narl][stl][find_if_not]" )
{
	std::vector< int > data { 1, 2, 3 };

	auto stl_result = std::find_if_not( std::begin( data ), std::end( data ), []( int i ) { return i != 2; } );

	auto narl_result = from( data ) | skip_while( []( int i ) { return i != 2; } );

	REQUIRE( *narl_result == *stl_result );
}


TEST_CASE( "copy", "[narl][stl][copy]" )
{
	std::vector< int > data { 1, 2, 3 };
	std::vector< int > stl_result;
	std::vector< int > narl_result;

	std::copy( std::begin( data ), std::end( data ), std::back_inserter( stl_result ) );

	auto r = from( data );
	std::copy( begin( r ), end( r ), std::back_inserter( narl_result ) );

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "copy_if", "[narl][stl][copy_if]" )
{
	std::vector< int > data { 1, 2, 3 };
	std::vector< int > stl_result;

	std::copy_if( std::begin( data ), std::end( data ), std::back_inserter( stl_result ), []( int i ) { return i != 2; } );

	auto narl_result = from( data )
		| where( []( int i ) { return i != 2; } ) 
		| to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "copy_n", "[narl][stl][copy_n]" )
{
	std::vector< int > data { 1, 2, 3, 4, 5, 6 };
	std::vector< int > stl_result;

	std::copy_n( std::begin( data ), 3, std::back_inserter( stl_result ) );

	auto narl_result = from( data ) 
		| take( 3 ) 
		| to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "copy_backward", "[narl][stl][copy_backward]" )
{
	std::vector< int > data { 1, 2, 3 };
	std::vector< int > stl_result( 5 );
	std::vector< int > narl_result( 5 );

	std::copy_backward( std::begin( data ), std::end( data ), std::end( stl_result ) );

	auto r = from( data ) | reverse();
	std::copy( begin( r ), end( r ), narl_result.rbegin() );

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "fill", "[narl][stl][fill]" )
{
	std::vector< int > stl_result( 5 );
	std::fill( std::begin( stl_result ), std::end( stl_result ), 101 );

	auto narl_result = fill_range< int >( 101 ) 
		| take( 5 ) 
		| to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "fill_n", "[narl][stl][fill_n]" )
{
	std::vector< int > stl_result( 5 );
	std::fill_n( std::begin( stl_result ), 2, 101 );

	auto narl_result = fill_range< int >( 101 ) 
		| take( 2 ) 
		| concat( fill_range< int >( 0 ) | take( 3 ) ) 
		| to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "transform", "[narl][stl][transform]" )
{
	std::vector< int > data { 1, 2, 3 };
	std::vector< int > stl_result;

	std::transform( std::begin( data ), std::end( data ), std::back_inserter( stl_result ), []( int i ) { return i * 10; } );

	auto narl_result = from( data ) 
		| select( []( int i ) { return i * 10; } )
		| to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "transform binary", "[narl][stl][transform][binary]" )
{
	std::vector< int > data { 1, 2, 3 };
	std::vector< int > stl_result;

	std::transform( std::begin( data ), std::end( data ), std::begin( data ), std::back_inserter( stl_result ), []( int l, int r ) { return l * r; } );

	auto narl_result = from( data ) 
		| zipwith( from( data ) ) 
		| select( []( const std::tuple< int, int > & v ) { return std::get< 0 >( v ) * std::get< 1 >( v ); } )
		| to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "generate", "[narl][stl][generate]" )
{
	std::vector< int > stl_result( 3 );

	std::generate( std::begin( stl_result ), std::end( stl_result ), [] { return 5; } );

	auto narl_result = fill_range< int >( 5 ) | take( 3 ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "generate_n", "[narl][stl][generate_n]" )
{
	std::vector< int > stl_result;

	std::generate_n( std::back_inserter( stl_result ), 3, []{ return 5; } );

	auto narl_result = fill_range< int >( 5 ) | take( 3 ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "remove", "[narl][stl][remove]" )
{
	std::vector< int > data { 1, 2, 3, 4 };
	std::vector< int > data2 { data };

	auto end = std::remove( std::begin( data ), std::end( data ), 3 );
	std::vector< int > stl_result { std::begin( data ), end };

	auto narl_result = from( data2 ) | where( []( int i ) { return i != 3; } ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "remove_if", "[narl][stl][remove_if]" )
{
	std::vector< int > data { 1, 2, 3, 4 };
	std::vector< int > data2 { data };

	auto end = std::remove_if( std::begin( data ), std::end( data ), []( int i ) { return i % 2 == 0; } );
	std::vector< int > stl_result { std::begin( data ), end };

	auto narl_result = from( data2 ) | where( []( int i ) { return i % 2 != 0; } ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "remove_copy", "[narl][stl][remove_copy]" )
{
	std::vector< int > data { 1, 2, 3, 4 };
	std::vector< int > stl_result;

	std::remove_copy( std::begin( data ), std::end( data ), std::back_inserter( stl_result ), 3 );

	auto narl_result = from( data ) | where( []( int i ) { return i != 3; } ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "remove_copy_if", "[narl][stl][remove_copy_if]" )
{
	std::vector< int > data { 1, 2, 3, 4 };
	std::vector< int > stl_result;

	std::remove_copy_if( std::begin( data ), std::end( data ), std::back_inserter( stl_result ), []( int i ) { return i % 2 == 0; } );

	auto narl_result = from( data ) | where( []( int i ) { return i % 2 != 0; } ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "reverse", "[narl][stl][reverse]" )
{
	std::vector< int > data { 1, 2, 3 };
	std::vector< int > stl_result{ data };
	
	std::reverse( std::begin( stl_result ), std::end( stl_result ) );

	auto narl_result = from( data ) | reverse() | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "reverse_copy", "[narl][stl][reverse_copy]" )
{
	std::vector< int > data { 1, 2, 3 };
	std::vector< int > stl_result;
	
	std::reverse_copy( std::begin( stl_result ), std::end( stl_result ), std::back_inserter( stl_result ) );

	auto narl_result = from( data ) | reverse() | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "unique", "[narl][stl][unique]" )
{
	std::vector< int > data { 1, 3, 3, 5 };
	std::vector< int > data2 { data };

	auto end = std::unique( std::begin( data ), std::end( data ) );
	std::vector< int > stl_result { std::begin( data ), end };

	auto narl_result = from( data ) | distinct() | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "unique predicate", "[narl][stl][unique][predicate]" )
{
	std::vector< int > data { 1, 3, 3, 5 };
	std::vector< int > data2 { data };

	auto end = std::unique( std::begin( data ), std::end( data ), std::equal_to< int >() );
	std::vector< int > stl_result { std::begin( data ), end };

	auto narl_result = from( data ) | distinct( std::equal_to< int >() ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "unique_copy", "[narl][stl][unique_copy]" )
{
	std::vector< int > data { 1, 3, 3, 5 };
	std::vector< int > data2 { data };
	std::vector< int > stl_result;

	std::unique_copy( std::begin( data ), std::end( data ), std::back_inserter( stl_result ) );

	auto narl_result = from( data ) | distinct() | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "unique_copy predicate", "[narl][stl][unique_copy][predicate]" )
{
	std::vector< int > data { 1, 3, 3, 5 };
	std::vector< int > data2 { data };
	std::vector< int > stl_result;

	std::unique_copy( std::begin( data ), std::end( data ), std::back_inserter( stl_result ), std::equal_to< int >() );

	auto narl_result = from( data ) | distinct( std::equal_to< int >() ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "lower_bound", "[narl][stl][lower_bound]" )
{
	std::vector< int > data { 1, 2, 4, 8, 16, 32, 64 };

	auto end = std::lower_bound( std::begin( data ), std::end( data ), 10 );
	std::vector< int > stl_result { std::begin( data ), end };

	auto narl_result = from( data ) | take_while( []( int i ) { return i < 10; } ) | to< std::vector >();

	WARN( "NB: More costly than STL operation - linear not logarithmic" );
	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "lower_bound predicate", "[narl][stl][lower_bound][predicate]" )
{
	std::vector< int > data { 1, 2, 4, 8, 16, 32, 64 };

	auto end = std::lower_bound( std::begin( data ), std::end( data ), 10, []( int l, int r ) { return l < r; } );
	std::vector< int > stl_result { std::begin( data ), end };

	auto narl_result = from( data ) | take_while( []( int i ) { return i < 10; } ) | to< std::vector >();

	WARN( "NB: More costly than STL operation - linear not logarithmic" );
	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "upper_bound", "[narl][stl][upper_bound]" )
{
	std::vector< int > data { 1, 2, 4, 8, 16, 32, 64 };

	auto begin = std::upper_bound( std::begin( data ), std::end( data ), 10 );
	std::vector< int > stl_result { begin, std::end( data ) };

	auto narl_result = from( data ) | skip_while( []( int i ) { return i < 10; } ) | to< std::vector >();

	WARN( "NB: More costly than STL operation - linear not logarithmic" );
	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "upper_bound predicate", "[narl][stl][upper_bound][predicate]" )
{
	std::vector< int > data { 1, 2, 4, 8, 16, 32, 64 };

	auto begin = std::upper_bound( std::begin( data ), std::end( data ), 10, []( int l, int r ) { return l < r; } );
	std::vector< int > stl_result { begin, std::end( data ) };

	auto narl_result = from( data ) | skip_while( []( int i ) { return i < 10; } ) | to< std::vector >();

	WARN( "NB: More costly than STL operation - linear not logarithmic" );
	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "merge", "[narl][stl][merge]" )
{
	std::vector< int > left { 1, 2, 3, 4, 5 };
	std::vector< int > right { 0, 2, 4, 6 };

	std::vector< int > stl_result;
	std::merge( std::begin( left ), std::end( left ), std::begin( right ), std::end( right ), std::back_inserter( stl_result ) );

	auto narl_result = from( left ) | merge_with( from( right ) ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "sort", "[narl][stl][sort]" )
{
	std::vector< int > data { 3, 2, 4, 1, 5 };
	std::vector< int > stl_result { data };

	std::sort( std::begin( stl_result ), std::end( stl_result ) );

	auto narl_result = from( data ) | sorted() | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "sort comparer", "[narl][stl][sort][comparer]" )
{
	std::vector< int > data { 3, 2, 4, 1, 5 };
	std::vector< int > stl_result { data };

	std::sort( std::begin( stl_result ), std::end( stl_result ), std::less< int >() );

	auto narl_result = from( data ) | sorted( std::less< int >() ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "stable_sort", "[narl][stl][stable_sort]" )
{
	std::vector< int > data { 3, 2, 4, 1, 5 };
	std::vector< int > stl_result { data };

	std::stable_sort( std::begin( stl_result ), std::end( stl_result ) );

	auto narl_result = from( data ) | sorted() | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "merge comparer", "[narl][stl][merge][comparer]" )
{
	std::vector< int > left { 1, 2, 3, 4, 5 };
	std::vector< int > right { 0, 2, 4, 6 };

	std::vector< int > stl_result;
	std::merge( std::begin( left ), std::end( left ), std::begin( right ), std::end( right ), std::back_inserter( stl_result ), []( int l, int r ) { return l < r; } );

	auto narl_result = from( left ) | merge_with( from( right ), []( int l, int r ) { return l < r; } ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "set_difference", "[narl][stl][set_difference]" )
{
	std::vector< int > left { 1, 2, 5, 5, 5, 9 };
	std::vector< int > right { 2, 5, 7 };

	std::vector< int > stl_result;
	std::set_difference( std::begin( left ), std::end( left ), std::begin( right ), std::end( right ), std::back_inserter( stl_result ) );

	auto narl_result = from( left ) | except( from( right ) ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "set_intersection", "[narl][stl][set_intersection]" )
{
	std::vector< int > left { 1, 2, 2, 3, 4, 5 };
	std::vector< int > right { 0, 2, 4, 4, 6 };

	std::vector< int > stl_result;
	std::set_intersection( std::begin( left ), std::end( left ), std::begin( right ), std::end( right ), std::back_inserter( stl_result ) );

	auto narl_result = from( left ) | intersect_with( from( right ) ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "set_symmetric_difference", "[narl][stl][set_symmetric_difference]" )
{
	std::vector< int > left { 1, 2, 2, 3, 4, 5 };
	std::vector< int > right { 0, 2, 4, 4, 6 };

	std::vector< int > stl_result;
	std::set_symmetric_difference( std::begin( left ), std::end( left ), std::begin( right ), std::end( right ), std::back_inserter( stl_result ) );

	auto narl_result = from( left ) 
		| except( from( right ) ) 
		| union_with( from( right ) | except( from( left ) ) ) 
		| to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "set_union", "[narl][stl][set_union]" )
{
	std::vector< int > left { 1, 2, 2, 3, 4, 5 };
	std::vector< int > right { 0, 2, 4, 4, 6 };

	std::vector< int > stl_result;
	std::set_union( std::begin( left ), std::end( left ), std::begin( right ), std::end( right ), std::back_inserter( stl_result ) );

	auto narl_result = from( left ) | union_with( from( right ) ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "max_element", "[narl][stl][max_element]" )
{
	std::vector< int > data { 5, 3, 2, 1, 7, 6 };

	auto stl_result = std::max_element( std::begin( data ), std::end( data ) );

	auto narl_result = from( data ) | aggregate( []( int c, int n ) { return std::max( c, n ); } );

	REQUIRE( *stl_result == narl_result );
}


TEST_CASE( "min_element", "[narl][stl][min_element]" )
{
	std::vector< int > data { 5, 3, 2, 1, 7, 6 };

	auto stl_result = std::min_element( std::begin( data ), std::end( data ) );

	auto narl_result = from( data ) | aggregate( []( int c, int n ) { return std::min( c, n ); } );

	REQUIRE( *stl_result == narl_result );
}


TEST_CASE( "iota", "[narl][stl][iota]" )
{
	std::vector< int > stl_result( 5 );
	std::iota( std::begin( stl_result ), std::end( stl_result ), 5 );

	auto narl_result = make_range< int >( 5 ) | take( 5 ) | to< std::vector >();

	REQUIRE( std::equal( std::begin( stl_result ), std::end( stl_result ), std::begin( narl_result ) ) );
}


TEST_CASE( "accumulate", "[narl][stl][accumulate]" )
{
	std::vector< int > data { 2, 4, 8, 16 };

	auto stl_result = std::accumulate( std::begin( data ), std::end( data ), 1, []( int l, int r ) { return l + r; } );

	auto narl_result = from( data ) | aggregate( 1, []( int l, int r ) { return l + r; } );

	REQUIRE( stl_result == narl_result );
}


