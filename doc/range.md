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

[Home](../README.md)
