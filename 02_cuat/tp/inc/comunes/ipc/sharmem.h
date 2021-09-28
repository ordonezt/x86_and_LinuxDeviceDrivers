#ifndef __SHARMEM_H
#define __SHARMEM_H

extern char* crear_shmem(key_t*,int*,char*,int,int, int);
extern void destruir_shmem(int,char*);

#endif /*__SHARMEM_H*/
