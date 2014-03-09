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
			unioning_range( range_type r, other_range o, comparer cmp )
				: r{ r }, o{ o }, cmp( cmp )
			{
			}


			auto operator*() const -> decltype( *r )
			{
				if( r && o )
					return cmp( *r, *o ) ? *r : *o;
				if( r ) return *r;
				return *o;
			}

			auto operator++() -> unioning_range &
			{
				if( r && o )
				{
					if( !cmp( *o, *r ) )
					{
						if( !cmp( *r++, *o ) )
							++o;
					}
					else
						++o;
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
				--r;
				--o;
				if( r && o )
				{
					if( cmp( *r, *o ) )
						++r;
					else if( cmp( *o, *r ) )
						++o;
				}
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
			unioning_range_default( range_type r, other_range o )
				: unioning_range< range_type, other_range, std::less< decltype( *r ) > >( r, o, std::less< decltype( *r ) >() )
			{
			}
	};


	template< typename range_type, typename comparer >
	auto union_with( range_type r, comparer cmp )
		-> decltype( make_factory< unioning_range >( r, cmp ) )
	{
		return make_factory< unioning_range >( r, cmp );
	}

	template< typename range_type >
	auto union_with( range_type r ) 
		-> decltype( make_factory< unioning_range_default >( r ) )
	{
		return make_factory< unioning_range_default >( r );
	}

}



