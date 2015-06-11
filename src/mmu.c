/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
/* Atributos paginas */
/* -------------------------------------------------------------------------- */


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

// Inicializo em manager de memoria en la posicion MEM_MANAGER, salto a la siguiente pagina
void mmu_inicializar() {
	int* mem_manager = (int*)MEM_MANAGER;
	(*mem_manager) = (MEM_MANAGER + PAGE_SIZE);
}

// Pedir una pagina al kernel, se mueve a la siguiente pagina libre, y devuelve la pagina 
int* mmu_get_pagina() {
	int* mem_manager = (int*)MEM_MANAGER;
	int* pagina = (int*)(*mem_manager);
	(*mem_manager) = (*mem_manager) + PAGE_SIZE;
	return pagina;
}

// Inicializar el descriptor de tablas y tablas del kernel para identity mapping de 0x0 a 0x3FFFFF
void mmu_inicializar_dir_kernel() {
	int i;
	int* dir = (int*)DIR_PAGINAS_KERNEL;
	int* table;
	
	(*dir) = ((0x28) << 12) | 0x003;
	table = (int*)(DIR_PAGINAS_KERNEL + PAGE_SIZE);

	for(i = 0; i < 1024; i++) {
		(*table) = (i << 12) | 0x003;
		table += 1;
	}
}

// Copia la pagina original a copia
void mmu_copiar_pagina(int* original, int* copia) {
	int i;
	int temp;
	
	for(i = 0; i < 128; i++) {
		temp = (*original);
		(*copia) = temp;

		original = original + 4;
		copia = copia + 4;
	}
}

// Mapea una pagina virtual, cr3, fisica
void mmu_mapear_pagina(unsigned int virtual, unsigned int cr3, unsigned int fisica) {
	unsigned int directory_offset = virtual >> 22;
	unsigned int table_offset = (virtual << 10) >> 22;

	int* dir = (int*)(cr3 & 0xFFFFF000);
	dir += directory_offset;

	int* table;
	if((*dir & 0x1) == 1) {
		table = (int*)((*dir) & 0xFFFFF000);

		table += table_offset;
		(*table) = (fisica & 0xFFFFF000) | 0x003;
	} else {
		table = (int*)PAGE_COPY;
		(*dir) = ((int)table & 0xFFFFF000) | 0x003;

		table += table_offset;
		(*table) = (fisica & 0xFFFFF000) | 0x003;
	}
}

// Unpeaear una pagina virtual del cr3
void mmu_unmapear_pagina(unsigned int virtual, unsigned int cr3) {
	unsigned int directory_offset = virtual >> 22;
	unsigned int table_offset = (virtual << 10) >> 22;

	int* dir = (int*)(cr3 & 0xFFFFF000);
	dir += directory_offset;

	int* table = (int*)((*dir) + table_offset);
	(*table) = (*table) & 0xFFFFF000;
}

// Inicializar un directorio de tablas y tabla de una tarea
void mmu_inicializar_dir_pirata(unsigned int posMapa, int* codigo, unsigned int cr3) {
	int i;
	int* dir = mmu_get_pagina();
	int* table = mmu_get_pagina();

	(*dir) = ((0x28) << 12) | 0x003;
	table = (int*)(DIR_PAGINAS_KERNEL + PAGE_SIZE);

	for(i = 0; i < 1024; i++) {
		(*table) = (i << 12) | 0x003;
		table += 1;
	}
	
	unsigned int virtual = 0x0400000 + PAGE_SIZE;
	mmu_mapear_pagina(virtual, cr3, posMapa);
	mmu_copiar_pagina(codigo, (int*)virtual);
	mmu_unmapear_pagina(virtual, cr3);
}