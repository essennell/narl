#pragma once

#include "range_factory.h"

#include <tuple>


namespace narl
{

	template< typename... ranges >
	class zipping_range;

	template< typename range_type, typename other_range_type >
	class zipping_range< range_type, other_range_type >
	{
		
		private:
			range_type r;
			other_range_type other;


		public:
			zipping_range( const range_type & r, const other_range_type & other )
				: r{ r }, other{ other }
			{
			}


			auto operator*() const -> decltype( std::make_tuple( *r, *other ) )
			{
				return std::make_tuple( *r, *other );
			}

			auto operator++() -> zipping_range &
			{
				++r;
				++other;
				return *this;
			}
			
			auto operator++( int ) -> zipping_range
			{
				zipping_range tmp{ *this };
				++*this;
				return tmp;
			}
			
			auto operator--() -> zipping_range &
			{
				--r;
				--other;
				return *this;
			}
			
			auto operator--( int ) -> zipping_range
			{
				zipping_range tmp{ *this };
				--*this;
				return tmp;
			}
			
			explicit operator bool() const
			{
				return !!r && !!other;
			}

			void goto_end()
			{
				r.goto_end();
				other.goto_end();
			}

	};

	template< typename range_type, typename... others >
	class zipping_range< range_type, others... > : public zipping_range< others... >
	{

		typedef zipping_range< others... > base;

		private:
			range_type r;
			bool before_begin;


		public:
			zipping_range( const range_type r, const others &... rest )
				: base{ rest... }, r{ r }, before_begin{ false }
			{
			}


			auto operator*() const -> decltype( std::tuple_cat( std::make_tuple( *r ), std::declval< base >().operator*() ) )
			{
				return std::tuple_cat( std::make_tuple( *r ), base::operator*() );
			}

			auto operator++() -> zipping_range &
			{
				if( before_begin || operator bool() )
				{
					++r;
					base::operator++();
					before_begin = before_begin ? false :  before_begin;
				}
				return *this;
			}

			auto operator++( int ) -> zipping_range
			{
				zipping_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> zipping_range &
			{
				if( !before_begin )
				{
					--r;
					base::operator--();
					before_begin = !operator bool();
				}
				return *this;
			}

			auto operator--( int ) -> zipping_range
			{
				zipping_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				return r && base::operator bool();
			}

			void goto_end()
			{
				r.goto_end();
				base::goto_end();
			}

	};


#ifndef _MSC_VER

	template< typename... range_type >
	auto zipwith( const range_type &... r ) -> decltype( make_factory< zipping_range >( r... ) )
	{
		return make_factory< zipping_range >( r... );
	}

#else

	template< typename range_type >
	auto zipwith( const range_type & r ) -> decltype( make_factory< zipping_range >( r ) )
	{
		return make_factory< zipping_range >( r );
	}

#endif

}


