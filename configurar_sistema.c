#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define KEY_RESERVAS 1234
#define KEY_TURMAS 5678
#define NUM_SALAS 10

int main() {
    int shm_reservas_id, shm_turmas_id;
    int *reservas, *turmas;

    // Criação da área de memória compartilhada para o status das reservas
    shm_reservas_id = shmget(KEY_RESERVAS, NUM_SALAS * sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
    if (shm_reservas_id < 0) {
        perror("Erro ao criar memória compartilhada para reservas");
        exit(1);
    }

    // Criação da área de memória compartilhada para os códigos das turmas
    shm_turmas_id = shmget(KEY_TURMAS, NUM_SALAS * sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
    if (shm_turmas_id < 0) {
        perror("Erro ao criar memória compartilhada para turmas");
        exit(1);
    }

    // Conecta-se à área de memória para reservas
    reservas = (int *)shmat(shm_reservas_id, NULL, 0);
    if (reservas == (int *)-1) {
        perror("Erro ao conectar à memória de reservas");
        exit(1);
    }

    // Conecta-se à área de memória para turmas
    turmas = (int *)shmat(shm_turmas_id, NULL, 0);
    if (turmas == (int *)-1) {
        perror("Erro ao conectar à memória de turmas");
        exit(1);
    }

    // Inicializa as áreas de memória
    for (int i = 0; i < NUM_SALAS; i++) {
        reservas[i] = 0;  // Sala disponível
        turmas[i] = 0;    // Nenhuma turma
    }

    printf("Memória compartilhada configurada com sucesso.\n");

    // Desanexar da memória
    shmdt(reservas);
    shmdt(turmas);

    return 0;
}
