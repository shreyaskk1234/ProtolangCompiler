	format PE64
	entry start
	section '.text' code readable executable
start:	push rbp
	mov rbp,rsp
	sub rsp,12
   	mov ax,1000
	movsx eax,ax
	mov [rbp-4],eax
	mov ax,1010
	movsx eax,ax
	mov [rbp-8],eax
	mov eax,[rbp-4]
	push rax
	mov eax,[rbp-8]
	pop rcx
	cmp ecx,eax
	setg al
	test al,al
	je L2
	mov eax,[rbp-4]
	push rax
	mov eax,[rbp-8]
	pop rcx
	xchg eax,ecx
	sub eax,ecx
	mov [rbp-12],eax
	jmp L1
L2:	mov eax,[rbp-8]
	push rax
	mov eax,[rbp-4]
	pop rcx
	xchg eax,ecx
	sub eax,ecx
	mov [rbp-12],eax
L1:	add rsp,12
	mov rsp,rbp
	pop rbp
	ret
