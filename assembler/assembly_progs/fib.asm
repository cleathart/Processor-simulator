#generates a fibancci value upto that store intially in r0

ldc r0 c46
ldc r1 c0
ldc r2 c0
ldc r3 c0
ldc r4 c1

while_label :
cmp r5 r0 r1
be r5 break_label

add r2 r3 r4
sto r1 r3
mov r3 r4 
mov r4 r2 
addi r1 r1 c1

j while_label

break_label :
sto r0 r3