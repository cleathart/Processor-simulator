# r1 holds the GCD of the numbers in r0 and r1
ldc r0 c21120
ldc r1 c410

mov r2 r0 
mov r3 r0

modulo :
sub r3 r3 r1
blth r3 gcd
j modulo

gcd :
add r2 r3 r1
be r2 end

addi r0 r1 c0
addi r3 r1 c0
addi r1 r2 c0

j modulo

end :
ldc r4 c410