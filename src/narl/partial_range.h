#pragma once


namespace narl
{

	template< typename range_type, typename limiting_type >
	class partial_range
	{

		private:
			mutable range_type r;
			mutable limiting_type limiter; 


		public:
			partial_range( range_type r, limiting_type limiter )
				: r{ r }, limiter{ limiter }
			{
			}


			auto operator*() const -> decltype( *r )
			{
				limiter.skip( r );
				return *r;
			}

			auto operator++() -> partial_range &
			{
				limiter.skip( r );
				limiter.step( r );
				return *this;
			}

			auto operator++( int ) -> partial_range
			{
				limiter.skip( r );
				partial_range tmp{ *this };
				++*this;
				return tmp;
			}

			explicit operator bool() const
			{
				limiter.skip( r );
				return !!r && limiter.valid();
			}

	};


	class skipper
	{

		template< typename range_type, typename limiting_type > 
		friend class partial_range;


		public:
			skipper( unsigned count ) 
				: count{ count } 
			{ }


		private:
			template< typename range_type >
			void skip( range_type & r )
			{
				while( r && count > 0 ) 
				{ 
					++r; 
					--count; 
				}
			}

			template< typename range_type >
			void step( range_type & r ) 
			{ 
				if( r )
					++r; 
			}

			auto valid() -> bool 
			{ 
				return true; 
			}
			
			unsigned count;

	};

	class taker
	{

		template< typename range_type, typename limiting_type > 
		friend class partial_range;


		public:
			taker( unsigned count ) 
				: count{ count } 
			{ }


		private:
			template< typename range_type >
			void skip( const range_type & ) { }
			
			template< typename range_type >
			void step( range_type & r ) 
			{ 
				if( valid() && r ) 
				{ 
					--count; 
					++r; 
				}
			}
			
			auto valid() -> bool 
			{ 
				return count > 0; 
			}
			
			unsigned count;

	};


	template< typename expression >
	class skipping_while
	{

		template< typename range_type, typename limiting_type > 
		friend class partial_range;


		public:
			skipping_while( expression expr ) 
				: expr{ expr }, skipped{ false }
			{ }


		private:
			template< typename range_type >
			void skip( range_type & r )
			{
				while( !skipped && r && expr( *r ) )
					++r; 
				skipped = true;
			}

			template< typename range_type >
			void step( range_type & r ) 
			{ 
				if( r )
					++r; 
			}

			auto valid() -> bool 
			{ 
				return true; 
			}
			
			expression expr;
			bool skipped;

	};

	template< typename expression >
	auto make_skipping_while( expression expr ) -> skipping_while< expression >
	{
		return skipping_while< expression >{ expr };
	}


	template< typename expression >
	class taking_while
	{

		template< typename range_type, typename limiting_type > 
		friend class partial_range;


		public:
			taking_while( expression expr ) 
				: expr{ expr }, stopped{ false }
			{ }


		private:
			template< typename range_type >
			bool check( const range_type & r )
			{
				if( !stopped && r && !expr( *r ) )
					stopped = true;
				return !stopped;
			}
			
			template< typename range_type >
			void skip( const range_type & r ) 
			{ 
				check( r ); 
			}
			
			template< typename range_type >
			void step( range_type & r ) 
			{ 
				if( check( r ) )
					++r; 
			}
			
			auto valid() -> bool 
			{ 
				return !stopped;
			}
			
			expression expr;
			bool stopped;

	};

	template< typename expression >
	auto make_taking_while( expression expr ) -> taking_while< expression >
	{
		return taking_while< expression >{ expr };
	}

}
