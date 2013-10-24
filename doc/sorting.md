# sorted merge_with

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

## merge_with


This operation merges two already ordered ranges into a single ordered range.

```c++
auto left = from( { 1, 2, 4, 8, 16 };
auto r = left | merge( from( { 2, 3, 4, 5, 6, 7, 8 } ) );

//Produces { 1, 2, 3, 4, 5, 6, 7, 8, 16 }
```

## reverse

Does exactly what it says on the tin - produces the elements of the input range in reverse order. Notably, this operation does **not** buffer the input - it is a streaming operation.

```c++
auto reversed = from( { 1, 2, 3 } ) | reverse()

// Produces { 3, 2, 1 }
```

[Home](../README.md)
[Index](../README.md#Usage)
