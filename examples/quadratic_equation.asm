in
in
in
pop RCX
pop RBX
pop RAX
call calculate_discriminant
push RDX
push 0
ja case_1:
    jmp meow_1:
    case_1:
        push 0
        push RBX
        sub
        push RDX
        sqrt
        add
        push RCX
        div
        out

        push 0
        push RBX
        sub
        push 0
        push RDX
        sqrt
        sub
        add
        push RCX
        div
        out
meow_1:

push RDX
push 0
je case_2:
    jmp meow_2:
    case_2:
        push 0
        push RBX
        sub
        push RCX
        div
        out
meow_2:

hlt



calculate_discriminant:
    push RBX
    push RBX
    mul
    push 4
    push RAX
    push RCX
    mul
    mul
    sub
    pop RDX
ret

