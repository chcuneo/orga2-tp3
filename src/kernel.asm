; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

; bochs -q -rc bochsdbg

%include "imprimir.mac"

global start

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

    ; Habilitar A20
    call habilitar_A20

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    ; Saltar a modo protegido
    jmp 0x40:modoprotegido

modoprotegido:
BITS 32
    ; Establecer selectores de segmentos
    xor eax, eax
    mov ax, 0x48
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov gs, ax

    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    mov ax, 0x60   ; Selector del segmento de video
    mov fs, ax

    ; Establecer la base de la pila
    mov ebp, 0x7D000; DECISION DE DISEÑO: Lo ponemos asi para que este en el limite del data segment
    mov esp, 0x7D000; TODO: Verificar que estas constantes esten bien

    ; Imprimir mensaje de bienvenida
    call screen_refresh_logo

    ; Inicializar el juego 

    ; Inicializar pantalla
    call screen_inicializar

    ; Inicializar el manejador de memoria, cargar directorio de paginas, cargar cr3
    call mmu_inicializar_dir_kernel

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Mapeamos las paginas a memoria
    call mmu_inicializar

    ; Inicializar tss

    ; Inicializar tss de la tarea Idle

    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]
    ; mov bx, 0x00
    ; div bx

    ; Configurar controlador de interrupciones
    call resetear_pic
    call habilitar_pic
    ; Cargar tarea inicial

    ; Habilitar interrupciones
    sti
    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

extern mmu_inicializar_dir_kernel
extern screen_refresh_logo
extern GDT_DESC
extern idt_inicializar
extern IDT_DESC
extern screen_inicializar
extern print_state
extern mmu_inicializar
extern resetear_pic
extern habilitar_pic

%include "a20.asm"