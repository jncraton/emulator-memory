; Return the nth Fibonacci number 
; n is passed via r0
; Output value is returned via r0

; Simple case
; The first 2 numbers are equal to n, so we simply Halt
LoadImmediate r1, 0
LoadImmediate r2, 1
BranchOnEqual r0, r1, 12
BranchOnEqual r0, r2, 12

; Iterative case
; r1 - previous value (already set to 0)
; r2 - current value (already set to 1)
; r3 - tmp
; r4 - always 1 (loop condition)
LoadImmediate r4, 1

; Loop body (instruction 5)
; Add previous and current and update previous and current values
Move r3, r2
Add r2, r1
Move r1, r3

; Update loop counter
AddImmediate r0, -1
BranchOnEqual r0, r4, 11
Jump 5

; Finish (instruction 13)
Move r0, r2
Halt
