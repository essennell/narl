# distinct except intersect_with union_with

## distinct

This operation returns a range where duplicates in the input have been removed. The input range must be ordered. The ```distinct``` operation can optionally take a comparison function which defaults to ```std::less```. A strictly weak-orderd input is required.

If a custom comparer is provided, it should ideally be the same comparer used to sort the input (it **must** return the same for any 2 elements as that used to sort the range).

See also [sorted](select.md).

```c++
auto unique = from( { 1, 1, 2, 3, 3 } ) | distinct();

struct item
{
	int id;
	std::string name;
};

std::vector< item > data; // unordered
auto r = from( data ) 
	| sorted( []( const item & l, const item & r ) { return l.id < r.id; } ) 
	| distinct( []( const item & l, const item & r ) { return l.id < r.id; } );
```

## except

This operation produces the set difference between the source range and another input range. Both ranges must be ordered, and as with ```distinct```, the comparison for ordering should be the same as that used to order the ranges. By default, ```std::less``` is used if none is provided.

Items from the first range not in the second are returned.

Where an item occurs multiple times in either range, they are matched to the second range individually (as for std::set_difference). For example, if an item occurs ```m``` times in the left range and ```n``` times in the right range, the result output will contain ```m-n``` occurrences of it.


```c++
auto diff = from( { 1, 2, 2, 3, 4 } ) | except( from( { 2, 4, 8 } ) );

// produces { 1, 2, 3 }
```

## intersect_with

This operation produces the set intersection between the source range and another input range. Both ranges must be ordered, and as with ```distinct```, the comparison for ordering should be the same as that used to order the ranges. By default, ```std::less``` is used if none is provided.

items from the first range that are also in the second range are returned.

Where an item occurs multiple times in either range, they are matched individually (as for std::set_intersection). So, if the left range has ```m``` occurrences of an item, and the right range has ```n``` occurrences, the result will contain the minimum of ```m``` and ```n```.

```c++
auto common = from( { 1, 2, 2, 3, 4 } ) | intersect_with( from( { 2, 2, 4, 4, 8 } ) );

// Produces { 2, 2, 4 }
```

## union_with

This operation produces the set union between the source range and another input range. Both ranges must be ordered, and as with ```distinct```, the comparison for ordering should be the same as that used to order the ranges. By default, ```std::less``` is used if none is provided.

Where an item occurs multiple times in either range, then all elements from the left range are returned, along with any remaining items from the right range not already matched to an item. E.g. if an item occurs ```m``` times in the left range and ```n``` times in the right range, the returned range contains ```m+std::max(n-m,0)``` occurrences.

```c++
auto both = from( { 1, 2, 2, 3, 4 } ) | union_with( from( { 2, 4, 4, 8 } ) );

// Produces { 1, 2, 2, 3, 4, 4, 8 }
```

[Home](../README.md)
[Index](../README.md#Usage)

