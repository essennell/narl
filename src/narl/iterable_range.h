#pragma once


#include <stdexcept>


namespace narl
{

	template< typename iterator_type, typename value_type >
	class iterable_range
	{

		public:
			iterable_range( iterator_type pos, iterator_type endpos )
				: pos{ pos }, endpos{ endpos }
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
				if( operator bool() )
					++pos;
				return *this;
			}

			auto operator++( int ) -> iterable_range
			{
				iterable_range tmp{ *this };
				++*this;
				return tmp;
			}

			explicit operator bool() const 
			{
				return pos != endpos;
			}


		private:
			iterator_type pos, endpos;

	};

}
