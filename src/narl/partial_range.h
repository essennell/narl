#pragma once

#include "range_factory.h"


namespace narl
{

	template< typename range_type, typename limiting_type >
	class partial_range
	{

		private:
			mutable range_type r;
			mutable limiting_type limiter; 


		public:
			partial_range( const range_type & r, const limiting_type & limiter )
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

			auto operator--() -> partial_range &
			{
				limiter.skip( r );
				limiter.back( r );
				return *this;
			}

			auto operator--( int ) -> partial_range
			{
				limiter.skip( r );
				partial_range tmp{ *this };
				--*this;
				return tmp;
			}

			void goto_end()
			{
				limiter.skip( r );
				limiter.goto_end( r );
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
				: count{ count }, pos{ 1 }
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
				if( pos == 0 || r )
				{
					++r;
					++pos;
				}
			}

			template< typename range_type >
			void back( range_type & r )
			{
				if( pos > 0 )
				{
					--r;
					--pos;
				}
			}

			auto valid() -> bool 
			{ 
				return pos > 0; 
			}

			template< typename range_type >
			void goto_end( range_type & r )
			{
				while( r )
					step( r );
			}
			
			unsigned count;
			unsigned pos;

	};

	class taker
	{

		template< typename range_type, typename limiting_type > 
		friend class partial_range;


		public:
			taker( unsigned count ) 
				: start{ count }, count{ count } 
			{ }


		private:
			template< typename range_type >
			void skip( const range_type & ) { }
			
			template< typename range_type >
			void step( range_type & r ) 
			{ 
				if( count > start || ( count > 0 && r ) )
				{ 
					--count; 
					++r; 
				}
			}

			template< typename range_type >
			void back( range_type & r )
			{
				if( count <= start && r )
				{
					++count;
					--r;
				}
			}
			
			auto valid() -> bool 
			{ 
				return count <= start && count > 0; 
			}

			template< typename range_type >
			void goto_end( range_type & r )
			{
				while( valid() && r )
					step( r );
			}
			
			unsigned start;
			unsigned count;

	};


	template< typename expression >
	class skipping_while
	{

		template< typename range_type, typename limiting_type > 
		friend class partial_range;


		public:
			skipping_while( const expression & expr ) 
				: skipped{ false }, expr( expr ), pos{ 1 }
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
				{
					++r; 
					++pos;
				}
			}

			template< typename range_type >
			void back( range_type & r )
			{
				if( pos > 0 )
				{
					--r;
					--pos;
				}
			}

			auto valid() -> bool 
			{ 
				return pos > 0; 
			}
			
			template< typename range_type >
			void goto_end( range_type & r )
			{
				while( r )
					step( r );
			}

			bool skipped;
			expression expr;
			unsigned pos;

	};

	template< typename expression >
	auto make_skipping_while( const expression & expr ) -> skipping_while< expression >
	{
		return skipping_while< expression >{ expr };
	}


	template< typename expression >
	class taking_while
	{

		template< typename range_type, typename limiting_type > 
		friend class partial_range;


		public:
			taking_while( const expression & expr ) 
				: expr( expr ), stopped{ false }, pos{ 1 }
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
				if( pos == 0 || check( r ) )
				{
					++r; 
					++pos;
				}
			}
			
			template< typename range_type >
			void back( range_type & r ) 
			{ 
				if( pos > 0 && r )
				{
					--r; 
					--pos;
					stopped = stopped ? false : stopped;
				}
			}
			
			auto valid() -> bool 
			{ 
				return !stopped && pos > 0;
			}
			
			template< typename range_type >
			void goto_end( range_type & r ) 
			{ 
				while( pos == 0 || check( r ) )
					step( r ); 
			}
			
			expression expr;
			bool stopped;
			unsigned pos;

	};

	template< typename expression >
	auto make_taking_while( const expression & expr ) -> taking_while< expression >
	{
		return taking_while< expression >{ expr };
	}


	inline auto skip( unsigned count ) -> decltype( make_factory< partial_range >( skipper( count ) ) )
	{
		return make_factory< partial_range >( skipper( count ) );
	}

	inline auto take( unsigned count ) -> decltype( make_factory< partial_range >( taker( count ) ) )
	{
		return make_factory< partial_range >( taker( count ) );
	}


	template< typename expression >
	auto skip_while( const expression & expr ) -> decltype( make_factory< partial_range >( make_skipping_while( expr ) ) )
	{
		return make_factory< partial_range >( make_skipping_while( expr ) );
	}

	template< typename expression >
	auto take_while( const expression & expr ) -> decltype( make_factory< partial_range >( make_taking_while( expr ) ) )
	{
		return make_factory< partial_range >( make_taking_while( expr ) );
	}

}
