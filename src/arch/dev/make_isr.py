has_err = [8, 10, 11, 12, 13, 14, 17, 30]
# all gprs?
'''
save_reg = [
    "rsi",
    "rax",
    "rbx",
    "rcx",
    "rdx",
    "rdi",
    "rbp",
    "rsp",
    "r8",
    "r9",
    "r10",
    "r11",
    "r12",
    "r13",
    "r14",
    "r15"
]
'''

save_reg = [
    "rsi",
    "rax",
    "rcx",
    "rdx",
    "rdi",
    "r8",  
    "r9",    
    "r10",
    "r11",
]

ISR_name = "IRQ_call"

save = "\n\t".join("push "+reg for reg in save_reg)
restore = "\n\t".join("pop "+reg for reg in save_reg[::-1])


with open("isr.asm", "w") as f:
    f.write(f"extern {ISR_name}\n")
    for i in range(256):
        if i in has_err:
            f.write(
f'''
isr_{i}:
    push rsi
    push rax
    mov rsi, [rsp + 16]
    mov rax, [rsp + 8]
    mov [rsp + 16], rax
    mov rax, [rsp]
    mov [rsp + 8], rax
    add rsp, 8
    push rcx
    push rdx
    push rdi
    push r8
    push r9
    push r10
    push r11

    ; C args (irq, err)
    mov rdi, {i}

    call {ISR_name}

    {restore}
    iretq
'''
        )
        else:
            f.write(
f'''
isr_{i}:
    {save}

    ; C args (irq, err)
    mov rdi, {i}

    call {ISR_name}

    {restore}
    iretq
'''
            )
    f.write("\nglobal isr_stub_table\nisr_stub_table:\n")
    for i in range(256):
        f.write(f"dq isr_{i}\n")
        