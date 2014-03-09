#pragma once


#include <iterator>


namespace narl
{

	template< typename range_type, typename value_type >
	class range_input_iterator : public std::iterator< std::input_iterator_tag, value_type >
	{

		public:
			range_input_iterator( range_type r )
				: r{ r }
			{
			}


			auto operator!=( range_input_iterator r ) const -> bool
			{
				return !operator==( r );
			}

			auto operator==( range_input_iterator ) const -> bool
			{
				return !r;
			}

			auto operator++() -> range_input_iterator & 
			{
				++r;
				return *this;
			}

			auto operator*() const -> value_type
			{
				return *r;
			}


		private:
			range_type r;
			
	};


	template< typename range_type >
	auto begin( range_type r ) -> range_input_iterator< range_type, decltype( *r ) >
	{
		return range_input_iterator< range_type, decltype( *r ) >{ r };
	}

	template< typename range_type >
	auto end( range_type r ) -> range_input_iterator< range_type, decltype( *r ) >
	{
		return range_input_iterator< range_type, decltype( *r ) >{ r };
	}

}
