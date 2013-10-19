#pragma once

#include "concatenating_range.h"
#include "diffing_range.h"
#include "distinct_range.h"
#include "filtering_range.h"
#include "grouping_range.h"
#include "intersecting_range.h"
#include "iterable_range.h"
#include "joined_range.h"
#include "merging_range.h"
#include "partial_range.h"
#include "range.h"
#include "range_accumulate.h"
#include "range_factory.h"
#include "range_input_iterator.h"
#include "range_generator.h"
#include "range_predicate.h"
#include "range_tocontainer.h"
#include "reversed_range.h"
#include "selectmany_range.h"
#include "sorted_range.h"
#include "transforming_range.h"
#include "unioning_range.h"
#include "zipping_range.h"


namespace narl
{

	template< typename range_type, typename factory_type >
	auto operator|( const range_type & r, const factory_type & factory )
		-> decltype( factory( r ) )
	{
		return factory( r );
	}


	template< typename range_type >
	auto operator|( const range_type & r, const range_0_factory< range_validator > & v ) -> bool
	{
		auto result = v( r );
		return result.value();
	}

	template< typename range_type, typename argument_type >
	auto operator|( const range_type & r, const range_2_factory< range_predicate, argument_type > & v ) -> bool
	{
		auto result = v( r );
		return result.value();
	}

	template< typename range_type, typename argument_type >
	auto operator|( const range_type & r, const range_2_factory< range_predicate_inverter, argument_type > & v ) -> bool
	{
		auto result = v( r );
		return result.value();
	}

	template< typename range_type, typename other_range_type >
	auto operator|( const range_type & r, const range_2_factory< range_equality_default, other_range_type > & v ) -> bool
	{
		auto result = v( r );
		return result.value();
	}


#ifndef _MSC_VER

	template< typename range_type, typename other_range_type, typename comparitor >
	auto operator|( const range_type & r, const range_N_factory< range_equality, other_range_type, comparitor > & v ) -> bool
	{
		auto result = v( r );
		return result.value();
	}

	template< typename range_type, typename argument_type, typename accumulator >
	auto operator|( const range_type & r, const range_N_factory< range_accumulate, argument_type, accumulator > & v ) -> decltype( v( r ).value() )
	{
		auto result = v( r );
		return result.value();
	}

#else

	template< typename range_type, typename other_range_type, typename comparitor >
	auto operator|( const range_type & r, const range_3_factory< range_equality, other_range_type, comparitor > & v ) -> bool
	{
		auto result = v( r );
		return result.value();
	}

	template< typename range_type, typename argument_type, typename accumulator >
	auto operator|( const range_type & r, const range_3_factory< range_accumulate, argument_type, accumulator > & v ) -> decltype( v( r ).value() )
	{
		auto result = v( r );
		return result.value();
	}

#endif

	template< typename range_type, typename accumulator >
	auto operator|( const range_type & r, const range_2_factory< range_default_accumulate, accumulator > & v ) -> decltype( v( r ).value() )
	{
		auto result = v( r );
		return result.value();
	}

	template< typename range_type >
	auto operator|( const range_type & r, const range_0_factory< range_counter > & v ) -> decltype( v( r ).value() )
	{
		auto result = v( r );
		return result.value();
	}

}


