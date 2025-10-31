push 1
pop RAX
printsquares:
    call calculate_square
    push RAX #adding 1 to value
    push 1
    add
    pop RAX
    push RAX
    push 10

jbe printsquares:
hlt

#functions

calculate_square:
    push RAX
    push RAX
    mul
    out
ret
hlt
