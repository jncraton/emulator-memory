; Count from 0 to 30 by 3

; Add constant values to registers
LoadImmediate r0, 0
LoadImmediate r1, -1

; Initialize loop counter
LoadImmediate r2, 11

; Initialize loop step
LoadImmediate r3, 3

; Initialize output counter
LoadImmediate r4, 0

; Decrement loop counter (instruction 5)
Add r2, r1

; Check if we're done
BranchOnEqual r0, r2, 9

; Loop body
Add r4, r3

; Loop
Jump 5

; We're done (instruction 9)
Halt
