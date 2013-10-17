#pragma once

#include "range_factory.h"
#include "range_input_iterator.h"


namespace narl
{

	template< template< typename... > class container_type >
	class to_container
	{

		public:

			template< typename range_type >
			auto operator()( const range_type & r ) const 
#ifndef _MSC_VER
				-> container_type< decltype( *r ) >
#else
				-> container_type< decltype( *r ), std::allocator< decltype( *r ) > >
#endif
			{
#ifndef _MSC_VER
				return container_type< decltype( *r ) > { begin( r ), end( r ) };
#else
				return container_type< decltype( *r ), std::allocator< decltype( *r ) > > { begin( r ), end( r ) };
#endif
			}
			
	};


	template< template< typename... > class container >
	auto to() -> decltype( to_container< container >() )
	{
		return to_container< container >();
	}

}


