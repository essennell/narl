#pragma once

#include "range.h"
#include "range_factory.h"

#include <memory>
#include <type_traits>
#include <utility>


namespace narl
{

	template< typename range_type, typename binder >
	class selectmany_range
	{

		private:
			mutable range_type r;
			binder expr;

#ifndef _MSC_VER
			mutable range< decltype( *expr( *r ) ) > buffer;
			mutable decltype( *r ) current;
#else
			typedef decltype( *std::declval< range_type >() ) param_type;
			typedef typename std::result_of< binder( param_type ) >::type result_type;
			mutable range< decltype( *std::declval< result_type >() ) > buffer;
			mutable param_type current;
#endif

			void extract() const
			{
				if( r )
				{
					if( !buffer )
					{
						current = *r;
						buffer = expr( current );
					}
				}
				else
					buffer = range< decltype( *expr( *r ) ) >{};
			}


		public:
			selectmany_range( range_type r, binder expr )
				: r{ r }, expr( expr )
			{
			}


			auto operator*() const -> decltype( *buffer )
			{
				extract();
				return *buffer;
			}

			auto operator++() -> selectmany_range &
			{
				if( !r || ( buffer && !++buffer ) )
				{
					++r;
					extract();
				}
				return *this;
			}

			auto operator++( int ) -> selectmany_range
			{
				extract();
				selectmany_range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> selectmany_range &
			{
				if( !buffer || !--buffer )
				{
					--r;
					extract();
					if( buffer )
					{
						buffer.goto_end();
						--buffer;
					}
				}
				return *this;
			}

			auto operator--( int ) -> selectmany_range
			{
				extract();
				selectmany_range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				extract();
				return r && buffer;
			}

			void goto_end()
			{
				r.goto_end();
			}

	};

	template< typename binder >
	auto selectmany( binder b ) -> decltype( make_factory< selectmany_range >( b ) )
	{
		return make_factory< selectmany_range >( b );
	}

}
