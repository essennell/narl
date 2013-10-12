#pragma once


namespace narl
{

	template< template< typename > class range_type >
	class range_0_factory
	{

		public:
			template< typename range_of >
			auto operator()( const range_of & r ) const -> range_type< range_of >
			{
				return range_type< range_of >{ r };
			}

	};

	template< template< typename > class range_type, typename argument_type >
	class range_1_factory
	{

		public:
			range_1_factory( const argument_type & expr )
				: expr{ expr }
			{
			}

			template< typename range_of >
			auto operator()( const range_of & r ) const -> range_type< range_of >
			{
				return range_type< range_of >{ r, expr };
			}


		private:
			argument_type expr;

	};

	template< template< typename, typename > class range_type, typename argument_type >
	class range_2_factory
	{

		public:
			range_2_factory( const argument_type & arg )
				: arg( arg )
			{
			}


			template< typename range_of >
			auto operator()( const range_of & r ) const -> range_type< range_of, argument_type >
			{
				return range_type< range_of, argument_type >{ r, arg };
			}


		private:
			argument_type arg;

	};

	template< template< typename... > class range_type, typename... others >
	class range_N_factory;

	template< template< typename... > class range_type, typename argument_type >
	class range_N_factory< range_type, argument_type >
	{

		public:
			range_N_factory( const argument_type & arg )
				: arg( arg )
			{
			}


			template< typename range_of, typename... others >
			auto build( const range_of & r, const others &... rest ) const -> range_type< range_of, others..., argument_type >
			{
				return range_type< range_of, others..., argument_type >{ r, rest..., arg };
			}

			template< typename range_of >
			auto operator()( const range_of & r ) const -> decltype( this->build( r ) )
			{
				return build( r );
			}


		private:
			argument_type arg;

	};

	template< template< typename... > class range_type, typename next_type, typename... others >
	class range_N_factory< range_type, next_type, others... > : public range_N_factory< range_type, others... >
	{
		typedef range_N_factory< range_type, others... > base;

		private:
			next_type next;

		public:
			range_N_factory( const next_type & next, const others &... rest )
				: base{ rest... }, next{ next }
			{
			}

		
			template< typename range_of >
			auto operator()( const range_of & r ) const -> decltype( this->base::build( r, next ) )
			{
				return base::build( r, next );
			}

			template< typename range_of, typename... tail >
			auto build( const range_of & r, tail &... rest ) const -> decltype( this->base::build( r, rest..., next ) )
			{
				return base::build( r, rest..., next );
			}

	};


	template< template< typename > class range_type >
	auto make_factory()
		-> range_0_factory< range_type >
	{
		return range_0_factory< range_type >{ };
	}

	template< template< typename > class range_type, typename argument_type >
	auto make_factory( const argument_type & expr )
		-> range_1_factory< range_type, argument_type >
	{
		return range_1_factory< range_type, argument_type >{ expr };
	}

	template< template< typename, typename > class range_type, typename argument_type >
	auto make_factory( const argument_type & arg ) 
		-> range_2_factory< range_type, argument_type >
	{
		return range_2_factory< range_type, argument_type >{ arg };
	}

#ifdef _MSC_VER

	template< template< typename, typename, typename > class range_type, typename other_range_type, typename argument_type >
	class range_3_factory
	{

		public:
			range_3_factory( const other_range_type & o, const argument_type & arg )
				: o( o ), arg( arg )
			{
			}


			template< typename range_of >
			auto operator()( const range_of & r ) const -> range_type< range_of, other_range_type, argument_type >
			{
				return range_type< range_of, other_range_type, argument_type >{ r, o, arg };
			}


		private:
			other_range_type o;
			argument_type arg;

	};

	template< template< typename, typename, typename > class range_type, typename other_range_type, typename argument_type >
	auto make_factory( const other_range_type & o, const argument_type & arg ) 
		-> range_3_factory< range_type, other_range_type, argument_type >
	{
		return range_3_factory< range_type, other_range_type, argument_type >{ o, arg };
	}

#endif

	template< template< typename... > class range_type, typename next_type, typename... argument_type >
	auto make_factory( const next_type & arg, const argument_type &... expr )
		-> range_N_factory< range_type, next_type, argument_type... >
	{
		return range_N_factory< range_type, next_type, argument_type... >{ arg, expr... };
	}

}
