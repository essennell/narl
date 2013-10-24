# narl

Not Another Range Library - Overload at [ACCU](http://accu.org): an article introducing the library - [Range and Elevation](http://accu.org/var/uploads/journals/Overload117.pdf)

#Get started

Narl is a header-only library for C++. You'll need gcc 4.8 or better, or MS Visual Studio 2013. Not all the features are available in Visual Studio. The code also compiles with Clang 3.2, with some warnings.

Add the ```src/narl``` folder to you search path and include ```<narl.h>```.

The library is based around the idea of Ranges, but for the most part, these aren't directly used. Instead, you compose expressions from Narl, and let the compiler figure out what the result is. 

There are lots of expressions which you can compose together, using the | operator. Here is a simple filtering expression:

For example:

```c++
#include <narl.h>

#include <iostream>
#include <string>
#include <vector>

using namespace narl;

int main( int argc, char *const argv[] )
{
	std::vector< std::string > v { argv, argv + argc };

	auto r = from( src ) | where( []( const item & i ) { return i.size() > 0; } );

	for( auto const & arg : r )
	{
		std::cout << arg;
	}
}
```

You can use initializer lists to initialize a range too: ```from( { 1, 2, 3 } )```.

Expressions can be arbitrarily complex, and are lazily evaluated left-to-right:

```c++
auto r = from( src ) 
	| where( []( const item & i ) { return i.size() > 0; } )
	| select( []( const item & i ) { return i.name(); } );
```

Lastly, you can "re-hydrate" a range back into a standard container with ```to```:

```c++
auto r = from( { 1, 2, 3 } ) 
	| select( []( int i ) { return std::to_string( i ); } ) 
	| to< std::list >();

	// or to< std::vector >() for example
```

## Usage

The full list of implemented expressions is:

[from](doc/range.md#from) [to](doc/range.md#to) [make_range](doc/range.md#make_range) [fill_range](doc/range.md#fill_range) [range](doc/range.md#range) [aggregate](doc/aggregate.md#aggregate) [all](doc/anyall.md#all) [any](doc/anyall.md#any) [concat](doc/select.md#concat) [count](doc/aggregate.md#count) [distinct](doc/setops.md#distinct) [except](doc/setops.md#except) [groupby](doc/groupby.md#groupby) [join](doc/groupby.md#join)* [intersect_with](doc/setops.md#intersect_with) [merge_with](doc/sorting.md#merge_with) [reverse](doc/select.md#reverse) [select](doc/select.md#select) [selectmany](doc/select.md#selectmany) [sequenceequal](doc/aggregate.md#sequenceequal) [skip](doc/skiptake.md#skip) [skipwhile](doc/skiptake.md#skipwhile) [sorted](doc/sorting.md#sorted) [take](doc/skiptake.md#take) [takewhile](doc/skiptake.md#takewhile) [union_with](doc/setops.md#union_with) [zipwith](doc/select.md#zipwith)

(*join does not work in VS2013)

For some example code, see [narl_stl.cpp](https://github.com/essennell/narl/tree/master/src/tests/testnarl/narl_stl.cpp) which has some narl expressions showing implementations of C++ standard algorithms.

## Building the tests

Narl uses [Catch](https://github.com/philsquared/Catch) which is referenced as an external git project in externals/catch

You'll need to initialize and update the submodule reference if you fork narl.

Test sources are in the ```src/tests/testnarl``` folder. There is a makefile for g++ and a .sln file for Visual Studio in ```src```.
