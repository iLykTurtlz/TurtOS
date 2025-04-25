
with open("test_stub.asm", "w") as f:
    f.write("extern exception_handler\n")
    for i in range(256):
        f.write(
f'''
isr_test_stub_{i}:
    call exception_handler
    iretq
'''
        )
    f.write("\nglobal isr_stub_table\nisr_stub_table:\n")
    for i in range(256):
        f.write(f"dq isr_test_stub_{i}\n")
        