#pragma once

#include "range_factory.h"
#include "range_input_iterator.h"

#include <functional>
#include <list>
#include <memory>
#include <type_traits>

namespace narl
{

	template< typename range_type, typename ordering >
	class sorted_range
	{

		private:
			bool before_begin;
			range_type r;
			ordering cmp;
#ifndef _MSC_VER
			mutable std::shared_ptr< std::list< decltype( *r ) > > buffer;
			mutable typename std::list< decltype( *r ) >::const_iterator pos;
#else
			mutable std::shared_ptr< std::list< decltype( *std::declval< range_type >() ) > > buffer;
			mutable typename std::list< decltype( *std::declval< range_type >() ) >::const_iterator pos;
#endif

			void buildsort() const
			{
				if( !buffer )
				{
					buffer = std::make_shared< std::list< decltype( *r ) > >( begin( r ), end( r ) );
					buffer->sort( [&]( const decltype( *r ) & left, const decltype( *r ) & right )
						{ return cmp( left, right ); } );
					pos = std::begin( *buffer );
				}
			}


		public:
			sorted_range( const range_type & r, const ordering & cmp )
				: before_begin{ false }, r{ r }, cmp( cmp )
			{
			}

			sorted_range( const sorted_range & other )
				: before_begin{ other.before_begin }, r{ other.r }, cmp( other.cmp ), buffer{ other.buffer }, pos{ other.pos }
			{
			}


			auto operator*() const -> decltype( *r )
			{
				buildsort();
				return *pos;
			}

			auto operator++() -> sorted_range &
			{
				buildsort();
				if( pos != std::end( *buffer ) )
				{
					if( before_begin )
						before_begin = false;
					else
						++pos;
				}
				return *this;
			}

			auto operator++( int ) -> sorted_range
			{
				buildsort();
				sorted_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> sorted_range &
			{
				buildsort();
				if( !before_begin )
				{
					if( pos == std::begin( *buffer ) )
						before_begin = true;
					else
						--pos;
				}
				return *this;
			}

			auto operator--( int ) -> sorted_range
			{
				buildsort();
				sorted_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				buildsort();
				return !before_begin && pos != std::end( *buffer );
			}

			void goto_end()
			{
				buildsort();
				pos = std::end( *buffer );
			}

	};


	template< typename range_type >
	class sorted_range_default
		: public sorted_range< range_type, std::less< decltype( *std::declval< range_type >() ) > >
	{

		public:
			sorted_range_default( const range_type & r )
				: sorted_range< range_type, std::less< decltype( *std::declval< range_type >() ) > >( r, std::less< decltype( *r ) >() )
			{
			}

	};


	template< typename ordering >
	auto sorted( const ordering & cmp ) -> decltype( make_factory< sorted_range >( cmp ) )
	{
		return make_factory< sorted_range >( cmp );
	}

	inline auto sorted() -> decltype( make_factory< sorted_range_default >() )
	{
		return make_factory< sorted_range_default >();
	}

}
