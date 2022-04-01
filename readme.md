Emulator Memory
===============

This project requires the implementation of a very simple processor to execute instructions. The instruction format used is intended to be very simple (while not being space efficient).

The project extends a previous project to add memory access and simple program to sum values stored in a memory array.

Operations
----------

This simple processor supports the following operations:

1. Halt - Stop the processor
2. LoadImmedate - Store the value in `immediate` to register `dst`.
3. Add - Add the value from register `src` to the value in register `dst` and store the result in `dst`.
4. AddImmediate - Add the value in `immediate` to register `src` and store the result in register `dst`.
5. Move - Copy the contents of register `src` to register `dst`.
6. And - Bitwise `and` the value from register `src` to the value in register `dst` and store the result in `dst`.
7. Or - Bitwise `or` the value from register `src` to the value in register `dst` and store the result in `dst`.
8. Xor - Bitwise `xor` the value from register `src` to the value in register `dst` and store the result in `dst`.
9. Jump - Jump to the absolute position stored in the `immediate` value.
10. BranchOnEqual - Jump to the absolute position stored in the `immediate` value if and only if the values in the `src` and `dst` registers are equal.
11. LoadDirect - Load the value stored in the memory location pointed to by `immediate` into the `dst` register.
12. StoreDirect - Store the value stored in register `src` into the memory location pointed to by the `immediate` value.
13. LoadIndirect - Load the value stored in the memory location pointed to by the value in `src` into the `dst` register.
14. StoreIndirect - Store the value stored in register `src` into the memory location pointed to by the value stored in `dst`.

It has no access to external I/O.

Array Sum
---------

The array-sum.asm should add a scaler to every item in an array. The values for this code segment are:

- Memory location 100: value to add to each item in the array
- Memory location 101: length of the array
- Memory location 102: first array item
- Memory location 103: second array item
- Memory location 104+: subsequent array items

Running
-------

The emulator can be compiled and tested by invoking:

```
make
```
