#include "position.h"

Position* create_position(int x, int y) {
    Position* pos = malloc(sizeof(Position));

    if (pos == NULL) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de position",
            "Falha ao criar/inicializar struct position. Nao foi possivel realizar alocacao", NULL, ALLEGRO_MESSAGEBOX_ERROR);

        return NULL;
    }

    pos->x = x;
    pos->y = y;

    return pos;
}

void destroy_position(Position* position) {
    if (position) {
        free(position);
    }
}