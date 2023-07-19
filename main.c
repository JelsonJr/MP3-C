#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <dirent.h>
#include "display.h"
#include "position.h"
#include "draw.h"
#include "fonts.h"
#include "mouse_location.h"
#ifdef _WIN32
#include <io.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#define BUTTON_RADIUS 70
#define SYMBOL_SIZE 30
#define ANIMATION_DURATION 0.8

int init_allegro() {
    if (!al_init()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o do Allegro", "Falha ao inicializar Allegro.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_install_keyboard()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o do Teclado", "Falha ao inicializar teclado.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_install_mouse()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o do Mouse", "Falha ao inicializar mouse.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_init_primitives_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Primitivas", "Falha ao inicializar add-on de primitivas.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_init_font_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Fontes", "Falha ao inicializar add-on de fontes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_init_ttf_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Fontes", "Falha ao inicializar ttf para importa\xc3\xa7\xc3\xa3o de fontes.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    if (!al_init_image_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Imagens", "Falha ao inicializar add-on de imagens.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    } 
    
    if (!al_install_audio()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Imagens", "Falha ao inicializar add-on de imagens.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    } 
    
    if (!al_init_acodec_addon()) {
        al_show_native_message_box(NULL, "Erro", "Inicializa\xc3\xa7\xc3\xa3o de Imagens", "Falha ao inicializar add-on de imagens.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }

    return 1;
}

void init_monitor(Display* display) {
    ALLEGRO_MONITOR_INFO monitorInfo;
    al_get_monitor_info(0, &monitorInfo);

    const int windowPosX = monitorInfo.x1 + (monitorInfo.x2 - monitorInfo.x1 - display->SCREEN_WIDTH) / 2;
    const int windowPosY = monitorInfo.y1 + (monitorInfo.y2 - monitorInfo.y1 - display->SCREEN_HEIGHT) / 2;

    al_set_window_position(display->screen, windowPosX, windowPosY);
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

int find_screen_center(Display* display, ALLEGRO_FONT* font, const char* text) {
    int center = display->SCREEN_WIDTH / 2 - al_get_text_width(font, text) / 2;

    return center;
}

void draw_initial_screen(Display* display) {
    al_set_window_title(display->screen, "MPC");
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
    
    init_monitor(display);
    draw_gradient(display);

    int posX;
    const char* textTitle = "Selecione a pr\xc3\xb3xima m\xc3\xbasica e curta o som!";
    const char* textApp = "MPC";

    ALLEGRO_FONT* fontTitle = al_load_font(MONTSERRAT_BOLD, 28, 0);
    posX = find_screen_center(display, fontTitle, textTitle);
    Position* posTitle = create_position(posX, 20);

    ALLEGRO_FONT* fontApp = al_load_font(MONTSERRAT_BOLD, 36, 0);
    posX = find_screen_center(display, fontApp, textApp);
    Position* posApp = create_position(posX, 350);
    
    ALLEGRO_FONT* fontCredits = al_load_font(MONTSERRAT_SEMIBOLD, 12, 0);
    Position* posCredits = create_position(10, 460);

    draw_text(fontTitle, posTitle, al_map_rgb(250, 250, 250), textTitle, "%s", ALLEGRO_ALIGN_LEFT);
    draw_text(fontApp, posApp, al_map_rgb(250, 250, 250), textApp, "%s", ALLEGRO_ALIGN_LEFT);
    draw_text(fontCredits, posCredits, al_map_rgb(250, 250, 250), "Criado por: Jelson Rodrigues Junior", "%s", ALLEGRO_ALIGN_LEFT);
}

void play_sound(const char* filepath) {
    if (!al_install_audio()) {
        printf("Erro ao inicializar o subsistema de áudio!\n");
        return;
    }

    if (!al_init_acodec_addon()) {
        printf("Erro ao inicializar o addon de áudio do Allegro!\n");
        return;
    }

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
    al_uninstall_audio();
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

int main() {
    if (!init_allegro()) {
        return -1;
    }

    Display* displayInicial = create_display(720, 480);
    if (!displayInicial) {
        return -1;
    }

    draw_initial_screen(displayInicial);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

    al_register_event_source(event_queue, al_get_display_event_source(displayInicial->screen));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    ALLEGRO_COLOR buttonColor = al_map_rgb(255, 255, 255);
    ALLEGRO_COLOR symbolColor = al_map_rgb(138, 43, 226);

    al_draw_filled_circle(displayInicial->SCREEN_WIDTH / 2, displayInicial->SCREEN_HEIGHT / 2, BUTTON_RADIUS, buttonColor);
    al_draw_filled_triangle(displayInicial->SCREEN_WIDTH / 2 - SYMBOL_SIZE + 8, displayInicial->SCREEN_HEIGHT / 2 - SYMBOL_SIZE,
        displayInicial->SCREEN_WIDTH / 2 + SYMBOL_SIZE + 8, displayInicial->SCREEN_HEIGHT / 2,
        displayInicial->SCREEN_WIDTH / 2 - SYMBOL_SIZE + 8, displayInicial->SCREEN_HEIGHT / 2 + SYMBOL_SIZE, symbolColor);

    al_flip_display();

    float symbolSizeScale = 1.0;
    float targetScale = 1.0;
    float scaleStep = 0.0;

    al_start_timer(timer);

    Position* posCredits = create_position(10, 460);
    ALLEGRO_FONT* fontCredits = al_load_font(MONTSERRAT_SEMIBOLD, 12, 0);

    int mouseX = 0;
    int mouseY = 0;
    int mouseCursorDefault = 1;

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
                const char* corpo_texto = "GitHub: https://github.com/JelsonJr\n"
                    "LinkedIn: https://www.linkedin.com/in/jelson-rodrigues-53333a229/";

                al_show_native_message_box(displayInicial->screen, "Informacoes do desenvolvedor", "Onde voce pode me encontrar:", corpo_texto, NULL, ALLEGRO_MESSAGEBOX_QUESTION);
               
                continue;
            }

            if (mouseY >= posCredits->y + al_get_font_line_height(fontCredits) &&
                mouseY < posCredits->y + al_get_font_line_height(fontCredits) * 2) {
                system("start https://github.com/JelsonJr");
            }
         
            else if (mouseY >= posCredits->y + al_get_font_line_height(fontCredits) * 2 &&
                mouseY < posCredits->y + al_get_font_line_height(fontCredits) * 3) {
                system("start https://www.linkedin.com/in/jelson-rodrigues-53333a229/");
            }

            if (is_mouse_over_button(displayInicial, mouseX, mouseY, BUTTON_RADIUS)) {
                int num_files = 0;

                char** musics = list_files_directory("C:\\Users\\jelso\\Downloads", &num_files);
                
                for (int i = 0; i < num_files; i++) {
                    char* music = musics[i];

                    play_sound(music);
                }

                break;
            }
        }

        if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (is_mouse_over_credits(mouseX, mouseY, posCredits, fontCredits)) {
                al_set_system_mouse_cursor(displayInicial->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                mouseCursorDefault = 0;

                continue;
            }

            if (is_mouse_over_button(displayInicial, mouseX, mouseY, BUTTON_RADIUS)) {
                if (!al_is_event_queue_empty(event_queue)) {
                    continue;
                }

                al_set_system_mouse_cursor(displayInicial->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK);
                mouseCursorDefault = 0;

                targetScale = 1.15;
                scaleStep = (targetScale - symbolSizeScale) / (ANIMATION_DURATION * 60.0);

                while (symbolSizeScale < targetScale) {
                    animateButton(displayInicial, symbolColor, buttonColor, symbolSizeScale);
                    symbolSizeScale += scaleStep;
                    al_rest(1.0 / 120.0);
                }

                animateButton(displayInicial, symbolColor, buttonColor, targetScale);
                
                continue;
            } 
             
            if (!al_is_event_queue_empty(event_queue)) {
                continue;
            }

            if (!mouseCursorDefault) {
                al_set_system_mouse_cursor(displayInicial->screen, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
                mouseCursorDefault = 1;
            }

            targetScale = 1.0;
            scaleStep = (targetScale - symbolSizeScale) / (ANIMATION_DURATION * 60.0);

            while (symbolSizeScale > targetScale) {
                animateButton(displayInicial, buttonColor, symbolColor, symbolSizeScale);
                symbolSizeScale += scaleStep;
                al_rest(1.0 / 120.0);
            }

            animateButton(displayInicial, buttonColor, symbolColor, targetScale);
        }

        if (ev.type == ALLEGRO_EVENT_TIMER) {
            // Timer event
        }
        
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }
    }

    printf("fim do programa");
    destroy_display(displayInicial);
    destroy_position(posCredits);
    al_destroy_font(fontCredits);
    al_destroy_event_queue(event_queue);

    return 0;
}
