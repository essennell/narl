#pragma once

#include <memory>


namespace narl
{

	template< typename value_type >
	class range
	{

		public:
			range()
			{
			}

			range( const range & other )
				: r{ other.r ? other.r->clone() : nullptr }
			{
			}

			template< typename range_type >
			range( const range_type & other )
				: r{ std::unique_ptr< range_wrapper< range_type > >{ new range_wrapper< range_type >{ other } } }
			{
			}

			template< typename range_type >
			auto operator=( const range_type & other ) -> range &
			{
				range tmp{ other };
				tmp.r.swap( r );
				return *this;
			}

			auto operator=( const range & other ) -> range &
			{
				range tmp { other };
				tmp.r.swap( r );
				return *this;
			}
			

			auto operator*() const -> value_type
			{
				return r->current();
			}

			auto operator++() -> range &
			{
				r->next();
				return *this;
			}

			auto operator++( int ) -> range
			{
				range tmp{ *this };
				++*this;
				return tmp;
			}

			auto operator--() -> range &
			{
				r->previous();
				return *this;
			}

			auto operator--( int ) -> range
			{
				range tmp{ *this };
				--*this;
				return tmp;
			}

			explicit operator bool() const
			{
				return r && r->valid();
			}

			void goto_end()
			{
				r->goto_end();
			}


		private:

			class range_adapter
			{
				public:
					virtual std::unique_ptr< range_adapter > clone() const = 0;
					virtual value_type current() const = 0;
					virtual void next() = 0;
					virtual void previous() = 0;
					virtual bool valid() const = 0;
					virtual void goto_end() = 0;
			};

			template< typename range_type >
			class range_wrapper : public range_adapter
			{
				public:
				range_wrapper( const range_type & r )
					: r{ r }
				{
				}

				virtual std::unique_ptr< range_adapter > clone() const override
				{
					return std::unique_ptr< range_adapter >{ new range_wrapper< range_type >( r ) };
				}

				virtual value_type current() const
				{
					return *r;
				}
				virtual void next()
				{
					++r;
				}
				virtual void previous()
				{
					--r;
				}
				virtual bool valid() const
				{
					return !!r;
				}
				virtual void goto_end()
				{
					r.goto_end();
				}

				range_type r;
			};

			std::unique_ptr< range_adapter > r;

	};

}
