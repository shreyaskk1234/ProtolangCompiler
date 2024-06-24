	format PE64
	entry start
	section '.text' code readable executable
natsum:	push rbp
	mov rbp,rsp
	sub rsp,4
    	mov al,1
	movsx eax,al
	mov [rbp-4],eax
L1:	mov eax,[rbp-4]
	push rax
	mov rbx,[rbp+24]
	mov eax,[rbx]
	pop rcx
	cmp ecx,eax
	setle al
	test al,al
	je L2
	jmp L3
L4:	mov eax,[rbp-4]
	push rax
	mov al,1
	movsx eax,al
	pop rcx
	add eax,ecx
	mov [rbp-4],eax
	jmp L1
L3:	mov r10,[rbp+16]
	mov rbx,[rbp+16]
	mov eax,[rbx]
	push rax
	mov eax,[rbp-4]
	pop rcx
	add eax,ecx
	mov [r10],eax
	jmp L4
L2:	add rsp,4
	mov rsp,rbp
	pop rbp
	ret
start:	push rbp
	mov rbp,rsp
	sub rsp,8
    	mov al,25
	movsx eax,al
	mov [rbp-4],eax
	lea rax,[rbp-4]
	push rax
	lea rax,[rbp-8]
	push rax
	call natsum
	add rsp,8
	mov rsp,rbp
	pop rbp
	ret
