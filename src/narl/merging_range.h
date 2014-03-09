#pragma once

#include "range_factory.h"


namespace narl
{

	template< typename range_type, typename other_range_type, typename comparitor >
	class merging_range
	{

		private:
			range_type r;
			other_range_type o;
			comparitor cmp;
			

		public:
			merging_range( range_type r, other_range_type o, comparitor cmp )
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

			auto operator++() -> merging_range &
			{
				if( r && o )
				{
					if( cmp( *r, *o ) )
						++r;
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

			auto operator++( int ) -> merging_range
			{
				merging_range tmp { *this };
				++*this;
				return tmp;
			}

			auto operator--() -> merging_range &
			{
				--r;
				--o;
				if( r && o )
				{
					if( cmp( *r, *o ) )
						++r;
					else 
						++o;
				}
				return *this;
			}

			auto operator--( int ) -> merging_range
			{
				merging_range tmp { *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				return !!r || !!o;
			}

			void goto_end()
			{
				r.goto_end();
				o.goto_end();
			}

	};

	template< typename range_type, typename other_range_type >
	class merging_range_default : public merging_range< range_type, other_range_type, std::less< decltype( *std::declval< range_type >() ) > >
	{

		public:
			merging_range_default( range_type r, other_range_type o )
				: merging_range< range_type, other_range_type, std::less< decltype( *r ) > >( r, o, std::less< decltype( *r ) >() )
			{
			}

	};


	template< typename other_range_type, typename comparitor >
	auto merge_with( other_range_type o, comparitor cmp ) -> decltype( make_factory< merging_range >( o, cmp ) )
	{
		return make_factory< merging_range >( o, cmp );
	}

	template< typename other_range_type >
	auto merge_with( other_range_type o ) -> decltype( make_factory< merging_range_default >( o ) )
	{
		return make_factory< merging_range_default >( o );
	}

}
