; Projectile motion simulation
; Intial conditions are loading into the following memory locations
; 100 x
; 101 y
; 102 x velocity
; 103 y velocity
; 104 x acceleration
; 105 y acceleration
; X
LoadDirect r0, 100
LoadDirect r1, 102
LoadDirect r2, 104
Add r1, r2
Add r0, r1
StoreDirect r0, 100
StoreDirect r1, 102

; Y
LoadDirect r0, 101
LoadDirect r1, 103
LoadDirect r2, 105
Add r1, r2
Add r0, r1
StoreDirect r0, 101
StoreDirect r1, 103

; Jump out when y is back to 0
BranchOnEqual r0, r4, 20
Jump 0
