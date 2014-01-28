#pragma once

#include <cstring>
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
				if( !before_begin )
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
				return !before_begin && pos != endpos;
			}


		private:
			iterator_type startpos, endpos;
			iterator_type pos;
			bool before_begin;
	};


	template< template< typename... > class container_type, typename value_type >
	class own_container_range
	{

		public:
#ifndef _MSC_VER
			own_container_range( container_type< value_type > && src )
#else
			own_container_range( container_type< value_type, std::allocator< value_type > > && src )
#endif
#ifndef _MSC_VER
				: src{ std::make_shared< container_type< value_type > >( std::move( src ) ) }, pos{ std::begin( *( this->src ) ) }, before_begin{ false }
#else
				: src{ std::make_shared< container_type< value_type, std::allocator< value_type > > >( std::move( src ) ) }, pos{ std::begin( *( this->src ) ) }, before_begin{ false }
#endif
			{
			}

			own_container_range( std::initializer_list< value_type > && src )
#ifndef _MSC_VER
				: src{ std::make_shared< container_type< value_type > >( std::move( src ) ) }, pos{ std::begin( *( this->src ) ) }, before_begin{ false }
#else
				: src{ std::make_shared< container_type< value_type, std::allocator< value_type > > >( std::move( src ) ) }, pos{ std::begin( *( this->src ) ) }, before_begin{ false }
#endif
			{
			}


			auto operator*() const -> value_type
			{
				if( ! operator bool() )
					throw std::out_of_range( "Attempt to access an invalid range object" );
				return *pos;
			}

			auto operator++() -> own_container_range &
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

			auto operator++( int ) -> own_container_range
			{
				own_container_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> own_container_range &
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

			auto operator--( int ) -> own_container_range
			{
				own_container_range tmp{ *this };
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
#ifndef _MSC_VER
			std::shared_ptr< container_type< value_type > > src;
			typename container_type< value_type >::const_iterator pos;
#else
			std::shared_ptr< container_type< value_type, std::allocator< value_type > > > src;
			typename container_type< value_type, std::allocator< value_type > >::const_iterator pos;
#endif
			bool before_begin;
	};


	template< typename container >
	auto from( const container & src ) 
		-> iterable_range< typename container::const_iterator, typename container::value_type >
	{
		return iterable_range< typename container::const_iterator, typename container::value_type >
			{ std::begin( src ), std::end( src ) };
	}

	template< template< typename... > class container, typename value_type >
#ifndef _MSC_VER
	auto from( container< value_type > && c )
#else
//	template< template< typename, typename > class container, typename value_type >
	auto from( container< value_type, std::allocator< value_type > > && c )
#endif
		-> own_container_range< container, value_type >
	{
		return own_container_range< container, value_type > { std::move( c ) };
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
		-> iterable_range< typename std::initializer_list< item_type >::const_iterator, item_type >
	{
		return iterable_range< typename std::initializer_list< item_type >::const_iterator, item_type >
			{ std::begin( src ), std::end( src ) };
	}

	template< typename item_type >
	auto from( std::initializer_list< item_type > && src ) 
		-> own_container_range< std::vector, item_type >
	{
		return own_container_range< std::vector, item_type >
			{ std::move( src ) };
	}

	inline auto from( const char * const src )
		-> iterable_range< const char * , char >
	{
		return iterable_range< const char * , char >
			{ src, src + std::strlen( src ) };
	}
}
