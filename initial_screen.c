#include "initial_screen.h"
#include "mouse_location.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <dirent.h>

void play_sound(const char* filepath) {
    if (!al_reserve_samples(1)) {
        printf("Erro ao reservar canais de áudio!\n");
        return;
    }

    ALLEGRO_SAMPLE* audioSample = al_load_sample(filepath);
    if (!audioSample) {
        printf("Erro ao carregar o arquivo .wav: %s\n", filepath);
        return;
    }

    ALLEGRO_SAMPLE_INSTANCE* instance = al_create_sample_instance(audioSample);
    if (!instance) {
        printf("Erro ao criar a instância de áudio!\n");
        al_destroy_sample(audioSample);
        return;
    }

    if (!al_attach_sample_instance_to_mixer(instance, al_get_default_mixer())) {
        printf("Erro ao anexar a instância ao mixer!\n");
        al_destroy_sample_instance(instance);
        al_destroy_sample(audioSample);
        return;
    }

    al_play_sample_instance(instance);

    int teste = 0;
    al_get_sample_instance_playing(instance);

    while (al_get_sample_instance_playing(instance)) {
        al_rest(1);
        teste++;
        printf("%ds ", teste);
    }

    al_destroy_sample_instance(instance);
    al_destroy_sample(audioSample);
}

char** list_files_directory(const char* diretorio, int* num_arquivos) {
    DIR* dir;
    struct dirent* entrada;

    dir = opendir(diretorio);

    if (dir == NULL) {
        printf("Erro ao abrir o diretório.\n");
        *num_arquivos = 0;
        return NULL;
    }

    char** lista_caminhos = NULL;
    int num_caminhos = 0;

    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;

        char* extensao = strrchr(entrada->d_name, '.');
        if ((extensao != NULL && strcmp(extensao, ".wav") == 0)) {
            char caminho_absoluto[512];
            snprintf(caminho_absoluto, sizeof(caminho_absoluto), "%s/%s", diretorio, entrada->d_name);

            char** nova_lista_caminhos = (char**)realloc(lista_caminhos, (num_caminhos + 1) * sizeof(char*));
            if (nova_lista_caminhos == NULL) {
                printf("Erro de alocação de memória.\n");
                closedir(dir);
                *num_arquivos = num_caminhos;
                return lista_caminhos;
            }

            lista_caminhos = nova_lista_caminhos;
            lista_caminhos[num_caminhos] = (char*)malloc(strlen(caminho_absoluto) + 1);

            if (lista_caminhos[num_caminhos] != NULL) {
                strcpy_s(lista_caminhos[num_caminhos], strlen(caminho_absoluto) + 1, caminho_absoluto);
                num_caminhos++;

                continue;
            }

            printf("Erro de alocação de memória.\n");
            closedir(dir);
            *num_arquivos = num_caminhos;

            return lista_caminhos;
        }
    }

    closedir(dir);
    *num_arquivos = num_caminhos;

    return lista_caminhos;
}

