; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

;; ISR
extern unrecoverableHandler
extern print_state
extern clear

%macro ISR 1
global _isr%1

_isr%1:
	push ecx
	push eax
	push edx
	call clear
	pop edx
	pop eax
	pop ecx

	;;Checkear esto
	add esp, 12
	print_regs

    mov eax, %1
    push eax
    call unrecoverableHandler
    jmp $

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
sched_tarea_offset:     dd 0xCACABEEF
sched_tarea_selector:   dw 0x00

extern screen_actualizar_reloj_global
extern fin_intr_pic1
extern scheduler_tick
extern game_tick
global _isr32

_isr32:
	pushad
	call fin_intr_pic1

	call scheduler_tick
	; eax = indice en la gdt de tarea que quiero ir, o -1

	push eax

	cmp eax, -1
	je .end

	str bx
	; bx = offset en la gdt de tarea en la que estoy
	shl eax, 3
	; eax = offset en la gdt de la tarea en la que quiero ir
	cmp eax, ebx
	je .end

	mov [sched_tarea_selector], ax
	jmp far [sched_tarea_offset]
.end:
	call game_tick
	pop eax

	call screen_actualizar_reloj_global
	popad
iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

extern isr_keyboard
global _isr33

_isr33:
	pushad
	call fin_intr_pic1

	in al, 0x60

	push eax
	call isr_keyboard
	pop eax

	popad
iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

extern isr_syscall
extern idle
global _isr46

_isr46:
	pushad
	call fin_intr_pic1

	push eax
	push ecx
	call isr_syscall
	; Popeamos en otro lado para no perder el dato de retorno, que esta en eax
	pop esi
	pop edi

	; TODO: como hacemos para hacer un return cuando llaman a posicion?
	mov word [sched_tarea_selector], 0xE ; GDT_IDX_TASKI_DESC este es el valor posta posta.
	jmp far [sched_tarea_offset]

	popad
iret
