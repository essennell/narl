#pragma once


#include "throwing_range.h"


template< template< typename > class range_type, typename under_range_type >
auto make_test_range( under_range_type r ) -> range_type< under_range_type >
{
	return range_type< under_range_type >{ r };
}


template< template< typename... > class range_type, typename under_range_type, typename... predicate >
auto make_test_range( under_range_type r, predicate... filter ) -> range_type< under_range_type, predicate... >
{
	return range_type< under_range_type, predicate... >{ r, filter... };
}


#ifdef _MSC_VER

template< template< typename, typename, typename > class range_type, typename under_range_type, typename other_range_type, typename comparer >
auto make_test_range( under_range_type r, other_range_type o, comparer cmp ) -> range_type< under_range_type, other_range_type, comparer >
{
	return range_type< under_range_type, other_range_type, comparer >{ r, o, cmp };
}

template< template< typename, typename, typename, typename, typename > class range_type, typename under_range_type, typename other_range_type, typename inner_key, typename outer_key, typename selector >
auto make_test_range( under_range_type r, other_range_type o, inner_key inner, outer_key outer, selector trans ) -> range_type< under_range_type, other_range_type, inner_key, outer_key, selector >
{
	return range_type< under_range_type, other_range_type, inner_key, outer_key, selector >{ r, o, inner, outer, trans };
}

template< template< typename, typename, typename, typename, typename, typename > class range_type, typename under_range_type, typename other_range_type, typename inner_key, typename outer_key, typename selector, typename comparer >
auto make_test_range( under_range_type r, other_range_type o, inner_key inner, outer_key outer, selector trans, comparer cmp ) -> range_type< under_range_type, other_range_type, inner_key, outer_key, selector, comparer >
{
	return range_type< under_range_type, other_range_type, inner_key, outer_key, selector, comparer >{ r, o, inner, outer, trans, cmp };
}

#endif