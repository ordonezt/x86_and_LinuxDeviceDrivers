#ifndef __SHARMEM_H
#define __SHARMEM_H

int crear_shmem(void **memoria, key_t llave, int longitud);
void destruir_shmem (int shm_id, void *mem);
void* vincular_shmem(int shm_id);

#endif /*__SHARMEM_H*/
