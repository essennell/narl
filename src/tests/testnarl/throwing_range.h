#pragma once

#include <stdexcept>
#include <string>


class range_access_exception : public std::logic_error
{
	public:
		range_access_exception( const std::string & what ) : std::logic_error{ what } { }
};


template< typename T >
struct throwing_range
{
	throwing_range() { }
	throwing_range( const throwing_range & ) { }

	auto operator++() -> throwing_range & { throw range_access_exception( "Preincrement" ); }
	auto operator++( int ) -> throwing_range { throw range_access_exception( "Postincrement" ); }
	auto operator*() const -> T { throw range_access_exception( "Expression evaluation" ); }
	explicit operator bool() const { throw range_access_exception( "Validity" ); }
};

