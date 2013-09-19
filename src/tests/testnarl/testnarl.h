#pragma once


#include "throwing_range.h"


template< template< typename, typename... > class range_type, typename under_range_type, typename... predicate >
auto make_test_range( under_range_type r, predicate... filter ) -> range_type< under_range_type, predicate... >
{
	return range_type< under_range_type, predicate... >{ r, filter... };
}


template< template< typename > class range_type, typename under_range_type >
auto make_test_range( under_range_type r, int count ) -> range_type< under_range_type >
{
	return range_type< under_range_type >{ r, count };
}


