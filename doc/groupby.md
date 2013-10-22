# groupby and join

## groupby

This operation is a simple joining operation that groups range elements together by a provided key. The resulting range''s elements are composite items comprising the matched key, and a contained range of the elements which match it.

The order of returned items'' keys is not specified.

```c++
// The key selector here produces boolean keys
auto groups = from( { 1, 2, 3, 4 } ) | groupby( []( int i ) { return i % 2 == 0; } );

for( auto kv : groups )
{
	std::cout << std::boolalpha << kv.key << ":";
	for( const auto & val : kv.values )
	{
		std::cout << val << " ";
	}
	std::cout << std::endl;
}
```

## join

This is a more sophisticated joining operation that emulates an ```inner join``` in database parlance. The inputs are an inner and outer range, and a key selection expression for each of them, and a transformation for an element from each of the two provided ranges.

The call to the transformation function is made for each element from inner and outer that match according to their keys. The input ranges can be of different types, but the keys from each must be of the same or a convertible type.

By default, the keys from each range are compared using ```<```. An optional comparer function can also be provided.

```c++
struct left
{
	int id;
	double val;
};
struct right
{
	int id;
	std::string name;
};
std::vector< left > inner 
{
	left{ 0, 0.1 },
	left{ 1, 1.1 },
	left{ 2, 2.1 },
};
std::vector< right > outer
{
	right{ 0, "Zero" },
	right{ 2, "Two" },
	right{ 4, "Four" }
};

auto joined = from( inner ) | join( from( outer ), 
	[]( const left & i ) { return i.id; },			// inner key selector
	[]( const right & i ) { return i.id; },			// outer key selector
	[]( const left & l, const right & r ) 			// transformation
		{ return std::make_tuple( std::get< 0 >( l ), std::get< 1 >( l ), std::get< 1 >( r ); } ) );

// Produces tuple< int, double, string > of
// ( 0, 0.1, "Zero" )
// ( 2, 2.1, "Two" )

```

[Home](../README.md)
[Index](../README.md#Usage)

