#include <narl.h>


namespace select_where
{

	template< typename range, typename filter, typename xform >
	size_t range_test( range r, filter f, xform x ) 
	{ 
		return r | where( f ) | select( x ) | count();
	}

}
