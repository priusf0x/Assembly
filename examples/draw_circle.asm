#var_1 = 2900
#var_2 = 2901
#var_3 = 2902
#...

call fill_screen
draw

hlt
fill_screen:
    next_string:
        push 0
        pop RAX

        print_string:
            call convert_to_index
            call check_if_in_circle
            push R0X
            pop [RCX]

            push RAX
            push 1
            add
            pop RAX

            push RAX
            push 97

        jb print_string:

        call convert_to_index
        push 10
        pop [RCX]

        push RBX
        push 1
        add
        pop RBX

        push RBX
        push 36

        jbe next_string:
ret

convert_to_index:
    push RBX
    push 98
    mul
    push RAX
    add
    pop RCX
ret

check_if_in_circle:
    push RAX
    push 49
    sub
    push RAX
    push 49
    sub
    mul
    push RBX
    push 18
    sub
    push RBX
    push 18
    sub
    mul
    add
    push 50
    ja skip_label_1:
        push 64
        pop R0X
        jmp meow:
    skip_label_1:
        push 32
        pop R0X
    meow:
ret
