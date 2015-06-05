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
extern sched_tarea_actualsched_tarea_actual

;; Screen
extern screen_actualizar_reloj_global
extern print


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
	interrupcion_%1_msg db     'INTERRRUPCION ', %1
	interrupcion_%1_len equ    $ - interrupcion_%1_msg
	imprimir_texto_mp interrupcion_%1_msg, interrupcion_%1_len, 0x07, %1, 0
    mov eax, %1
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

;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

global _isr32
_isr32:
	; GUARDO REGISTROS ANTES DE LLAMAR A LA FUNCION DE C
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
	pushad

	call fin_intr_pic1

	mov ebx, esp
	sub ebx, 4
	xor eax, eax
	in al, 0x60
	push eax
	
	mov ecx, 0x000F000F
	push ecx
	mov ecx, 10
	push ecx
	mov ecx, 10
	push ecx
	push ebx
	
	call print

	add esp, 20
	popad	
	iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;


