#pragma once

#include "range_factory.h"

#include <type_traits>


namespace narl
{

	template< typename range_type, typename other_range, typename comparer >
	class unioning_range
	{
		
		private:
			mutable range_type r;
			mutable other_range o;
			comparer cmp;

			
		public:
			unioning_range( const range_type & r, const other_range & o, const comparer & cmp )
				: r{ r }, o{ o }, cmp( cmp )
			{
			}


			auto operator*() const -> decltype( *r )
			{
				if( r && o )
					return std::min( *r, *o, cmp );
				if( r ) return *r;
				return *o;
			}

			auto operator++() -> unioning_range &
			{
				if( r && o )
				{
					auto left = *r;
					auto right = *o;
					if( !cmp( right, left ) )
						while( ++r && !cmp( left, *r ) )
							;
					if( !cmp( left, right ) )
						while( ++o && !cmp( right, *o ) )
							;
				}
				else
				{
					++r;
					++o;
				}
				return *this;
			}

			auto operator++( int ) -> unioning_range
			{
				unioning_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> unioning_range &
			{
				auto left = r--;
				auto right = o--;
				while( left && r && !cmp( *r, *left ) )
					--r;
				while( right && o && !cmp( *o, *right ) )
					--o;
				if( o && r && cmp( *r, *o ) )
					r = left;
				else if( r && o && cmp( *o, *r ) )
					o = right;
				return *this;
			}

			auto operator--( int ) -> unioning_range
			{
				unioning_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				return r || o;
			}

			void goto_end()
			{
				r.goto_end();
				o.goto_end();
			}

	};

	template< typename range_type, typename other_range >
	class unioning_range_default : public unioning_range< range_type, other_range, std::less< decltype( *std::declval< range_type >() ) > >
	{
		
		public:
			unioning_range_default( const range_type & r, const other_range & o )
				: unioning_range< range_type, other_range, std::less< decltype( *r ) > >( r, o, std::less< decltype( *r ) >() )
			{
			}
	};


	template< typename range_type, typename comparer >
	auto union_with( const range_type & r, const comparer & cmp )
		-> decltype( make_factory< unioning_range >( r, cmp ) )
	{
		return make_factory< unioning_range >( r, cmp );
	}

	template< typename range_type >
	auto union_with( const range_type & r ) 
		-> decltype( make_factory< unioning_range_default >( r ) )
	{
		return make_factory< unioning_range_default >( r );
	}

}



