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

			range_generator( const item_type & v )
				: v{ v }
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


	template< typename item_type, typename callback >
	class range_generator_callback
	{

		private:
			item_type v;
			callback fn;

		public:
			range_generator_callback( const item_type & init, const callback & fn ) 
				: v{ init }, fn( fn )
			{
			}


			auto operator *() const -> item_type
			{ 
				return v; 
			}

			auto operator++() -> range_generator_callback &
			{
				v = fn( v );
				return *this;
			}

			auto operator++( int ) -> range_generator_callback
			{
				range_generator_callback tmp{ *this };
				++*this;
				return tmp;
			}

			explicit operator bool() const 
			{ 
				return true; 
			}

	};

	struct identity
	{
		template< typename T >
		const T & operator()( const T & t ) { return t; }
	};

	template< typename item_type >
	class range_generator_callback_default : public range_generator_callback< item_type, identity >
	{

		public:
			range_generator_callback_default( const item_type & i )
				: range_generator_callback< item_type, identity >( i, identity() )
			{
			}

	};

	template< typename item_type >
	auto make_range() -> range_generator< item_type >
	{
		return range_generator< item_type >{ };
	}

	template< typename item_type >
	auto make_range( const item_type & init ) -> range_generator< item_type >
	{
		return range_generator< item_type >{ init };
	}

	template< typename item_type >
	auto fill_range( const item_type & init ) -> range_generator_callback_default< item_type >
	{
		return range_generator_callback_default< item_type >{ init };
	}

	template< typename item_type, typename callback >
	auto make_range( const item_type & init, const callback & fn ) -> range_generator_callback< item_type, callback >
	{
		return range_generator_callback< item_type, callback >{ init, fn };
	}

}
