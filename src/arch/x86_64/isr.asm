extern IRQ_call

isr_0:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
xor rdx, rdx
; C args (irq, err)
    mov rdi, 0

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_1:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 1

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_2:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 2

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_3:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 3

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_4:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 4

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_5:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 5

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_6:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 6

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_7:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 7

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_8:
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

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 8

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_9:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 9

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_10:
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

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 10

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_11:
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

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 11

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_12:
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

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 12

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_13:
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

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 13

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_14:
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

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 14

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_15:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 15

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_16:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 16

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_17:
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

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 17

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_18:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 18

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_19:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 19

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_20:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 20

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_21:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 21

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_22:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 22

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_23:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 23

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_24:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 24

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_25:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 25

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_26:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 26

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_27:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 27

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_28:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 28

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_29:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 29

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_30:
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

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 30

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_31:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 31

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_32:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 32

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_33:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 33

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_34:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 34

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_35:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 35

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_36:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 36

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_37:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 37

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_38:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 38

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_39:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 39

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_40:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 40

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_41:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 41

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_42:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 42

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_43:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 43

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_44:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 44

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_45:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 45

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_46:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 46

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_47:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 47

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_48:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 48

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_49:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 49

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_50:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 50

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_51:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 51

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_52:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 52

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_53:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 53

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_54:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 54

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_55:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 55

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_56:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 56

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_57:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 57

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_58:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 58

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_59:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 59

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_60:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 60

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_61:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 61

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_62:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 62

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_63:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 63

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_64:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 64

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_65:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 65

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_66:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 66

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_67:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 67

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_68:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 68

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_69:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 69

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_70:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 70

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_71:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 71

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_72:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 72

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_73:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 73

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_74:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 74

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_75:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 75

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_76:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 76

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_77:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 77

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_78:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 78

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_79:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 79

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_80:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 80

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_81:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 81

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_82:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 82

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_83:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 83

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_84:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 84

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_85:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 85

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_86:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 86

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_87:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 87

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_88:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 88

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_89:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 89

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_90:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 90

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_91:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 91

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_92:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 92

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_93:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 93

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_94:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 94

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_95:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 95

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_96:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 96

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_97:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 97

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_98:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 98

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_99:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 99

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_100:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 100

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_101:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 101

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_102:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 102

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_103:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 103

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_104:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 104

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_105:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 105

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_106:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 106

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_107:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 107

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_108:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 108

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_109:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 109

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_110:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 110

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_111:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 111

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_112:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 112

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_113:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 113

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_114:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 114

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_115:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 115

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_116:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 116

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_117:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 117

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_118:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 118

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_119:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 119

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_120:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 120

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_121:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 121

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_122:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 122

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_123:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 123

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_124:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 124

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_125:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 125

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_126:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 126

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_127:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 127

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

; for yield
isr_128:
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push rbp

	;this doesn't work
	;push ds
	;push es
	;push fs
	;push gs

	;solution
	xor rax, rax
	mov rax, ds
	push rax
	mov rax, es
	push rax
	mov rax, fs
	push rax
	mov rax, gs
	push rax
    

    ; C args (irq, err, arg)
    mov rdi, 128
	mov rdx, rsp

    call IRQ_call

	pop rax
	mov gs, ax
	pop rax
	mov fs, ax
	pop rax
	mov es, ax
	pop rax
	mov ds, ax


	pop rbp
	pop r15
	pop r14
	pop r13
	pop r12
    pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
    iretq

; for kexit
isr_129:
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push rbp

	;this doesn't work
	;push ds
	;push es
	;push fs
	;push gs

	;solution
	xor rax, rax
	mov rax, ds
	push rax
	mov rax, es
	push rax
	mov rax, fs
	push rax
	mov rax, gs
	push rax
    

    ; C args (irq, err, arg)
    mov rdi, 129
	mov rdx, rsp

    call IRQ_call

	pop rax
	mov gs, ax
	pop rax
	mov fs, ax
	pop rax
	mov es, ax
	pop rax
	mov ds, ax


	pop rbp
	pop r15
	pop r14
	pop r13
	pop r12
    pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
    iretq

isr_130:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 130

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_131:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 131

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_132:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 132

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_133:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 133

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_134:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 134

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_135:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 135

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_136:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 136

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_137:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 137

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_138:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 138

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_139:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 139

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_140:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 140

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_141:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 141

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_142:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 142

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_143:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 143

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_144:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 144

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_145:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 145

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_146:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 146

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_147:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 147

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_148:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 148

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_149:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 149

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_150:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 150

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_151:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 151

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_152:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 152

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_153:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 153

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_154:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 154

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_155:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 155

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_156:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 156

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_157:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 157

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_158:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 158

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_159:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 159

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_160:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 160

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_161:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 161

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_162:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 162

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_163:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 163

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_164:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 164

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_165:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 165

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_166:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 166

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_167:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 167

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_168:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 168

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_169:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 169

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_170:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 170

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_171:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 171

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_172:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 172

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_173:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 173

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_174:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 174

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_175:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 175

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_176:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 176

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_177:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 177

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_178:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 178

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_179:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 179

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_180:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 180

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_181:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 181

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_182:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 182

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_183:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 183

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_184:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 184

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_185:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 185

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_186:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 186

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_187:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 187

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_188:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 188

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_189:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 189

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_190:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 190

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_191:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 191

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_192:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 192

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_193:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 193

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_194:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 194

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_195:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 195

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_196:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 196

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_197:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 197

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_198:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 198

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_199:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 199

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_200:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 200

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_201:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 201

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_202:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 202

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_203:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 203

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_204:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 204

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_205:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 205

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_206:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 206

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_207:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 207

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_208:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 208

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_209:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 209

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_210:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 210

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_211:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 211

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_212:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 212

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_213:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 213

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_214:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 214

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_215:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 215

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_216:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 216

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_217:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 217

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_218:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 218

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_219:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 219

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_220:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 220

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_221:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 221

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_222:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 222

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_223:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 223

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_224:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 224

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_225:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 225

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_226:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 226

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_227:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 227

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_228:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 228

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_229:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 229

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_230:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 230

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_231:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 231

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_232:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 232

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_233:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 233

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_234:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 234

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_235:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 235

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_236:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 236

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_237:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 237

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_238:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 238

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_239:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 239

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_240:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 240

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_241:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 241

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_242:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 242

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_243:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 243

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_244:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 244

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_245:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 245

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_246:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 246

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_247:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 247

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_248:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 248

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_249:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 249

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_250:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 250

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_251:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 251

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_252:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 252

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_253:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 253

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_254:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 254

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

