fill_screen:
    push 0b01001101010011010100110101001101
    push 0
    pop RAX
    push [RAX]
    push 0b01001101010011010100110101001101
    push 1
    pop RAX
    push [RAX]
    push 0b01001101010011010100110101001101
    push 2
    pop RAX
    push [RAX]
    push 0b01001101010011010100110101001101
    push 3
    pop RAX
    push [RAX]
drawb
hlt



