; Load known values from memory
LoadDirect r0, 0
LoadDirect r1, 100
; Store known values to memory and read back
LoadImmediate r4, 71
StoreDirect r4, 101
LoadDirect r2, 101
LoadImmediate r4, -19
StoreDirect r4, 102
LoadDirect r3, 102
; Test indirect operations
LoadImmediate r4, 103
LoadImmediate r1, 37
StoreIndirect r4, r1
LoadImmediate r4, 102
LoadIndirect r1, r4
AddImmediate r1, r1, 1
LoadImmediate r4, 104
StoreIndirect r4, r1
Halt