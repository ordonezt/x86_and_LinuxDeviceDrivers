# Planilla de avance de Trabajos Prácticos del Alumno

**->Usuario SIGA:** toordonez  
**->Legajo:** 159.565-9  

## TP 1
- [X] TP01.01

- [X] TP01.02  
 **Comentarios:**
	- Me quedo pendiente hacer el retorno del memcpy, aunque no tiene mucho sentido en el ejemplo
	
- [X] TP01.03  
 **Comentarios:**
	- Tengo el siguiente warning que no pude arreglar
		```
		nasm -Wall -felf32 -lmibios.lst src/mibios.asm -o mibios.elf
		src/mibios.asm:57: warning: 8- or 16-bit relocations in ELF is a GNU extension [-w+gnu-elf-extensions]
		```
- [X] TP01.04  
 **Comentarios:**
	- La version mas completa es la v2

- [X] TP01.05  
 **Comentarios:**
	- Casi listo, solo falta el teclado que lo voy a hacer mas adelante

- [X] TP01.06  

- [X] TP01.07  
 **Comentarios:**
	- Falta arreglar bug de posiciones de pantalla

- [X] TP01.08  

- [X] TP01.09  

- [X] TP01.10  
 **Comentarios:**
	- Se hizo identity mapping entre 0x00010000 (<code>__TABLAS_PAGINACION_VMA_FISICA</code>) y 0x00050000 (<code>__RUTINAS_VMA_FISICA</code>) para evitar iteracion infinita en la paginacion dinamica.

- [X] TP01.11  

- [X] TP01.12  

- [X] TP01.13  
<!-- ## TP Integrador
- [ ] Servidor  
 **Comentarios:**
	- Esto es un comentario
	- Esto es otro <code>comentario</code>
- [ ] Cliente  
 **Comentarios:**
	- Esto es un comentario
	- Esto es otro <code>comentario</code>
- [ ] Driver  
 **Comentarios:**
	- Esto es un comentario
	- Esto es otro <code>comentario</code> -->