isr_255:
    push rsi
	push rax
	push rcx
	push rdx
	push rdi
	push r8
	push r9
	push r10
	push r11

    xor rdx, rdx
; C args (irq, err)
    mov rdi, 255

    call IRQ_call

    pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rdx
	pop rcx
	pop rax
	pop rsi
    iretq

global isr_stub_table
isr_stub_table:
dq isr_0
dq isr_1
dq isr_2
dq isr_3
dq isr_4
dq isr_5
dq isr_6
dq isr_7
dq isr_8
dq isr_9
dq isr_10
dq isr_11
dq isr_12
dq isr_13
dq isr_14
dq isr_15
dq isr_16
dq isr_17
dq isr_18
dq isr_19
dq isr_20
dq isr_21
dq isr_22
dq isr_23
dq isr_24
dq isr_25
dq isr_26
dq isr_27
dq isr_28
dq isr_29
dq isr_30
dq isr_31
dq isr_32
dq isr_33
dq isr_34
dq isr_35
dq isr_36
dq isr_37
dq isr_38
dq isr_39
dq isr_40
dq isr_41
dq isr_42
dq isr_43
dq isr_44
dq isr_45
dq isr_46
dq isr_47
dq isr_48
dq isr_49
dq isr_50
dq isr_51
dq isr_52
dq isr_53
dq isr_54
dq isr_55
dq isr_56
dq isr_57
dq isr_58
dq isr_59
dq isr_60
dq isr_61
dq isr_62
dq isr_63
dq isr_64
dq isr_65
dq isr_66
dq isr_67
dq isr_68
dq isr_69
dq isr_70
dq isr_71
dq isr_72
dq isr_73
dq isr_74
dq isr_75
dq isr_76
dq isr_77
dq isr_78
dq isr_79
dq isr_80
dq isr_81
dq isr_82
dq isr_83
dq isr_84
dq isr_85
dq isr_86
dq isr_87
dq isr_88
dq isr_89
dq isr_90
dq isr_91
dq isr_92
dq isr_93
dq isr_94
dq isr_95
dq isr_96
dq isr_97
dq isr_98
dq isr_99
dq isr_100
dq isr_101
dq isr_102
dq isr_103
dq isr_104
dq isr_105
dq isr_106
dq isr_107
dq isr_108
dq isr_109
dq isr_110
dq isr_111
dq isr_112
dq isr_113
dq isr_114
dq isr_115
dq isr_116
dq isr_117
dq isr_118
dq isr_119
dq isr_120
dq isr_121
dq isr_122
dq isr_123
dq isr_124
dq isr_125
dq isr_126
dq isr_127
dq isr_128
dq isr_129
dq isr_130
dq isr_131
dq isr_132
dq isr_133
dq isr_134
dq isr_135
dq isr_136
dq isr_137
dq isr_138
dq isr_139
dq isr_140
dq isr_141
dq isr_142
dq isr_143
dq isr_144
dq isr_145
dq isr_146
dq isr_147
dq isr_148
dq isr_149
dq isr_150
dq isr_151
dq isr_152
dq isr_153
dq isr_154
dq isr_155
dq isr_156
dq isr_157
dq isr_158
dq isr_159
dq isr_160
dq isr_161
dq isr_162
dq isr_163
dq isr_164
dq isr_165
dq isr_166
dq isr_167
dq isr_168
dq isr_169
dq isr_170
dq isr_171
dq isr_172
dq isr_173
dq isr_174
dq isr_175
dq isr_176
dq isr_177
dq isr_178
dq isr_179
dq isr_180
dq isr_181
dq isr_182
dq isr_183
dq isr_184
dq isr_185
dq isr_186
dq isr_187
dq isr_188
dq isr_189
dq isr_190
dq isr_191
dq isr_192
dq isr_193
dq isr_194
dq isr_195
dq isr_196
dq isr_197
dq isr_198
dq isr_199
dq isr_200
dq isr_201
dq isr_202
dq isr_203
dq isr_204
dq isr_205
dq isr_206
dq isr_207
dq isr_208
dq isr_209
dq isr_210
dq isr_211
dq isr_212
dq isr_213
dq isr_214
dq isr_215
dq isr_216
dq isr_217
dq isr_218
dq isr_219
dq isr_220
dq isr_221
dq isr_222
dq isr_223
dq isr_224
dq isr_225
dq isr_226
dq isr_227
dq isr_228
dq isr_229
dq isr_230
dq isr_231
dq isr_232
dq isr_233
dq isr_234
dq isr_235
dq isr_236
dq isr_237
dq isr_238
dq isr_239
dq isr_240
dq isr_241
dq isr_242
dq isr_243
dq isr_244
dq isr_245
dq isr_246
dq isr_247
dq isr_248
dq isr_249
dq isr_250
dq isr_251
dq isr_252
dq isr_253
dq isr_254
dq isr_255
