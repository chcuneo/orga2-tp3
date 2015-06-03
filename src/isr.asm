; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_tick
extern sched_tarea_actual

extern unrecoverableHandler
extern print_state
extern clear

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

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
extern screen_actualizar_reloj_global
extern fin_intr_pic1
global _isr32

_isr32:
	pushad
	call fin_intr_pic1
	call screen_actualizar_reloj_global
	popad
iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33

_isr33:
	inc ebx
	pushad
	in al, 0x60
	call fin_intr_pic1
	print_regs
	popad
iret


;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;


