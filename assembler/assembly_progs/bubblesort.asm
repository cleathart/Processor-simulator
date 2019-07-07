ldc r10 c0
ldc r9 c0
ldc r11 c9
ldc r0 c1
ldc r1 c-1

check :
addi r9 r9 c1
mov r13 r10
cmp r12 r11 r9
cmp r5 r1 r12
be r12 end

main :
cmp r14 r11 r13
be r14 check

ldr r0 r13
addi r4 r13 c1
ldr r1 r4
cmp r2 r0 r1
cmp r3 r2 r0
be r3 increment
j check

increment :
sto r13 r1
addi r13 r13 c1
sto r13 r0
j main

end :
ldc r14 c11