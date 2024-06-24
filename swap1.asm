	format PE64
	entry start
	section '.text' code readable executable
swap:	push rbp
	mov rbp,rsp
	sub rsp,4
    	mov rbx,[rbp+16]
	mov eax,[rbx]
	mov [rbp-4],eax
	mov r10,[rbp+16]
	mov rbx,[rbp+24]
	mov eax,[rbx]
	mov [r10],eax
	mov r10,[rbp+24]
	mov eax,[rbp-4]
	mov [r10],eax
	add rsp,4
	mov rsp,rbp
	pop rbp
	ret
start:	push rbp
	mov rbp,rsp
	sub rsp,8
    	mov al,25
	movsx eax,al
	mov [rbp-4],eax
	mov al,99
	movsx eax,al
	mov [rbp-8],eax
	lea rax,[rbp-4]
	push rax
	lea rax,[rbp-8]
	push rax
	call swap
	add rsp,8
	mov rsp,rbp
	pop rbp
	ret
