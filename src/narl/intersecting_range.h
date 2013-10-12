#pragma once

#include "range_factory.h"


namespace narl
{

	template< typename range_type, typename other_range, typename comparer >
	class intersecting_range
	{
		
		private:
			mutable range_type r;
			mutable other_range o;
			comparer cmp;

			void find_next_match() const
			{
				while( r && o && !( are_equal() ) )
				{
					while( r && o && cmp( *r, *o ) ) ++r;
					while( o && r && cmp( *o, *r ) ) ++o;
				}
			}

			void find_prev_match() const
			{
				while( r && o && !( are_equal() ) )
				{
					while( r && o && cmp( *r, *o ) ) --o;
					while( r && o && cmp( *o, *r ) ) --r;
				}
			}

			bool are_equal() const
			{
				auto l = *r;
				auto r = *o;
				return !( cmp( l, r ) || cmp( r, l ) );
			}


		public:
			intersecting_range( const range_type & r, const other_range & o, comparer cmp )
				: r{ r }, o{ o }, cmp( cmp )
			{
			}


			auto operator*() const -> decltype( *r )
			{
				find_next_match();
				return *r;
			}

			auto operator++() -> intersecting_range &
			{
				if( !r || !o )
				{
					if( !o ) ++o;
					if( !r ) ++r;
				}
				else
				{
					while( r && o && !cmp( *o, *r ) )
						++r;
				}
				find_next_match();
				return *this;
			}

			auto operator++( int ) -> intersecting_range
			{
				find_next_match();
				intersecting_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> intersecting_range &
			{
				if( !r || !o )
				{
					if( !r ) --r;
					if( !o ) --o;
				}
				else
				{
					while( o && r && !cmp( *o, *r ) )
						--o;
				}
				find_prev_match();
				return *this;
			}

			auto operator--( int ) -> intersecting_range
			{
				find_next_match();
				intersecting_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				find_next_match();
				return !!r && !!o;
			}

			void goto_end()
			{
				r.goto_end();
				o.goto_end();
			}

	};

	template< typename range_type, typename other_range >
	class intersecting_range_default : public intersecting_range< range_type, other_range, std::less< decltype( *std::declval< range_type >() ) > >
	{

		public:
			intersecting_range_default( const range_type & r, const other_range & other )
				: intersecting_range< range_type, other_range, std::less< decltype( *r ) > >( r, other, std::less< decltype( *r ) >() )
			{
			}

	};


	template< typename range_type, typename comparer >
	auto intersect_with( const range_type & r, const comparer & cmp ) 
		-> decltype( make_factory< intersecting_range >( r, cmp ) )
	{
		return make_factory< intersecting_range >( r, cmp );
	}

	template< typename range_type >
	auto intersect_with( const range_type & r ) 
		-> decltype( make_factory< intersecting_range_default >( r ) )
	{
		return make_factory< intersecting_range_default >( r );
	}

}



