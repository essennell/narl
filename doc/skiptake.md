# skip skip_while take and take_while

## skip

This operation skips over the first ```N``` elements, effectively starting the range on the ```N+1``` element.

```c++
auto r = from( { 1, 2, 3, 4, 5 } ) | skip( 3 );

//produces { 4, 5 }
```

## skip_while

This operation skips over elements *until* the provided predicate is false.

```c++
auto r = from( { 1, 2, 3, 4, 5 } ) | skip_while( []( int i ) { return i <= 3; } );

//produces { 4, 5 }
```

## take

This operation produces ```N``` elements from the input and then stops (becomes invalid)

```c++
auto r = from( { 1, 2, 3, 4, 5 } ) | take( 3 );

//produces { 1, 2, 3 }
```

This is especially useful for limiting the number of elements returned from an otherwise infinite range, produced by [make_range](range.md#make_range) and [fill_range](range.md#fill_range).

## take_while

This operation takes elements from the range until the provided predicate is false.

```c++
auto r = from( { 1, 2, 3, 4, 5 } ) | take_while( []( int i ) { return i <= 3; } );

//produces { 1, 2, 3 }
```

This is especially useful for limiting the number of elements returned from an otherwise infinite range, produced by [make_range](range.md#make_range) and [fill_range](range.md#fill_range).

[Home](../README.md)
[Index](../README.md#Usage)

