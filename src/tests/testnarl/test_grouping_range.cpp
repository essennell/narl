#include "testnarl.h"

#include <grouping_range.h>
#include <range_input_iterator.h>

#include <catch.hpp>

#include <string>


using namespace narl;


struct item
{
	int id;
	std::string name;
	double val;
};

TEST_CASE( "Grouping range is evaluated lazily", "[narl][grouping][lazy]" )
{
	auto r = make_test_range< grouping_range >( throwing_range< int >{}, []( int ) { return 1; } );

	REQUIRE_THROWS_AS( ++r, range_access_exception );
	REQUIRE_THROWS_AS( r++, range_access_exception );
	REQUIRE_THROWS_AS( *r, range_access_exception );
	REQUIRE_THROWS_AS( !r, range_access_exception );
}


TEST_CASE( "Grouping range can be incremented to yield next value", "[narl][grouping][increment]" )
{
	std::vector< item > src { 
		item{ 0, "0", 0.0 }, 
		item{ 1, "1", 1.1 } 
	};
	auto r = make_test_range< grouping_range >( from( src ), []( item i ) { return i.id; } );

	REQUIRE( ( *r ).key == 0 );
	++r;
	REQUIRE( ( *r ).key == 1 );
}


TEST_CASE( "Grouping range can be post-incremented to yield next value", "[narl][grouping][postincrement" )
{
	std::vector< item > src { 
		item{ 0, "0", 0.0 }, 
		item{ 1, "1", 1.1 } 
	};
	auto r = make_test_range< grouping_range >( from( src ), []( item i ) { return i.id; } );

	REQUIRE( ( *r++ ).key == 0 );
	REQUIRE( ( *r++ ).key == 1 );
}


TEST_CASE( "Grouping range can be decremented to yield previous value", "[narl][grouping][decrement]" )
{
	std::vector< item > src { 
		item{ 0, "0", 0.0 }, 
		item{ 1, "1", 1.1 } 
	};
	auto r = make_test_range< grouping_range >( from( src ), []( item i ) { return i.id; } );
	++r;

	REQUIRE( ( *r ).key == 1 );
	--r;
	REQUIRE( ( *r ).key == 0 );
}


TEST_CASE( "Grouping range can be post-decremented to yield previous value", "[narl][grouping][postdecrement" )
{
	std::vector< item > src { 
		item{ 0, "0", 0.0 }, 
		item{ 1, "1", 1.1 } 
	};
	auto r = make_test_range< grouping_range >( from( src ), []( item i ) { return i.id; } );
	++r;
	REQUIRE( ( *r-- ).key == 1 );
	REQUIRE( ( *r-- ).key == 0 );
}


TEST_CASE( "Grouping range can be moved to end and is invalid", "[narl][grouping][goto_end][invalid]" )
{
	std::vector< item > src { 
		item{ 0, "0", 0.0 }, 
		item{ 1, "1", 1.1 } 
	};
	auto r = make_test_range< grouping_range >( from( src ), []( item i ) { return i.id; } );
	r.goto_end();
	REQUIRE( !r );
}


TEST_CASE( "Grouping range is invalid when before begin", "[narl][grouping][invalid][beforebegin]" )
{
	std::vector< item > src { 
		item{ 0, "0", 0.0 }, 
		item{ 1, "1", 1.1 } 
	};
	auto r = make_test_range< grouping_range >( from( src ), []( item i ) { return i.id; } );
	--r;
	REQUIRE( !r );
}


TEST_CASE( "Grouping range can be incremented from before begin", "[narl][grouping][valid][beforebegin][increment]" )
{
	std::vector< item > src { 
		item{ 0, "0", 0.0 }, 
		item{ 1, "1", 1.1 } 
	};
	auto r = make_test_range< grouping_range >( from( src ), []( item i ) { return i.id; } );
	--r;
	REQUIRE( !r );
	++r;
	REQUIRE( !!r );
}


TEST_CASE( "Grouping range can be decremented from the end", "[narl][grouping][valid][end][decrement]" )
{
	std::vector< item > src { 
		item{ 0, "0", 0.0 }, 
		item{ 1, "1", 1.1 } 
	};
	auto r = make_test_range< grouping_range >( from( src ), []( item i ) { return i.id; } );
	r.goto_end();
	--r;
	REQUIRE( !!r );
}


TEST_CASE( "Grouping range groups items according to provided key selector", "[narl][grouping][key]" )
{
	std::vector< item > src {
		item{ 0, "0", 0.0 },
		item{ 0, "00", 0.1 },
		item{ 1, "1", 1.0 }
	};
	auto r = make_test_range< grouping_range >( from( src ), []( const item & i ) { return i.id; } );

	auto i = *r++;
	auto v = i.values;
	REQUIRE( i.key == 0 );
	REQUIRE( ( *v++ ).name == "0" );
	REQUIRE( ( *v++ ).name == "00" );
	REQUIRE( !v );

	i = *r++;
	v = i.values;
	REQUIRE( i.key == 1 );
	REQUIRE( ( *v++ ).name == "1" );
	REQUIRE( !v );
}


TEST_CASE( "Grouping range can be keyed on any field", "[narl][grouping][key][any]" )
{
	std::vector< item > src {
		item{ 0, "0", 0.0 },
		item{ 1, "1", 0.1 },
		item{ 2, "1", 1.0 }
	};
	auto r = make_test_range< grouping_range >( from( src ), []( const item & i ) { return i.name; } );

	auto i = *r++;
	auto v = i.values;
	REQUIRE( i.key == "0" );
	REQUIRE( ( *v++ ).val == 0.0 );
	REQUIRE( !v );

	auto j = *r++;
	v = j.values;
	REQUIRE( j.key == "1" );
	REQUIRE( ( *v++ ).val == 0.1 );
	REQUIRE( ( *v++ ).val == 1.0 );
	REQUIRE( !v );
}


