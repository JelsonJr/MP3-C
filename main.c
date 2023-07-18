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

/*void play_sound(const char* file) {
    ALLEGRO_MIXER* mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    if (!mixer) {
        fprintf(stderr, "Falha ao criar o mixer.\n");
        return -1;
    }

    ALLEGRO_SAMPLE* audioSample = al_load_sample(file);
    if (!audioSample) {
        fprintf(stderr, "Falha ao carregar o arquivo de áudio.\n");
        return -1;
    }

    printf("dentro da funcao de tocar musica\n");
    al_play_sample(audioSample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    
    // al_rest(25.0); Aguarda 5 segundos para ouvir o áudio
    al_destroy_sample(audioSample);
    al_destroy_mixer(mixer);
    al_uninstall_audio();
    printf("dentro da funcao de tocar musica - FIM\n");
}*/

void playWav(const char* filepath) {
    // Inicializa o subsistema de áudio
    if (!al_install_audio()) {
        printf("Erro ao inicializar o subsistema de áudio!\n");
        return;
    }

    if (!al_init_acodec_addon()) {
        printf("Erro ao inicializar o addon de áudio do Allegro!\n");
        return;
    }

    // Reserva 1 canal de áudio para reprodução
    if (!al_reserve_samples(1)) {
        printf("Erro ao reservar canais de áudio!\n");
        return;
    }

    // Carrega o arquivo .wav
    ALLEGRO_SAMPLE* audioSample = al_load_sample(filepath);
    if (!audioSample) {
        printf("Erro ao carregar o arquivo .wav: %s\n", filepath);
        return;
    }

    // Cria a instância para reproduzir o áudio
    ALLEGRO_SAMPLE_INSTANCE* instance = al_create_sample_instance(audioSample);
    if (!instance) {
        printf("Erro ao criar a instância de áudio!\n");
        al_destroy_sample(audioSample);
        return;
    }

    // Anexa a instância ao mixer padrão
    if (!al_attach_sample_instance_to_mixer(instance, al_get_default_mixer())) {
        printf("Erro ao anexar a instância ao mixer!\n");
        al_destroy_sample_instance(instance);
        al_destroy_sample(audioSample);
        return;
    }

    // Reproduz o áudio
    al_play_sample_instance(instance);

    int teste = 0;
    al_get_sample_instance_playing(instance);
    // Aguarda a reprodução terminar
    while (al_get_sample_instance_playing(instance)) {
        al_rest(1);
        teste++;
        printf("%ds ", teste);
    }    
    // Limpeza e destruição dos recursos
    al_destroy_sample_instance(instance);
    al_destroy_sample(audioSample);

    // Finaliza o subsistema de áudio
    al_uninstall_audio();
}

void encontrar_caminho_relativo(const char* caminho_atual, const char* caminho_absoluto) {
    const char* delimiter = "\\";
    char* context_atual;
    char* context_absoluto;
    char caminho_relativo[512] = "";

    char* token_atual = strtok_s((char*)caminho_atual, delimiter, &context_atual);
    char* token_absoluto = strtok_s((char*)caminho_absoluto, delimiter, &context_absoluto);

    while (token_atual && token_absoluto && strcmp(token_atual, token_absoluto) == 0) {
        token_atual = strtok_s(NULL, delimiter, &context_atual);
        token_absoluto = strtok_s(NULL, delimiter, &context_absoluto);
    }

    while (token_atual) {
        strcat_s(caminho_relativo, sizeof(caminho_relativo), "..\\");
        token_atual = strtok_s(NULL, delimiter, &context_atual);
    }

    if (token_absoluto) {
        strcat_s(caminho_relativo, sizeof(caminho_relativo), token_absoluto);
        token_absoluto = strtok_s(NULL, delimiter, &context_absoluto);

        while (token_absoluto) {
            strcat_s(caminho_relativo, sizeof(caminho_relativo), "\\");
            strcat_s(caminho_relativo, sizeof(caminho_relativo), token_absoluto);
            token_absoluto = strtok_s(NULL, delimiter, &context_absoluto);
        }
    }

    printf("Caminho relativo: %s\n", caminho_relativo);
    playWav(caminho_relativo);

}

void listar_arquivos_diretorio(const char* diretorio) {
    DIR* dir;
    struct dirent* entrada;

    dir = opendir(diretorio);

    if (dir == NULL) {
        printf("Erro ao abrir o diretório.\n");
        return;
    }

    char caminho_atual[256];
    char caminho_absoluto[512];

    if (getcwd(caminho_atual, sizeof(caminho_atual)) == NULL) {
        return;
    }

    while ((entrada = readdir(dir)) != NULL) {
        if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
            continue;

        char* extensao = strrchr(entrada->d_name, '.');
        if ((extensao != NULL && strcmp(extensao, ".wav") == 0)) {
            snprintf(caminho_absoluto, sizeof(caminho_absoluto), "%s\\%s", diretorio, entrada->d_name);

            printf("Caminho absoluto: %s\n", caminho_absoluto);
            printf("Caminho do diretorio atual: %s\n", caminho_atual);

            encontrar_caminho_relativo(caminho_atual, caminho_absoluto);
        }
    }

    closedir(dir);
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
                al_show_native_message_box(displayInicial->screen, "Caixa de Di\xc3\xa1logo", "Hello World", "Este \xc3\xa9 um exemplo de caixa de di\xc3\xa1logo!", NULL, ALLEGRO_MESSAGEBOX_QUESTION);
                continue;
            }

            if (is_mouse_over_button(displayInicial, mouseX, mouseY, BUTTON_RADIUS)) {
                listar_arquivos_diretorio("C:\\Users\\Jelson\\Downloads");
                
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
