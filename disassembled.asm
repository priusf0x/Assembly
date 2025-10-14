push 1
pop RAX
call 20
push RAX
push 1
add
pop RAX
push RAX
push 10
jbe 4
hlt
push RAX
push RAX
mul
out
ret
