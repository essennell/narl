#include <narl.h>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#ifndef _MSC_VER
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/algorithm/count.hpp>
#include <boost/range/algorithm/set_algorithm.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/range/numeric.hpp>

using namespace boost::adaptors;
#endif

#include <algorithm>
#include <chrono>
#include <iostream>
#include <list>
#include <numeric>
#include <tuple>

using namespace narl;


template< typename fn >
auto time_avg( fn f, size_t tries ) -> std::tuple< size_t, std::chrono::microseconds >
{
	double result;

	std::chrono::microseconds cumulation { 0 };
	for( auto i : narl::make_range< size_t >() | narl::take( tries ) )
	{
		auto start = std::chrono::high_resolution_clock::now();
		result = f();
		auto end = std::chrono::high_resolution_clock::now();
		auto res = std::chrono::duration_cast< std::chrono::milliseconds >( end - start );

		cumulation = ( cumulation * i + res ) / ( i + 1 );
	}

	return std::make_tuple( result, cumulation );
}


TEST_CASE( "Select where", "" )
{
	const int N = 10000000;
	const size_t tries = 3;

	auto items = make_range< int >() | select( []( int i ) { return i % 4 > 0 ? i * 2.3 : i * -3.2; } ) | take( N );
	auto l = items | to< std::vector >();

	auto stlres = time_avg( [&] 
	{ 
		std::vector< double > out;
		std::transform( begin( l ), 
			std::partition( begin( l ), end( l ), []( double d ) { return d < 0.0; } ), 
				std::inserter( out, out.begin() ), []( double d ) { return d * 0.175; } );
		return std::accumulate( out.begin(), out.end(), 0.0 );
	}, tries );
	auto stlnum = std::get< 0 >( stlres );
	auto stltime = std::get< 1 >( stlres ).count() / 1000.0;
	INFO( "STL      : " << std::setw( 7 ) << std::fixed << std::setprecision( 2 ) << stltime << "ms" );

#ifndef _MSC_VER
	auto boostres = time_avg( [&]
	{
		std::vector< double > out;
		return boost::accumulate( l
			| filtered( []( double d ) { return d < 0.0; } ) 
			| transformed( []( double d ) { return d * 0.175; } ),
			0.0 );
	}, tries );
	auto boostnum = std::get< 0 >( boostres );
	auto boosttime = std::get< 1 >( boostres ).count() / 1000.0;
	INFO( "BOOST    : " << std::setw( 7 ) << std::fixed << std::setprecision( 2 ) << boosttime << "ms" );
#else
	auto boostnum = stlnum;
	INFO( "BOOST    : " << std::setw( 7 ) << "xxx" );
#endif

	auto narlres = time_avg( [&] 
	{ 
		return from( l ) //items
			| where( []( double d ) { return d < 0.0; } ) 
			| select( []( double d ) { return d * 0.175; } ) 
			| aggregate( []( double l, double r ) { return l + r; } ); 
	}, tries );
	auto narlnum = std::get< 0 >( narlres );
	auto narltime = std::get< 1 >( narlres ).count() / 1000.0;

	INFO( "NARL     : " << std::setw( 7 ) << std::fixed << std::setprecision( 2 ) << narltime << "ms" );

	REQUIRE( ( narlnum == stlnum && narlnum == boostnum ) );
}

