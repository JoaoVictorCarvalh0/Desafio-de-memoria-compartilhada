#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define KEY_RESERVAS 1234
#define KEY_TURMAS 5678

int main() {
    int shm_reservas_id, shm_turmas_id;

    // Localizar a área de memória para reservas
    shm_reservas_id = shmget(KEY_RESERVAS, 0, 0666);
    if (shm_reservas_id < 0) {
        perror("Erro ao localizar memória compartilhada de reservas");
        exit(1);
    }

    // Localizar a área de memória para turmas
    shm_turmas_id = shmget(KEY_TURMAS, 0, 0666);
    if (shm_turmas_id < 0) {
        perror("Erro ao localizar memória compartilhada de turmas");
        exit(1);
    }

    // Remover memória compartilhada de reservas
    shmctl(shm_reservas_id, IPC_RMID, NULL);

    // Remover memória compartilhada de turmas
    shmctl(shm_turmas_id, IPC_RMID, NULL);

    printf("Memória compartilhada removida com sucesso.\n");

    return 0;
}
