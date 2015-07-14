; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"

global start

extern screen_inicializar
extern screen_refrescar
extern screen_pintar_puntajes
extern idt_inicializar
extern print_group

extern inicializar_mmu
extern mmu_inicializar_dir_kernel

extern resetear_pic
extern habilitar_pic

extern tss_inicializar
extern tss_crear_idle

extern inicializar_sched
extern sched_pirata_manual

extern GDT_DESC
extern IDT_DESC

extern BASE_TSS

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
    CALL habilitar_A20

    ; Cargar la GDT
    LGDT [GDT_DESC]

    ; Setear el bit PE del registro CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; Saltar a modo protegido
    jmp 0x40:modoprotegido
    BITS 32
    modoprotegido:

    ; Establecer selectores de segmentos
    MOV AX, 0x48    
    MOV DS, AX
    MOV ES, AX
    MOV SS, AX

    ; Establecer la base de la pila
    MOV EBP, 0x27000
    MOV ESP, EBP

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar el juego


    ; Inicializar pantalla
    CALL screen_inicializar

    ; Inicializar el manejador de memoria
    CALL inicializar_mmu

    ; Inicializar el directorio de paginas
    CALL mmu_inicializar_dir_kernel

    ; Cargar directorio de paginas
    MOV EAX, 0x27000
    MOV CR3, EAX

    ; Habilitar paginacion
    MOV EAX, CR0
    OR  EAX, 0x80000000
    MOV CR0, EAX

    ; Imprimir el nombre del grupo
    CALL print_group
    ; Inicializar tss
    ; Inicializar tss de la tarea Idle
    CALL tss_inicializar
 
    ; Inicializar el scheduler
    call inicializar_sched
    
    ; Inicializar la IDT
    call idt_inicializar

    ; Cargar IDT
    lidt [IDT_DESC]

    ; Cargar tarea inicial
    XOR EAX, EAX
    MOV AX, 0x68
    LTR AX

    ;call sched_pirata_manual

    ; Configurar controlador de interrupciones
    call resetear_pic
    call habilitar_pic
    
    xchg bx, bx

    ; Habilitar interrupciones
    sti
    ;jmp 0x78:0

    ; Saltar a la primera tarea: Idle
    JMP 0x70:0

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"