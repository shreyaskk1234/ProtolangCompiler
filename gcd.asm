	format PE64
	entry start
	section '.data' data readable writeable
	a	dq	?
	b	dq	?
	c	dq	?
	section '.text' code readable executable
start:	push rbp
	mov rbp,rsp
	sub rsp,0
    	mov ax,9999
	movsx rax,ax
	mov [a],rax
	mov ax,3333
	movsx rax,ax
	mov [b],rax
L1:	mov rax,[a]
	push rax
	mov rax,[b]
	pop rcx
	cmp rcx,rax
	setne al
	test al,al
	je L2
	mov rax,[a]
	push rax
	mov rax,[b]
	pop rcx
	cmp rcx,rax
	setg al
	test al,al
	je L4
	mov rax,[a]
	push rax
	mov rax,[b]
	pop rcx
	xchg rax,rcx
	sub rax,rcx
	mov [a],rax
	jmp L3
L4:	mov rax,[b]
	push rax
	mov rax,[a]
	pop rcx
	xchg rax,rcx
	sub rax,rcx
	mov [b],rax
L3:	jmp L1
L2:	mov rax,[a]
	mov [c],rax
	add rsp,0
	mov rsp,rbp
	pop rbp
	ret