int draw_initial_screen(Display* display, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_TIMER* timer) {
    al_set_window_title(display->screen, "MPC");
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);

    init_monitor(display);
    draw_gradient(display);

    const char* textTitle = "Selecione a pr\xc3\xb3xima m\xc3\xbasica e curta o som!";
    const char* textApp = "MPC";

    ALLEGRO_FONT* fontTitle = al_load_font(MONTSERRAT_BOLD, 28, 0);
    Position* posTitle = create_position(find_screen_center(display, fontTitle, textTitle), 20);

    ALLEGRO_FONT* fontApp = al_load_font(MONTSERRAT_BOLD, 36, 0);
    Position* posApp = create_position(find_screen_center(display, fontApp, textApp), 350);

    ALLEGRO_FONT* fontCredits = al_load_font(MONTSERRAT_SEMIBOLD, 12, 0);
    Position* posCredits = create_position(10, 460);

    draw_text(fontTitle, posTitle, al_map_rgb(250, 250, 250), textTitle, "%s", ALLEGRO_ALIGN_LEFT);
    draw_text(fontApp, posApp, al_map_rgb(250, 250, 250), textApp, "%s", ALLEGRO_ALIGN_LEFT);
    draw_text(fontCredits, posCredits, al_map_rgb(250, 250, 250), "Saiba mais sobre o projeto", "%s", ALLEGRO_ALIGN_LEFT);

    fontCredits = al_load_font(MONTSERRAT_SEMIBOLD, 12, 0);
    posCredits = create_position(10, 460);

    ALLEGRO_COLOR buttonColor = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR symbolColor = al_map_rgb(138, 43, 226);

    float symbolSizeScale = 1.0;
    float targetScale = 1.0;
    float scaleStep = 0.0;

    al_draw_filled_circle((float)display->SCREEN_WIDTH / 2, (float)display->SCREEN_HEIGHT / 2, BUTTON_RADIUS, buttonColor);
    al_draw_filled_triangle((float)display->SCREEN_WIDTH / 2 - SYMBOL_SIZE + 8, (float)display->SCREEN_HEIGHT / 2 - SYMBOL_SIZE,
        (float)display->SCREEN_WIDTH / 2 + SYMBOL_SIZE + 8, (float)display->SCREEN_HEIGHT / 2,
        (float)display->SCREEN_WIDTH / 2 - SYMBOL_SIZE + 8, (float)display->SCREEN_HEIGHT / 2 + SYMBOL_SIZE, symbolColor);

    al_flip_display();
    al_start_timer(timer);

    int mouseX = 0;
    int mouseY = 0;
    int mouseCursorDefault = 1;
    int option = 0;

    while (1) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        mouseX = ev.mouse.x;
        mouseY = ev.mouse.y;

        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            if (!al_is_event_queue_empty(event_queue)) {
                continue;
            }

            if (is_mouse_over_credits(mouseX, mouseY, posCredits, fontCredits)) {
                const char* command = "start https://github.com/JelsonJr/MP3-C";

                system(command);
                continue;
            }

            if (is_mouse_over_button(display, mouseX, mouseY, BUTTON_RADIUS)) {
                int num_files = 0;

                char** musics = list_files_directory("C:\\Users\\Jelson\\Downloads", &num_files);

                for (int i = 0; i < num_files; i++) {
                    char* music = musics[i];

                    play_sound(music);
                }

                option = 1;
                break;
            }
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (is_mouse_over_credits(mouseX, mouseY, posCredits, fontCredits)) {
                al_set_system_mouse_cursor(display->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                mouseCursorDefault = 0;

                continue;
            }

            if (is_mouse_over_button(display, mouseX, mouseY, BUTTON_RADIUS)) {
                if (!al_is_event_queue_empty(event_queue)) {
                    continue;
                }

                al_set_system_mouse_cursor(display->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                mouseCursorDefault = 0;

                targetScale = 1.15;
                scaleStep = (targetScale - symbolSizeScale) / (ANIMATION_DURATION * 60.0);

                while (symbolSizeScale < targetScale) {
                    animateButton(display, symbolColor, buttonColor, symbolSizeScale);
                    symbolSizeScale += scaleStep;
                    al_rest(1.0 / 120.0);
                }

                animateButton(display, symbolColor, buttonColor, targetScale);

                continue;
            }

            if (!al_is_event_queue_empty(event_queue)) {
                continue;
            }

            if (!mouseCursorDefault) {
                al_set_system_mouse_cursor(display->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                mouseCursorDefault = 1;
            }

            targetScale = 1.0;
            scaleStep = (targetScale - symbolSizeScale) / (ANIMATION_DURATION * 60.0);

            while (symbolSizeScale > targetScale) {
                animateButton(display, buttonColor, symbolColor, symbolSizeScale);
                symbolSizeScale += scaleStep;
                al_rest(1.0 / 120.0);
            }

            animateButton(display, buttonColor, symbolColor, targetScale);
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Timer event
        }

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }

    destroy_position(posCredits);
    al_destroy_font(fontCredits);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    return option;
}

void animateButton(Display* display, ALLEGRO_COLOR buttonColor, ALLEGRO_COLOR symbolColor, float symbolSizeScale) {
    int buttonX = display->SCREEN_WIDTH / 2;
    int buttonY = display->SCREEN_HEIGHT / 2;
    int buttonRadius = BUTTON_RADIUS;
    int symbolSize = SYMBOL_SIZE * symbolSizeScale;

    al_draw_filled_circle(buttonX, buttonY, buttonRadius, buttonColor);
    al_draw_filled_triangle(buttonX - symbolSize + 8, buttonY - symbolSize, buttonX + symbolSize + 8, buttonY, buttonX - symbolSize + 8, buttonY + symbolSize, symbolColor);

    al_flip_display();
}
