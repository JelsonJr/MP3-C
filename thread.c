#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h> 
#include <Windows.h>

bool is_directory(const char* path) {
    struct stat path_stat;
    stat(path, &path_stat);

    return S_ISDIR(path_stat.st_mode);
}

void list_publics_directorys(const char* directory, int* done) {
    DIR* dir;
    struct dirent* input;

    dir = opendir(directory);

    if (dir == NULL) {
        *done = 1;
        return;
    }

    while ((input = readdir(dir)) != NULL) {
        if (strcmp(input->d_name, ".") == 0 || strcmp(input->d_name, "..") == 0) {
            continue;
        }

        char caminho_absoluto[512];
        snprintf(caminho_absoluto, sizeof(caminho_absoluto), "%s/%s", directory, input->d_name);

        if (is_directory(caminho_absoluto)) {
            list_publics_directorys(caminho_absoluto, done);
            continue;
        }

        const char* ext = strrchr(input->d_name, '.');

        if (ext != NULL && strcmp(ext, ".wav") == 0) {
            printf("%s\n", caminho_absoluto);
        }

    }

    closedir(dir);
}

DWORD WINAPI ListDirectoriesThread(LPVOID lpParam) {
    const char* diretorio_raiz = (const char*)lpParam;
    int done = 0;
    list_publics_directorys(diretorio_raiz, &done);
    return 0;
}

int main_executable() {
    char userProfile[MAX_PATH];
    if (!GetEnvironmentVariableA("USERPROFILE", userProfile, MAX_PATH)) {
        printf("Erro ao obter o diretório do usuário.\n");
        return 1;
    }

    printf("%s\n", userProfile);

    HANDLE hThread = CreateThread(NULL, 0, ListDirectoriesThread, (LPVOID)userProfile, 0, NULL);
    if (hThread == NULL) {
        printf("Falha ao criar a thread.\n");
        return 1;
    }

    int count = 0;
    while (1) {
        count++;
        printf("Incremented variable: %d\n", count);

        Sleep(1000);

        if (WaitForSingleObject(hThread, 0) == WAIT_OBJECT_0) {
            break;
        }
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}