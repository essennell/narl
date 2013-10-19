# aggregate count and sequenceequal

## aggregate

This operation is a general purpose reduction (folding) operation that can be used to sum a collection.

It is an overloaded expression. The simple version just takes an accumulator expression, the other version also takes an initial value as a seed.

```c++
auto sum = from( { 2, 4, 8 } ) | aggregate( []( int curr, int val ) { return curr + val; } );
auto product = from( { 2, 4, 8 } ) | aggregate( 1, []( int curr, int val ) { return curr * val; } );
```

## count

This operation is a specialised form of aggregate that simply returns the number of elements in a range.

```c++
auto num = from( { 1, 2, 3, 5, 8, 13 } ) | where( []( int i ) { return i % 2 == 0; } ) | count();
```

# sequenceequal

This operation is in this section only because it is a variation of a fold that collapses to ```true``` if two ranges have the same elements in the same order.

```c++
auto same = from( { 1, 2, 3 } ) | sequenceequal( from( { 1, 2, 3 } ) );
```

[HOME](../README.md)
