#pragma once

#include "range_factory.h"


namespace narl
{

	template< typename range_type, typename other_range >
	class concatenating_range
	{

		private:
			range_type r;
			other_range o;
			bool before_begin;


		public:
			concatenating_range( const range_type & r, const other_range & o )
				: r{ r }, o{ o }, before_begin{ false }
			{
			}


			auto operator*() const -> decltype( *r )
			{
				if( !r )
					return *o;
				return *r;
			}

			auto operator++() -> concatenating_range &
			{
				if( before_begin )
					++r;
				else
					if( !r )
						++o;
					else
						++r;
				before_begin = before_begin ? false : before_begin;
				return *this;
			}

			auto operator++( int ) -> concatenating_range
			{
				concatenating_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> concatenating_range &
			{
				if( !r )
					--o;
				if( !!r || !o )
				{
					--r;
					before_begin = !r;
				}
				return *this;
			}

			auto operator--( int ) -> concatenating_range
			{
				concatenating_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				return !before_begin && !( !r && !o );
			}

			void goto_end()
			{
				r.goto_end();
				o.goto_end();
			}

	};


	template< typename range_type >
	auto concat( const range_type & r ) -> decltype( make_factory< concatenating_range >( r ) )
	{
		return make_factory< concatenating_range >( r );
	}

}
