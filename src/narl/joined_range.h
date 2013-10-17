#pragma once

#include "range.h"
#include "range_factory.h"


namespace narl
{

	template< typename inner_range, typename outer_range, typename inner_key_selector, typename outer_key_selector, typename transformation, typename key_comparer >
	class joined_range
	{

		private:
			mutable range< decltype( *std::declval< inner_range >() ) > r, old_r;
			mutable outer_range o;
			bool reset_r;
			inner_key_selector inner;
			outer_key_selector outer;
			transformation expr;
			key_comparer cmp;

			void find_next() const
			{
				while( r && o && !are_equal() )
				{
					while( r && o && cmp( inner( *r ), outer( *o ) ) ) ++r;
					while( o && r && cmp( outer( *o ), inner( *r ) ) ) ++o;
				}
			}

			void find_prev_match() const
			{
				while( r && o && !( are_equal() ) )
				{
					while( r && o && cmp( inner( *r ), outer( *o ) ) ) --o;
					while( r && o && cmp( outer( *o ), inner( *r ) ) ) --r;
				}
			}

			bool are_equal() const
			{
				auto left = inner( *r );
				auto right = outer( *o );
				return !( cmp( left, right ) || cmp( right, left ) );
			}


		public:
			joined_range( const inner_range & r, const outer_range & o, const inner_key_selector & inner, const outer_key_selector outer, const transformation & expr, const key_comparer cmp )
				: r{ r }, old_r{ r }, o{ o }, reset_r{ false }, inner( inner ), outer( outer ), expr( expr ), cmp( cmp )
			{
			}


			auto operator*() const -> decltype( expr( *r, *o ) )
			{
				find_next();
				return expr( *r, *o );
			}

			auto operator++() -> joined_range &
			{
				if( !r || !o )
				{
					if( !r ) ++r;
					if( !o ) ++o;
				}
				else
				{
					auto tmp = r;
					if( o && r && ++tmp && !cmp( outer( *o ), inner( *tmp ) ) )
					{
						old_r = r++;
						reset_r = true;
					}
					else
					{
						++o;
						if( reset_r )
						{
							r = old_r;
							reset_r = false;
						}
					}					
				}
				find_next();
				return *this;
			}

			auto operator++( int ) -> joined_range
			{
				find_next();
				joined_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> joined_range &
			{
				if( !r || !o )
				{
					if( !r ) --r;
					if( !o ) --o;
				}
				else
				{
					auto tmp = r;
					if( o && r && --tmp && !cmp( inner( *tmp ), outer( *o ) ) )
					{
						old_r = r--;
						reset_r = true;
					}
					else
					{
						--o;
						if( reset_r )
						{
							r = old_r;
							reset_r = false;
						}
					}
				}				
				find_prev_match();
				return *this;
			}

			auto operator--( int ) -> joined_range
			{
				find_next();
				joined_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				find_next();
				return r && o;
			}

			void goto_end()
			{
				r.goto_end();
				o.goto_end();
			}

	};

	struct join_cmp
	{
		template< typename left_type >
		bool operator()( const left_type & l, const left_type & r ) const
		{
			return l < r;
		}
	};
	template< typename inner_range, typename outer_range, typename inner_key_selector, typename outer_key_selector, typename transformation >
	class joined_range_default : public joined_range< inner_range, outer_range, inner_key_selector, outer_key_selector, transformation, join_cmp >
	{

		public:
			joined_range_default( const inner_range & r, const outer_range & o, const inner_key_selector & inner, const outer_key_selector & outer, const transformation t )
				: joined_range< inner_range, outer_range, inner_key_selector, outer_key_selector, transformation, join_cmp >( r, o, inner, outer, t, join_cmp() )
			{
			}

	};

	template< typename outer_range, typename inner_key_selector, typename outer_key_selector, typename transformation >
	auto join( const outer_range & o, const inner_key_selector & inner, const outer_key_selector & outer, const transformation & t ) -> decltype( make_factory< joined_range_default >( o, inner, outer, t ) )
	{
		return make_factory< joined_range_default >( o, inner, outer, t );
	}

}
