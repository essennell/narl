#pragma once

#include <iostream>

#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>


namespace xri
{

	template< typename range_type >
	class basic_range
	{
		public:
			auto any() -> bool { return true; }

			template< typename unary_predicate >
			auto any( unary_predicate fn ) -> bool { return false; }

			template< typename unary_predicate >
			auto all( unary_predicate fn ) -> bool { return true; }

			auto count() -> size_t { return 0; }
	};

	template< template< typename, typename > class range_type, typename expression_type >
	class range_factory
	{
		public:
			range_factory( expression_type action ) : action{ action } { }

			template< typename range_of >
			auto operator()( range_of r ) -> range_type< range_of, expression_type >
			{ 				
				return range_type< range_of, expression_type >{ r, action };
			}

		private:
			expression_type action;
	};

	template< template< typename > class range_type, typename expression_type >
	class range_factory_1
	{
		public:
			range_factory_1( expression_type action ) : action{ action } { }

			template< typename range_of >
			auto operator()( range_of r ) -> range_type< range_of >
			{ 				
				return range_type< range_of >{ r, action };
			}

		private:
			expression_type action;
	};

	template< template< typename, typename > class range_type, typename expression_type >
	auto make_range_factory( expression_type expr ) -> range_factory< range_type, expression_type >
	{
		return range_factory< range_type, expression_type >{ expr };
	}

	template< template< typename > class range_type, typename expression_type >
	auto make_range_factory( expression_type expr ) -> range_factory_1< range_type, expression_type >
	{
		return range_factory_1< range_type, expression_type >{ expr };
	}

	template< typename iterator_type, typename value_type_tag=typename iterator_type::value_type >
	class iterable_range
	{
		public:
			typedef value_type_tag value_type; 

			iterable_range( iterator_type begin, iterator_type end )
				: pos{ begin }, end_pos{ end } { }

			auto operator*() const -> value_type
			{
				return *pos;
			}

			auto operator++() -> iterable_range &
			{
				++pos;
				return *this;
			}
			auto operator++( int ) -> iterable_range
			{
				iterable_range tmp{ *this };
				operator++();
				return tmp;
			}

			explicit operator bool() const { return pos != end_pos; }

		private:
			iterator_type pos, end_pos;
	};

	template< typename container_type >
	auto make_range( const container_type & ctr ) -> iterable_range< typename container_type::const_iterator >
	{
		return iterable_range< typename container_type::const_iterator >{ begin( ctr ), end( ctr ) };
	}

	template< typename array_type, size_t N >
	auto make_range( array_type( &array )[ N ] ) -> iterable_range< array_type*, array_type >
	{
		return iterable_range< array_type*, array_type >{ array, array + N };
	}


	template< typename range_type, typename transformation >
	class transforming_range
	{
		private:
			range_type r;
			transformation fn;

		public:
			transforming_range( range_type r, transformation fn )
				: r{ r }, fn{ fn } { }

			auto operator*() const -> decltype( fn( *r ) )
			{
				return fn( *r );
			}

			auto operator++() -> transforming_range &
			{
				++r;
				return *this;
			}
			auto operator++( int ) -> transforming_range
			{
				transforming_range tmp{ *this };
				operator++();
				return tmp;
			}

			explicit operator bool() const { return !!r; }

	};

	template< typename range_type, typename transformation >
	auto select( range_type r, transformation fn ) -> transforming_range< range_type, transformation >
	{
		return transforming_range< range_type, transformation >{ r, fn };
	}

	template< typename transformation >
	auto select( transformation fn ) -> range_factory< transforming_range, transformation >
	{
		return make_range_factory< transforming_range >( fn );
	}

	template< typename range_type, typename unary_predicate >
	class filtering_range
	{
		private:
			void find_next_match() const
			{
				while( r && !fn( *r ) )
					++r;
			}
			
			mutable range_type r;
			unary_predicate fn;

		public:
			filtering_range( range_type r, unary_predicate fn ) : r{ r }, fn{ fn } 
				{ }

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
				filtering_range tmp{ *this };
				operator++();
				return tmp;
			}

			explicit operator bool() const { find_next_match(); return !!r; }

	};

	template< typename range_type, typename unary_predicate >
	auto where( range_type range, unary_predicate fn ) -> filtering_range< range_type, unary_predicate >
	{
		return filtering_range< range_type, unary_predicate >{ range, fn };
	}

	template< typename unary_predicate >
	auto where( unary_predicate fn ) -> decltype( make_range_factory< filtering_range >( fn ) )
	{
		return make_range_factory< filtering_range >( fn );
	}

	template< typename range_type >
	class limited_range
	{
		private:
			void skip()
			{
				while( r && count < 0 )
				{
					++r;
					++count;
				}
			}
			range_type r;
			int count;

		public:
			limited_range( range_type r, int count ) 
				: r{ r }, count{ count }
			{ 
				skip();
			}

			auto operator*() const -> decltype( *r )
			{ 
				return *r; 
			}
			auto operator++() -> limited_range & 
			{ 
				if( r )
				{
					++r;
					--count; 
				}
				return *this; 
			}
			auto operator++( int ) -> limited_range 
			{ 
				limited_range tmp{ *this }; 
				operator++(); 
				return tmp; 
			}
			explicit operator bool() const
			{ 
				return !!r && count > 0; 
			}
	};

	template< typename range_type >
	auto take( range_type range, size_t count ) -> limited_range< range_type >
	{
		return limited_range< range_type >{ range, static_cast< int >( count ) };
	}

	auto take( size_t count ) -> decltype( make_range_factory< limited_range >( static_cast< int >( count ) ) )
	{
		return make_range_factory< limited_range >( static_cast< int >( count ) );
	}


	template< typename range_type, typename range_factory_type >
	auto operator|( range_type range, range_factory_type factory ) -> decltype( factory( range ) )
	{
		return factory( range );
	}

	template< typename range_type, typename value_type >
	class range_output_iterator : public std::iterator< std::input_iterator_tag, value_type >
	{
		public:
			range_output_iterator( const range_type & r ) 
				: r{ r } 
			{
			}

			auto operator!=( const range_output_iterator & r ) const -> bool
			{
				return !operator==( r );
			}

			auto operator==( const range_output_iterator & ) const -> bool
			{
				return !r;
			}

			auto operator++() -> range_output_iterator & 
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
	auto begin( range_type r ) -> range_output_iterator< range_type, decltype( *r ) >
	{
		return range_output_iterator< range_type, decltype( *r ) >{ r };
	}
	template< typename range_type >
	auto end( range_type r ) -> range_output_iterator< range_type, decltype( *r ) >
	{
		return range_output_iterator< range_type, decltype( *r ) >{ r };
	}

}


