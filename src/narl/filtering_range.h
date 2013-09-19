#pragma once


namespace narl
{

	template< typename range_type, typename predicate >
	class filtering_range
	{

		private:
			mutable range_type r;
			predicate filter;

			void find_next_match() const
			{
				while( r && !filter( *r ) )
					++r;
			}


		public:
			filtering_range( range_type r, predicate filter )
				: r{ r }, filter{ filter }
			{
			}

			
			auto operator*() const -> decltype( *r )
			{
				find_next_match();
				return *r;
			}

			auto operator++() -> filtering_range &
			{
				++r;
				find_next_match();
				return *this;
			}

			auto operator++( int ) -> filtering_range
			{
				find_next_match();
				filtering_range tmp{ *this };
				++*this;
				return tmp;
			}

			explicit operator bool() const
			{
				find_next_match();
				return !!r;
			}

	};

}
