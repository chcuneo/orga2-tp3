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
extern loadRegisters

%macro ISR 1
global _isr%1

_isr%1:
	; Cargamos los registros para debug, si fuera necesario.
	pushf
	push esp
	add dword [esp], 8
	push ebp
	push dword [esp - 12]
	call loadRegisters
	pop eax
	pop eax
	pop eax
	popf

	; Llamamos al handler para esta excepcion
    mov eax, %1
    push eax
    call unrecoverableHandler
    pop eax

    ; Saltamos a la tarea idle, asi nos evitamos volver a faultear
	mov word [sched_tarea_selector], 0x70 ; GDT_IDX_TASKI_DESC este es el valor posta posta.
	jmp far [sched_tarea_offset]

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

	; Corremos el scheduler, para ver a donde saltar
	call scheduler_tick
	push eax 		; eax = indice en la gdt de tarea que quiero ir, o -1

	; Si el scheduler no tiene que operar, saltamos al final
	cmp eax, -1
	je .end

	str bx			; bx = task register actual
	and ebx, 0x00FF ; bx = offset en la gdt de tarea en la que estoy
	shl eax, 3 		; eax = offset en la gdt de la tarea en la que quiero ir
	; Si la tarea actual y la nueva son la misma, saltamos al final del algoritmo
	cmp eax, ebx
	je .end

	; Saltamos a la nueva tarea
	mov [sched_tarea_selector], ax
	jmp far [sched_tarea_offset]
.end:
	; Si somos una tarea del juego, actualizamos los relojes de tareas
	mov ebx, [esp]
	cmp ebx, 2
	jl .skip
	call game_tick
.skip:
	pop eax
	; Actualizamos el reloj global
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

	; Levantamos la entrada de teclado
	in al, 0x60

	; Llamamos al "driver" de teclado
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
global _isr70

_isr70:
	pushad
	push ecx ; param
	push eax ; operation
	call fin_intr_pic1

	; Llamamos al handler para la syscall
	call isr_syscall
	; Popeamos en otro lado para no perder el dato de retorno, que esta en eax
	pop esi
	pop edi

	; Ponemos el valor de eax en la pila como el valor de retorno del syscall,
	; asi el popad anda bien y pone el valor de retorno correcto
	mov [esp + 7*4], eax

	; Saltamos a la tarea idle
	mov word [sched_tarea_selector], 0x70 ; GDT_IDX_TASKI_DESC este es el valor posta posta.
	jmp far [sched_tarea_offset]
	
	popad
iret
