#pragma once


namespace narl
{

	template< template< typename > class range_type, typename expression_type >
	class range_1_factory
	{

		public:
			range_1_factory( expression_type expr )
				: expr{ expr }
			{
			}

			template< typename other_range_type >
			auto operator()( other_range_type r ) -> range_type< other_range_type >
			{
				return range_type< other_range_type >{ r, expr };
			}


		private:
			expression_type expr;

	};

	template< template< typename, typename > class range_type, typename expression_type >
	class range_2_factory
	{

		public:
			range_2_factory( expression_type expr )
				: expr{ expr }
			{
			}

			template< typename other_range_type >
			auto operator()( other_range_type r ) -> range_type< other_range_type, expression_type >
			{
				return range_type< other_range_type, expression_type >{ r, expr };
			}


		private:
			expression_type expr;

	};


	template< template< typename, typename > class range_type, typename expression_type >
	auto make_factory( expression_type expr )
		-> range_2_factory< range_type, expression_type >
	{
		return range_2_factory< range_type, expression_type >{ expr };
	}

	template< template< typename > class range_type, typename expression_type >
	auto make_factory( expression_type expr )
		-> range_1_factory< range_type, expression_type >
	{
		return range_1_factory< range_type, expression_type >{ expr };
	}

}
