#include <narl.h>

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/count.hpp>
#include <boost/range/algorithm/set_algorithm.hpp>

using namespace boost::adaptors;


#include <algorithm>
#include <chrono>
#include <deque>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <tuple>
#include <vector>


const int N = 1000000;
const size_t tries_for_avg = 3;


template< template< typename... > class container_type, typename left_range_type, typename right_range_type >
size_t std_intersect_full( left_range_type left, right_range_type right )
{
	container_type< int > output;
	std::set_intersection( begin( left ), end( left ), begin( right ), end( right ), 
		std::inserter( output, output.end() ) );
	return output.size();
}

template< template< typename... > class container_type, typename item_type >
size_t std_intersect_full( const container_type< item_type > & left, const container_type< item_type > & right )
{
	container_type< int > output;
	std::set_intersection( left.begin() , left.end(), right.begin(), right.end(), 
		std::inserter( output, output.end() ) );
	return output.size();
}


template< template< typename... > class container_type, typename item_type >
size_t boost_intersect_full( const container_type< item_type > & left, const container_type< item_type > & right )
{
	container_type< int > output;
	boost::set_intersection( left, right,
		std::inserter( output, output.end() ) );
	return output.size();
}


template< template< typename... > class container_type, typename left_range_type, typename right_range_type >
size_t std_union_full( left_range_type left, right_range_type right )
{
	container_type< int > output;
	std::set_union( begin( left ), end( left ), begin( right ), end( right ),
		std::inserter( output, output.end() ) );
	return output.size();
}

template< template< typename... > class container_type, typename item_type >
size_t std_union_full( const container_type< item_type > & left, const container_type< item_type > & right )
{
	container_type< int > output;
	std::set_union( left.begin() , left.end(), right.begin(), right.end(), 
		std::inserter( output, output.end() ) );
	return output.size();
}

template< template< typename... > class container_type, typename left_range_type, typename right_range_type >
size_t std_diff_full( left_range_type left, right_range_type right )
{
	container_type< int > output;
	std::set_difference( begin( left ), end( left ), begin( right ), end( right ),
		std::inserter( output, output.end() ) );
	return output.size();
}

template< template< typename... > class container_type, typename item_type >
size_t std_diff_full( const container_type< item_type > & left, const container_type< item_type > & right )
{
	container_type< int > output;
	std::set_difference( left.begin() , left.end(), right.begin(), right.end(), 
		std::inserter( output, output.end() ) );
	return output.size();
}


template< template< typename... > class container_type, typename item_type >
size_t std_search_replace( container_type< item_type > items )
{
	std::vector< double > result;

	std::transform( items.begin(), std::partition( items.begin(), items.end(), 
			[]( const item_type & v ) { return v > 0.0; } ), 
		std::inserter( result, result.end() ),
		[]( const item_type & v ) { return v + v * 0.175; } );
		
	return result.size();
}

template< template< typename... > class container_type, typename item_type >
size_t boost_search_replace( const container_type< item_type > & items )
{
	std::vector< double > result;
	boost::push_back( result, items
		| filtered( []( const item_type & v ) { return v > 0.0; } )
		| transformed( []( const item_type & v ) { return v + v * 0.175; } )
		);
	return result.size();
}


template< typename fn >
auto time_avg( fn f, size_t tries ) -> std::tuple< size_t, std::chrono::milliseconds >
{
	size_t result;

	std::chrono::milliseconds cumulation { 0 };
	for( auto i : narl::make_range< int >() | narl::take( tries ) )
	{
		auto start = std::chrono::high_resolution_clock::now();
		result = f();
		auto end = std::chrono::high_resolution_clock::now();
		auto res = std::chrono::duration_cast< std::chrono::milliseconds >( end - start );

		cumulation = ( cumulation * i + res ) / ( i + 1 );
	}

	return std::make_tuple( result, cumulation );
}

template< typename range >
void print_tests( const char * const testname, const range & items )
{
	std::cout << testname << " : " << N << " items" << std::endl;
	for( auto test : items )
	{
		auto name = std::get< 0 >( test );
		auto fn = std::get< 1 >( test );

		std::cout << " " << std::setw( 35 ) << std::left << name << ":" << std::right;
		auto result = time_avg( fn, tries_for_avg );

		auto count = std::get< 0 >( result );
		auto ms = std::get< 1 >( result );
		std::cout << std::setw( 10 ) << count << " items in " << std::setw( 6 ) << ms.count() << " ms" << std::endl;
	}
	std::cout << std::endl;
}


