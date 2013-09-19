#pragma once


namespace narl
{

	template< typename range_type, typename transformation >
	class transforming_range
	{

		private:
			range_type r;
			transformation expr;


		public:
			transforming_range( range_type r, transformation expr )
				: r{ r }, expr{ expr }
			{
			}


			auto operator*() const -> decltype( expr( *r ) )
			{
				return expr( *r );
			}

			auto operator++() -> transforming_range &
			{
				++r;
				return *this;
			}

			auto operator++( int ) -> transforming_range
			{
				transforming_range tmp{ *this };
				++*this;
				return tmp;
			}

			explicit operator bool() const
			{
				return !!r;
			}

	};

}
