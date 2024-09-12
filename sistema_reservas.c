#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define KEY_RESERVAS 1234
#define KEY_TURMAS 5678
#define NUM_SALAS 10

void reservar_sala(int *reservas, int *turmas) {
    int sala, codigo_turma;

    printf("Digite o número da sala que deseja reservar (0-9): ");
    scanf("%d", &sala);

    if (sala < 0 || sala >= NUM_SALAS) {
        printf("Sala inválida.\n");
        return;
    }

    if (reservas[sala] == 1) {
        printf("Sala já está reservada.\n");
    } else {
        printf("Digite o código da turma: ");
        scanf("%d", &codigo_turma);
        reservas[sala] = 1;
        turmas[sala] = codigo_turma;
        printf("Sala %d reservada com sucesso.\n", sala);
    }
}

void liberar_sala(int *reservas, int *turmas) {
    int sala;

    printf("Digite o número da sala que deseja liberar (0-9): ");
    scanf("%d", &sala);

    if (sala < 0 || sala >= NUM_SALAS) {
        printf("Sala inválida.\n");
        return;
    }

    if (reservas[sala] == 0) {
        printf("Sala já está disponível.\n");
    } else {
        reservas[sala] = 0;
        turmas[sala] = 0;
        printf("Sala %d liberada com sucesso.\n", sala);
    }
}

void visualizar_status(int *reservas, int *turmas) {
    printf("Status das salas:\n");
    for (int i = 0; i < NUM_SALAS; i++) {
        printf("Sala %d: %s - Turma %d\n", i, reservas[i] == 0 ? "Disponível" : "Reservada", turmas[i]);
    }
}

int main() {
    int shm_reservas_id, shm_turmas_id;
    int *reservas, *turmas;
    int opcao;

    // Conecta à memória compartilhada de reservas
    shm_reservas_id = shmget(KEY_RESERVAS, NUM_SALAS * sizeof(int), 0666);
    if (shm_reservas_id < 0) {
        perror("Erro ao conectar à memória de reservas");
        exit(1);
    }

    // Conecta à memória compartilhada de turmas
    shm_turmas_id = shmget(KEY_TURMAS, NUM_SALAS * sizeof(int), 0666);
    if (shm_turmas_id < 0) {
        perror("Erro ao conectar à memória de turmas");
        exit(1);
    }

    // Anexar as áreas de memória
    reservas = (int *)shmat(shm_reservas_id, NULL, 0);
    if (reservas == (int *)-1) {
        perror("Erro ao anexar à memória de reservas");
        exit(1);
    }

    turmas = (int *)shmat(shm_turmas_id, NULL, 0);
    if (turmas == (int *)-1) {
        perror("Erro ao anexar à memória de turmas");
        exit(1);
    }

    // Menu de opções
    do {
        printf("\n1. Reservar sala\n");
        printf("2. Liberar sala\n");
        printf("3. Visualizar status das salas\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                reservar_sala(reservas, turmas);
                break;
            case 2:
                liberar_sala(reservas, turmas);
                break;
            case 3:
                visualizar_status(reservas, turmas);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    // Desanexar da memória
    shmdt(reservas);
    shmdt(turmas);

    return 0;
}