template< template< typename... > class container_type >
void run_select_tests( const char * const testname )
{
	using namespace narl;

	auto srv = make_range< int >() | select( []( int i ) { return i % 4 != 0 ? i * 2.3 : -( i * 3.2 ); } ) | take( N );
	auto srv_c = srv | to< container_type >();

	auto set_names = {
		"std search/replace",
		"boost search/replace",
		"narl search/replace",
		"narl search/replace range",
	};
	std::initializer_list< std::function< size_t() > > set_tests = {
		[&] { return std_search_replace( srv_c ); },
		[&] { return boost_search_replace( srv_c ); },
		[&] { return from( srv_c ) | where( []( double v ) { return v > 0.0; } ) | select( []( double v ) { return v + v * 0.175; } ) | count(); },
		[&] { return srv | where( []( double v ) { return v > 0.0; } ) | select( []( double v ) { return v + v * 0.175; } ) | count(); },
	};

	print_tests( testname, from( set_names ) | zipwith( from( set_tests ) ) );
}


template< template< typename ... > class container_type >
void run_set_tests( const char * const testname )
{
	using namespace narl;

	auto s = make_range< int > () | where( []( int i ) { return i % 3 == 0; } ) | take( N );
	auto o = make_range< int > () | where( []( int i ) { return i % 5 == 0; } ) | take( N );

	auto left_c = s | to< container_type >();
	auto right_c = o | to< container_type >();

	auto set_test_names = {
		"std diff extract",
		"nrl diff extract",
		"nrl diff count",
		"std diff collection",
		"nrl diff collection",
		"nrl diff collection count",
		"std intersect extract",
		"nrl intersect extract",
		"nrl intersect count",
		"std intersect collection",
		"nrl intersect collection",
		"nrl intersect collection count",
		"std union extract",
		"nrl union extract",
		"nrl union count",
		"std union collection",
		"nrl union collection",
		"nrl union collection count",
	};

	typedef std::function< size_t() > test_fn;
	std::initializer_list< test_fn > set_tests = {
		[&]{ return std_diff_full< container_type >( s, o ); },
		//[&]{ return ( s | except( o ) | to< container_type >() ).size(); },
		[&]{ auto r = s | except( o ); size_t count = 0; for( auto i : r ) { ++count; }; return count; },
		[&]{ return s | except( o ) | narl::count(); },
		[&]{ return std_diff_full< container_type, int >( left_c, right_c ); },
		[&]{ return ( from( left_c ) | except( from( right_c ) ) | to< container_type >() ).size(); },
		[&]{ return from( left_c ) | except( from( right_c ) ) | narl::count(); },
		[&]{ return std_intersect_full< container_type >( s, o ); },
		[&]{ return ( s | intersect_with( o ) | to< container_type >() ).size(); },
		[&]{ return s | intersect_with( o ) | narl::count(); },
		[&]{ return std_intersect_full< container_type, int >( left_c, right_c ); },
		[&]{ return ( from( left_c ) | intersect_with( from( right_c ) ) | to< container_type >() ).size(); },
		[&]{ return from( left_c ) | intersect_with( from( right_c ) ) | count(); },
		[&]{ return std_union_full< container_type >( s, o ); },
		[&]{ return ( s | union_with( o ) | to< container_type >() ).size(); },
		[&]{ return s | union_with( o ) | count(); },
		[&]{ return std_union_full< container_type, int >( left_c, right_c ); },
		[&]{ return ( from( left_c ) | union_with( from( right_c ) ) | to< container_type >() ).size(); },
		[&]{ return from( left_c ) | union_with( from( right_c ) ) | count(); },
	};

	print_tests( testname, from( set_test_names ) | zipwith( from( set_tests ) ) );
}

int main()
{
#ifndef _MSC_VER
	run_set_tests< std::set >   ( "std::set" );
#endif
	run_set_tests< std::vector >( "std::vector" );
	run_select_tests< std::list >( "std::list" );
	run_select_tests< std::vector >( "std::vector" );
	run_select_tests< std::deque >( "std::deque" );

	return 0;
}
