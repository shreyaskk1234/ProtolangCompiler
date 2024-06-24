	format PE64
	entry start
	section '.text' code readable executable
start:	push rbp
	mov rbp,rsp
	sub rsp,12
   	mov al,4
	movsx eax,al
	mov [rbp-4],eax
	mov al,5
	movsx eax,al
	mov [rbp-8],eax
	mov eax,[rbp-4]
	push rax
	mov eax,[rbp-8]
	pop rcx
	add eax,ecx
	mov [rbp-12],eax
	add rsp,12
	mov rsp,rbp
	pop rbp
	ret
