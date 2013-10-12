#pragma once

#include "iterable_range.h"
#include "range_factory.h"

#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>


namespace narl
{

	template< typename range_type, typename key_selector >
	class grouping_range
	{

		private:
			mutable range_type r;
			key_selector key;

#ifndef _MSC_VER
			typedef decltype( key( *r ) ) key_type;
			typedef std::vector< decltype( *r ) > value_type;
#else
			typedef typename std::result_of< key_selector( decltype( *std::declval< range_type >() ) ) >::type key_type;
			typedef std::vector< decltype( *std::declval< range_type >() ) > value_type;
#endif

			mutable std::shared_ptr< std::map< key_type, value_type > > groups;
			mutable typename std::map< key_type, value_type >::const_iterator pos;
			bool before_begin;

			void buildgroups() const
			{
				if( ! groups )
				{
					auto local = std::make_shared< std::map< key_type, value_type > >();
					while( r )
					{
						const auto & i = *r++;
						( *local )[ key( i ) ].push_back( i );
					}
					groups = local;
					pos = std::begin( *groups );
				}
			}

			struct grouping
			{
				grouping( const key_type & key, const iterable_range< typename value_type::const_iterator, typename value_type::value_type > & values )
					: key{ key }, values{ values }
					{ }
				key_type key;
				iterable_range< typename value_type::const_iterator, typename value_type::value_type > values;
			};


		public:
			grouping_range( const range_type & r, const key_selector & key )
				: r{ r }, key( key ), before_begin{ false }
			{
			}

			grouping_range( const grouping_range & other )
				: r{ other.r }, key( other.key ), groups{ other.groups }, pos{ other.pos }, before_begin{ other.before_begin }
			{
			}


			auto operator*() const -> grouping
			{
				buildgroups();
				return grouping{ pos->first, from( pos->second ) };
			}

			auto operator++() -> grouping_range &
			{
				buildgroups();
				if( pos != std::end( *groups ) )
				{
					++pos;
					before_begin = before_begin ? false : before_begin;
				}
				return *this;
			}

			auto operator++( int ) -> grouping_range
			{
				buildgroups();
				grouping_range tmp{ *this };
				operator++();
				return tmp;
			}

			auto operator--() -> grouping_range &
			{
				buildgroups();
				if( ! before_begin )
				{
					before_begin = pos == std::begin( *groups );
					if( ! before_begin )
						--pos;
				}
				return *this;
			}

			auto operator--( int ) -> grouping_range
			{
				buildgroups();
				grouping_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				buildgroups();
				return !before_begin && pos != std::end( *groups );
			}

			void goto_end()
			{
				buildgroups();
				pos = std::end( *groups );
			}

	};


	template< typename key_selector >
	auto groupby( const key_selector & key )
		-> decltype( make_factory< grouping_range >( key ) )
	{
		return make_factory< grouping_range >( key );
	}

}



