#include <narl.h>

#include <catch.hpp>

#include <type_traits>


using namespace narl;


struct generator
{
	generator() {}
};

TEST_CASE( "Range generator can be instantiated from int", "[narl][range_generator][int]" )
{
	auto r = range_generator< int >{};
	REQUIRE( !!r );
}


TEST_CASE( "Range generator produces expected value on dereference", "[narl][range_generator][int][dereference]" )
{
	auto r = make_range< int >();
	REQUIRE( *r == 0 );
}


TEST_CASE( "Range generator produces next value after postincrement", "[narl][range_generator][int][postinc]" )
{
	auto r = range_generator< int >{};
	REQUIRE( *r++ == 0 );
	REQUIRE( *r++ == 1 );
	REQUIRE( *r++ == 2 );
}


TEST_CASE( "Range generator produces next value after preincrement", "[narl][range_generator][int][preinc]" )
{
	auto r = range_generator< int >{};
	++r;
	REQUIRE( *r == 1 );
	++r;
	REQUIRE( *r == 2 );
}


TEST_CASE( "Range generator can be instantiated from user type", "[narl][range_generator][userdef]" )
{
	auto r = range_generator< int >{};
	REQUIRE( !!r );
}


TEST_CASE( "Range generator from user type produces correct type on dereference", "[narl][range_generator][userdef][dereferencetype]" )
{
	auto r = range_generator< generator >{};
	REQUIRE( ( std::is_same< decltype( *r ), generator >() ) );
}



