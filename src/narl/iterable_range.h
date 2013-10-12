#pragma once

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <vector>


namespace narl
{

	template< typename iterator_type, typename value_type >
	class iterable_range
	{

		public:
			iterable_range( iterator_type startpos, iterator_type endpos )
				: startpos{ startpos }, endpos{ endpos }, pos{ startpos }, before_begin{ false }
			{
			}
			

			auto operator*() const -> value_type
			{
				if( ! operator bool() )
					throw std::out_of_range( "Attempt to access an invalid range object" );
				return *pos;
			}

			auto operator++() -> iterable_range &
			{
				if( pos != endpos )
				{
					if( before_begin )
						before_begin = false;
					else
						++pos;
				}
				return *this;
			}

			auto operator++( int ) -> iterable_range
			{
				iterable_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> iterable_range &
			{
				if( ! before_begin )
				{
					if( pos == startpos )
						before_begin = true;
					else
						--pos;
				}
				return *this;
			}

			auto operator--( int ) -> iterable_range
			{
				iterable_range tmp{ *this };
				--*this;
				return tmp;
			}

			void goto_end()
			{
				pos = endpos;
			}
			
			explicit operator bool() const 
			{
				return pos != endpos && !before_begin;
			}


		private:
			iterator_type startpos, endpos;
			iterator_type pos;
			bool before_begin;

	};


	template< typename value_type >
	class iterable_initlist_range
	{

		public:
			iterable_initlist_range( const std::initializer_list< value_type > & src )
				: src{ std::make_shared< std::vector< value_type > >( src ) }, pos{ std::begin( *( this->src ) ) }, before_begin{ false }
			{
			}


			auto operator*() const -> value_type
			{
				if( ! operator bool() )
					throw std::out_of_range( "Attempt to access an invalid range object" );
				return *pos;
			}

			auto operator++() -> iterable_initlist_range &
			{
				if( pos != std::end( *src ) )
				{
					if( before_begin )
						before_begin = false;
					else
						++pos;
				}
				return *this;
			}

			auto operator++( int ) -> iterable_initlist_range
			{
				iterable_initlist_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> iterable_initlist_range &
			{
				if( !before_begin )
				{
					if( pos == std::begin( *src ) )
						before_begin = true;
					else
						--pos;
				}
				return *this;
			}

			auto operator--( int ) -> iterable_initlist_range
			{
				iterable_initlist_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				return !before_begin && pos != std::end( *src );
			}
		
			void goto_end()
			{
				pos = std::end( *src );
			}


		private:
			std::shared_ptr< std::vector< value_type > > src;
			typename std::vector< value_type >::const_iterator pos;
			bool before_begin;
	};


	template< typename container >
	auto from( const container & src ) 
		-> iterable_range< typename container::const_iterator, typename container::value_type >
	{
		return iterable_range< typename container::const_iterator, typename container::value_type >
			{ std::begin( src ), std::end( src ) };
	}

	template< typename value_type, size_t len >
	auto from( value_type( &array )[ len ] ) 
		-> iterable_range< const value_type*, value_type >
	{
		return iterable_range< const value_type*, value_type >
			{ array, array + len };
	}

	template< typename item_type >
	auto from( const std::initializer_list< item_type > & src ) 
		-> iterable_initlist_range< item_type >
	{
		return iterable_initlist_range< item_type >
			{ src };
	}

}
