has_err = [8, 10, 11, 12, 13, 14, 17, 30]
# all gprs?
'''
save_reg = [
    "rax",
    "rbx",
    "rcx",
    "rdx",
    "rdi",
    "rsi",
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
    "rax",
    "rcx",
    "rdx",
    "rsi",
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
    {save}

    ; C args (irq, err)
    mov rdi, {i}
    mov rsi, [rsp]
    add rsp, 8

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
        