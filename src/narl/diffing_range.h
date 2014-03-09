#pragma once

#include "range_factory.h"

#include <functional>


namespace narl
{

	template< typename range_type, typename other_range, typename comparer >
	class diffing_range
	{

		private:
			mutable range_type r;
			mutable other_range o;
			comparer cmp;

			static void skip_forward( range_type & r, other_range & o, comparer cmp )
			{
				if( !r )
					o.goto_end();
				//if( r && o && !cmp( *r, *o ) )
				while( r && o && !cmp( *r, *o ) )
				{
					if( !cmp( *o, *r ) )
						++r;
					if( r && !cmp( *r, *o ) )
						++o;
				}
			}

			static void skip_backward( range_type & r, other_range & o, comparer cmp )
			{
				while( !r && o )
					--o;
				while( r && o && !cmp( *o, *r ) )
				{
					if( !cmp( *r, *o ) )
						--r;
					if( r && !cmp( *o, *r ) )
						--o;
				}
			}

			void( *skip )( range_type & r, other_range & o, comparer cmp );
			

		public:
			diffing_range( range_type r, other_range o, comparer cmp )
				: r{ r }, o{ o }, cmp( cmp ), skip{ skip_forward }
			{
			}


			auto operator*() const -> decltype( *r )
			{
				skip( r, o, cmp );
				return *r;
			}

			auto operator++() -> diffing_range &
			{
				skip = skip_forward;
				if( !r && !o )
				{
					++r;
					++o;
				}
				else
					++r;
				skip( r, o, cmp );
				return *this;
			}

			auto operator++( int ) -> diffing_range
			{
				skip( r, o, cmp );
				diffing_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> diffing_range &
			{
				skip = skip_backward;
				if( !r && !o )
				{
					--r;
					--o;
				}
				else
					--r;
				skip( r, o, cmp );
				return *this;
			}

			auto operator--( int ) -> diffing_range
			{
				diffing_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				skip( r, o, cmp );
				return !!r;
			}

			void goto_end()
			{
				r.goto_end();
				o.goto_end();
			}

	};


	template< typename range_type, typename other_range >
	class diffing_range_default : public diffing_range< range_type, other_range, std::less< decltype( *std::declval< range_type >() ) > >
	{

		public:
			diffing_range_default( range_type r, other_range o )
				: diffing_range< range_type, other_range, std::less< decltype( *r ) > >( r, o, std::less< decltype( *r ) >() )
			{
			}

	};


	template< typename range_type, typename comparer >
	auto except( range_type r, comparer cmp ) -> decltype( make_factory< diffing_range >( r, cmp ) )
	{
		return make_factory< diffing_range >( r, cmp );
	}

	template< typename range_type >
	auto except( range_type r )
		-> decltype( make_factory< diffing_range_default >( r ) )
	{
		return make_factory< diffing_range_default >( r );
	}

}
