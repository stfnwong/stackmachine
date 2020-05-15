; ALL_INSTR
; Just a list of all instructions supported by this stack machine
;
; Stefan Wong 2020

; N# is the #th element of the stack
; Also in this version, I will use long words for mnemonics (eg: STORE rather than !)

ADD             ; Add N1 and N2. Not sure that the register names should start with N right now...
STORE N1 $60    ; Store top of stack at address 0x60
SUB             ; Subtract N2 from N1, push result to N3
RPUSH           ; Push N1 onto top of return stack
FETCH $500      ; Fetch the value at memory address 0x500
AND             ; And N1 and N2, push result to N3
DROP            ; Drop N1 from stack
DUP             ; Duplicate N1, pushing a copy of it onto the stack
OR              ; Perform bitwise OR of N1 and N2, pushing onto stack
OVER            ; Push a copy of the second element to top of stack (N1 N2 -> N1 N2 N1)
RPOP            ; Pop top element of return stack and push onto data stack as N1
SWAP            ; Swap the order of N1, N2 and place on the stack
XOR             ; Perform bitwise XOR of N1, N2, placing result in N3
IF              ; If N1 is false (value is 0) perform branch to address in next program cell
CALL            ; Perform subroutine call to address in next cell
EXIT            ; Perform subroutine return
LIT             ; Treat the value in next program cell as a literal
