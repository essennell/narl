#include <range_factory.h>

#include <catch.hpp>


using namespace narl;


template< typename R >
struct dummy_range_1
{
	dummy_range_1( R r, int n ) : r{ r }, n{ n } { }
	R r;
	int n;
};

template< typename R, typename T >
struct dummy_range_2
{
	dummy_range_2( R r, T t ) : r{ r }, t{ t } { }
	R r;
	T t;
};


TEST_CASE( "Factory object creates unary range type with provided expression", "[narl][range_factory][unary]" )
{
	range_1_factory< dummy_range_1, int > factory{ 10 };
	auto result = factory( 2 );

	int underlying = 2;
	int expected = 10;
	REQUIRE( result.r == underlying );
	REQUIRE( result.n == expected );
}


TEST_CASE( "Factory object creates selected range type with provided expression", "[narl][range_factory][binary]" )
{
	range_2_factory< dummy_range_2, int > factory{ 10 };
	auto result = factory( 2 );

	int underlying = 2;
	int expected = 10;
	REQUIRE( result.r == underlying );
	REQUIRE( result.t == expected );
}


TEST_CASE( "Factory function creates unary factory object of correct type", "[narl][make_factory][unary]" )
{
	auto factory = make_factory< dummy_range_1 >( 10 );
	auto result = factory( 2 );

	int underlying = 2;
	int expected = 10;
	REQUIRE( result.r == underlying );
	REQUIRE( result.n == expected );
}


TEST_CASE( "Factory function creates binary factory object of correct type", "[narl][make_factory][binary]" )
{
	auto factory = make_factory< dummy_range_2 >( 10 );
	auto result = factory( 2 );

	int underlying = 2;
	int expected = 10;
	REQUIRE( result.r == underlying );
	REQUIRE( result.t == expected );
}



