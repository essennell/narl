#pragma once


namespace narl
{

	template< typename item_type >
	class range_generator
	{

		private:
			item_type v;


		public:
			range_generator() 
				: v{ item_type{} }
			{
			}


			auto operator *() const -> item_type
			{ 
				return v; 
			}

			auto operator++() -> range_generator &
			{
				++v;
				return *this;
			}

			auto operator++( int ) -> range_generator
			{
				range_generator tmp{ *this };
				++*this;
				return tmp;
			}

			explicit operator bool() const 
			{ 
				return true; 
			}

	};


	template< typename item_type >
	auto make_range() -> range_generator< item_type >
	{
		return range_generator< item_type >{ };
	}

}
