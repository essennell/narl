#pragma once

#include "filtering_range.h"
#include "iterable_range.h"
#include "range_factory.h"

#include <functional>
#include <type_traits>


namespace narl
{

	template< typename range_type, typename expression >
	class range_predicate
	{

		public:
			range_predicate( const range_type & r, const expression & expr )
				: r{ filtering_range< range_type, expression >{ r, expr } }
			{
			}


			auto value() const -> bool
			{
				return !!r;
			}


		private:
			filtering_range< range_type, expression > r;

	};


	template< typename range_type, typename other_range_type, typename comparitor >
	class range_equality
	{

		public:
			range_equality( const range_type & r, const other_range_type & o, const comparitor & cmp )
				: r{ r }, o{ o }, cmp( cmp )
			{
			}


			auto value() const -> bool
			{
				while( r && o && cmp( *r, *o ) )
				{
					++r; 
					++o;
				}
				return !r && !o;
			}


		private:
			mutable range_type r;
			mutable other_range_type o;
			comparitor cmp;

	};

	
	template< typename range_type, typename other_range_type >
	class range_equality_default : public range_equality< range_type, other_range_type, std::equal_to< decltype( *std::declval< range_type >() ) > >
	{

		public:
			range_equality_default( const range_type & r, const other_range_type & o )
				: range_equality< range_type, other_range_type, std::equal_to< decltype( *r ) > >( r, o, std::equal_to< decltype( *r ) >() )
			{
			}

	};


	template< typename range_type >
	class range_validator
	{

		public:
			range_validator( const range_type & r )
				: r{ r }
			{
			}


			auto value() const -> bool
			{
				return !!r;
			}


		private:
			range_type r;

	};


	template< typename range_type, typename expression >
	class range_predicate_inverter
	{

		public:
			range_predicate_inverter( const range_type r, const expression & expr )
				: r{ filtering_range< range_type, negate >{ r, negate( r, expr ) } }
			{
			}


			auto value() const -> bool
			{
				return !r;
			}


		private:
			struct negate
			{
				range_type lr;
				expression expr;
				negate( const range_type & lr, const expression & expr ) 
					: lr{ lr }, expr( expr ) 
					{ }
				bool operator()( const decltype( *std::declval< range_type >() ) & v ) const 
					{ return !expr( v ); }
			};

			filtering_range< range_type, negate > r;

	};


	inline auto any() -> decltype( make_factory< range_validator >() )
	{
		return make_factory< range_validator >();
	}

	template< typename expression >
	auto any( const expression & expr ) -> decltype( make_factory< range_predicate >( expr ) )
	{
		return make_factory< range_predicate >( expr );
	}
	
	template< typename other_range_type, typename comparitor >
	auto sequence_equal( const other_range_type & o, const comparitor & cmp )
		-> decltype( make_factory< range_equality >( o, cmp ) )
	{
		return make_factory< range_equality >( o, cmp );
	}

	template< typename other_range_type >
	auto sequence_equal( const other_range_type & o )
		-> decltype( make_factory< range_equality_default >( o ) )
	{
		return make_factory< range_equality_default >( o );
	}

	template< typename expression >
	auto all( const expression & expr ) -> decltype( make_factory< range_predicate_inverter >( expr ) )
	{
		return make_factory< range_predicate_inverter >( expr );
	}

}
