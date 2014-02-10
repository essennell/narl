# Range operations

There are several range types produced by calling one or more of the various expression functions declared elsewhere in the docs. The range types aren''t directly related, except by behaviour: the set of common operations for some given range object ```r``` are as follows:

```c++
item_type v = *r; // dereference the current position to get the value
static_cast< bool >( r );// determine if the range is valid

++r; // increment the range object by one position
auto copy = r++; // increment the range but return the position prior to the increment...
item_type v1 = *r++; // Get the current element and increment the range in one go
--r; // decrement the range position
auto copy = r--; // decrement the range position but return the position prior to the decrement...
item_type v2 = *r--; // Get the current element and decrement the range in one go
```

Attempting to dereference a range that is invalid (i.e. ```static_cast< bool >( r ) == false```) results in a ```range_access_exception```. NB: a range can be made invalid by incrementing past the last element or decrementing past the first element. If the range has elements, then decrementing or incrementing (respectively) makes the range "valid" once more.

## from

This is the operation to create a range object. Ranges can be created from a standard initialization list, or any collection which can be used in a range-based (different range!) for loop (i.e. any ```x``` for which ```begin( x )``` and ```end( x )``` are well-defined).

The ```from``` operation handles temporaries by moving the argument into a managed ```std::vector``` of the correct value type. Calling ```from``` with a named object (lvalue) produces a ```range``` object that directly references that object through the iterator pair obtained by calling ```begin``` and ```end``` for it.

```c++
auto r1 = from( { 1, 2, 3 } ); // r-value init list, creates a vector of ints to reference

auto i = { 1, 2, 3 };
auto r2 = from( i ); // l-value init list, references the contents of i directly

auto r3 = from( std::vector< std::string >() ); // r-value container (empty), which is moved to an owned-container

std::list< double > d { 0.1, 2.3, 4.5 };
auto r4 = from( d ); // l-value container of doubles, r4 references d iterators directly
```

## to

This is the operation to turn a ```range``` back into a standard container. Currently only ```std::vector```, ```std::list```, ```std::deque``` and ```std::forward_list``` are supported.

```c++
auto r = from( { 1, 2, 3 } );
auto v = r | to< std::vector >();
auto l = r | to< std::list >();
auto d = r | to< std::deque >();
auto f = r | to< std::forward_list >();
```

## make_range

This is an one of two alternate ways to make a range, and has three overloads.
(1) makes a default range from a type, which must support ```++``` to get the next value. This produces (effectively) an infinite range.
(2) is the same type of default range, but begins at a provided value

```c++
auto def = make_range< int >();
auto init = make_range< int >( 5 );

// def Produces { 0, 1, 2, 3, ... std::numeric_limits< int >::max}
// init produces { 5, 6, 7, ... std::numeric_limits< int >::max}
```
(3) This third overload can be used to provide a completely user-defined range. Whenever the resulting range is incremented (using ```++```), the callback is invoked to get the next item. The previous value is passed to callback function. An initial value is passed in to the ```make_range``` function.

```c++
auto custom = make_range< int >( 2, []( int i ) { return i + 5; } );

// Produces { 2, 10, 50, 2500, ... }
```

## fill_range

This is a variant of ```make_range``` which fills a range of given type with a single value.

```c++
auto r = fill_range< int >( 128 );

// Produces { 128, 128, 128, ... }
```

## range

There is limited support for a local named variable of ```range``` type that can be used to refer to any range object. It supports all the operations of the basic ranges (```*```, ```++```, ```--``` and conversion to ```bool```).

```c++
range< int > r;
r = from( { 1, 2, 3 } ) | select( []( int i ) { return i * 100; } );

for( auto i : r )
{
	std::cout << i << ":";
}
std::cout << "\n";

r = from( { 2, 4, 6, 8, 10, 12 } ) | where( []( int i ) { return i % 3 == 0; } );
for( auto i : r )
{
	std::cout << i << ":";
}
std::cout << "\n";
```

## cycle

Given an input range, produce an infinite range by cycling its contents, i.e. when iterating the range reaches the end, re-start at the beginning

```c++
auto r = from( { 1, 2, 3 } ) | cycle() | take( 5 );
for( auto i : r )
{
	std::cout << i << ":";
}
std::cout << "\n";

// 1:2:3:1:2
```

[Home](../README.md)
[Index](../README.md#Usage)
