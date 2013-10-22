# select where reverse selectmany sorted zipwith

## select

This is the transforming operation. It takes the elements of an input range and produces elements transformed from them. The resulting elements may have different values, or perhaps a different type. 

```c++
auto transformed = from( { 1, 2, 3 } ) | select( []( int i ) { return std::to_string( i * 10 ) } );

// Produces { "10", "20", "30" }
```

## where

This is the basic filtering operation which only returns elements which match the provided predicate.

```c++
auto filtered = from( { 1, 2, 3, 4 } ) | where( []( int i ) { return i % 2 == 0; } );

// Produces { 2, 4 }
```

## reverse

Does exactly what it says on the tin - produces the elements of the input range in reverse order. Notably, this operation does **not** buffer the input - it is a streaming operation.

```c++
auto reversed = from( { 1, 2, 3 } ) | reverse()

// Produces { 3, 2, 1 }
```

## sorted

The operation sorts the input range according to the provided comparison operation. If no comparison is provided, ```std::less``` is used. The comparison operation should take two elements and return ```true``` if the left is less than the right.

This operation buffers the input range and copies the elements, so introduces a break in a streamed expression. 

The sorting is stable. The buffer is a ```std::list``` and sorting uses the list''s ```sort``` member function.

```c++
auto r = from( { 3, 2, 1 } ) | sorted();

// Produces { 3, 2, 1 }
```

Not recommended, but this produces a reverse-ordered range:
```c++
auto r = from( { 1, 2, 3 } ) | sorted( []( int left, int right ) { return right < left; } );

// Produces { 1, 2, 3 }
```

## selectmany

This is a flattening operation, and is similar to the ```bind``` operation for amplified types in functional programming. The provided expression needs to return a range; the ranges from each of the original elements are then "flattened" into a single range of individual elements from those ranges (as if they were concatenated).

```c++
struct item
{
	int id;
	std::vector< std::string > subitems;
};
std::vector< item > items { 
	item { 1, { "1", "One" } },
	item { 1, { "2", "Two" } },
	item { 1, { "3", "Three" } },
};
auto r = from( items ) | selectmany( []( const item & i ) { return from( i.subitems ); } );

// Produces { "1", "One", "2", "Two", "3", "Three" }
```

## zipwith

This operation takes an input range and associates it with one (or more) other ranges such that for 2 ranges r1 and r2, 
the new result range''s elements are (r1[0],r2[0]), (r1[1],r2[1]), ... (r1[n],r2[n]).

Zipping multiple ranges is not supported in Visual Studion 2013.

```c++
auto zipped = from( { 1, 2, 3 } ) | zipwith( from( { "1", "2", "3" } ) );
auto zipped_n = from( { 1, 2, 3 } ) | zipwith( from( { "1", "2", "3" } ), from( { "One", "Two", "Three" } ) );

```

[Home](../README.md)
[Index](../README.md#Usage)
