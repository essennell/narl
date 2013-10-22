# any and all

## any

This operation returns ```true``` if the given predicate returns ```true``` for any element in the range. If no predicate is given, the semantics are that it returns ```true``` if there are any elements.

```c++
bool exists = from( { 1, 2, 3 } ) | any( []( int i ) { return i % 2 == 0; } );
bool overlap - from( { 1, 2, 3 } ) | intersect_with( from( { 4, 5, 6 } ) ) | any();
```

(See [Set operations](setops.md) for ```intersect_with```)

## all

This operation returns ```true``` if all of the elements of a range match the given predicate.

```c++
bool even = from( { 2, 4, 6 } ) | all( []( int i ) { return i % 2 == 0; } );
```

It is equivalent to a negated ```any``` for the same range and inverted predicate, i.e.:
```c++
bool odd = !( from( { 2, 4, 6 } ) | any( []( int i ) { return i % 2 != 0; } ) );
```

[Home](../README.md)
