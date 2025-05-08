#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 10
#define MAX_LISTS 10
#define MAX_LENGTH 50

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_MAGENTA "\033[1;35m"

typedef struct {
    char descriere[MAX_LENGTH];
    int completata;
} Task;

typedef struct {
    char titlu[MAX_LENGTH];
    Task taskuri[MAX_TASKS];
    int nrTaskuri;
} ListaToDo;

static ListaToDo liste[MAX_LISTS];
static int nrListe = 0;

int citesteNumarIntre(char *prompt, int min, int max) {
    int val;
    printf("%s", prompt);
    if (scanf("%d", &val) != 1) {
        printf("Valoare introdusă %sinvalidă%s!", COLOR_RED, COLOR_RESET);
        while (getchar() != '\n');
        return -1;
    }
    getchar();
    if (val < min || val > max) {
        printf("Valoare %sinvalidă%s! Trebuie să fie între %d și %d.", COLOR_RED, COLOR_RESET, min, max);
        return -1;
    }
    return val;
}

void salveazaDate() {
    FILE *file = fopen("todo_data.txt", "w");
    if (!file) {
        printf("%sEroare%s la salvarea datelor!", COLOR_RED, COLOR_RESET);
        return;
    }
    fprintf(file, "%d\n", nrListe);
    for (int i = 0; i < nrListe; i++)
    {
        fprintf(file, "%s\n", liste[i].titlu);
        fprintf(file, "%d\n", liste[i].nrTaskuri);
        for (int j = 0; j < liste[i].nrTaskuri; j++) {
            fprintf(file, "%s\n", liste[i].taskuri[j].descriere);
            fprintf(file, "%d\n", liste[i].taskuri[j].completata);
        }
    }
    fclose(file);
    printf("Datele au fost %ssalvate%s! Verifica fisierul 'todo_data.txt'.", COLOR_GREEN, COLOR_RESET);
}

void incarcaDate() {
    FILE *file = fopen("todo_data.txt", "r");
    if (!file) {
        printf("Fisierul 'todo_data.txt' %snu%s a fost %sgasit%s!", COLOR_RED, COLOR_RESET, COLOR_RED, COLOR_RESET);
        return;
    }
    if (fscanf(file, "%d", &nrListe) != 1) {
        printf("%sEroare%s la citirea numarului de liste!", COLOR_RED, COLOR_RESET);
        fclose(file);
        return;
    }
    fgetc(file);

    for (int i = 0; i < nrListe; i++) {
        fgets(liste[i].titlu, MAX_LENGTH, file);
        liste[i].titlu[strcspn(liste[i].titlu, "\n")] = '\0';
        if (fscanf(file, "%d", &liste[i].nrTaskuri) != 1) {
            printf("%sEroare%s la citirea numarului de taskuri pentru lista %d!", COLOR_RED, COLOR_RESET, i);
            break;
        }
        fgetc(file);
        for (int j = 0; j < liste[i].nrTaskuri; j++) {
            fgets(liste[i].taskuri[j].descriere, MAX_LENGTH, file);
            liste[i].taskuri[j].descriere[strcspn(liste[i].taskuri[j].descriere, "\n")] = '\0';
            fscanf(file, "%d", &liste[i].taskuri[j].completata);
            fgetc(file);
        }
    }
    fclose(file);
}

void afiseazaFisier() {
    FILE *file = fopen("todo_data.txt", "r");
    if (!file) {
        printf("%sNu%s %sexista%s fisier de date!", COLOR_RED, COLOR_RESET, COLOR_RED, COLOR_RESET);
        return;
    }
    int nr;
    fscanf(file, "%d", &nr);
    printf("%sNumar liste %ssalvate%s:%s %d", COLOR_MAGENTA, COLOR_GREEN, COLOR_RESET, COLOR_MAGENTA, nr);
    fclose(file);
}

void clearScreen() {
    printf("\033[H\033[J");
}

void afiseazaMeniuPrincipal() {
    clearScreen();
    printf("%s===== MENIU PRINCIPAL =====%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("1. %sAdauga lista To-Do%s\n", COLOR_CYAN, COLOR_RESET);
    printf("2. %sAfiseaza liste To-Do%s\n", COLOR_CYAN, COLOR_RESET);
    printf("3. %sMarcheaza activitate%s\n", COLOR_CYAN, COLOR_RESET);
    printf("4. %sAjutor%s\n", COLOR_CYAN, COLOR_RESET);
    printf("5. %sIesire%s\n", COLOR_CYAN, COLOR_RESET);
    printf("6. %sVerifica datele salvate%s\n", COLOR_CYAN, COLOR_RESET);
    printf("Alege o optiune: ");
}

void afiseazaHelp() {
    clearScreen();
    printf("%s===== AJUTOR =====%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("1. Adauga lista To-Do - Creeaza o noua lista de activitati\n");
    printf("2. Afiseaza liste To-Do - Vizualizeaza toate listele existente\n");
    printf("3. Marcheaza activitate - Bifeaza o activitate ca efectuata\n");
    printf("4. Ajutor - Afiseaza acest mesaj\n");
    printf("5. Iesire - Inchide aplicatia\n");
}

void adaugaListaMeniu() {
    clearScreen();
    printf("%s===== ADAUGĂ LISTĂ TO-DO =====%s\n", COLOR_MAGENTA, COLOR_RESET);
    if (nrListe >= MAX_LISTS) {
        printf("%sNu mai poti adauga liste!%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    printf("Introduceti %stitlul%s listei To-Do (max 50 caractere): ", COLOR_MAGENTA, COLOR_RESET);
    getchar();
    fgets(liste[nrListe].titlu, MAX_LENGTH, stdin);
    liste[nrListe].titlu[strcspn(liste[nrListe].titlu, "\n")] = '\0';

    int nrTaskuri = citesteNumarIntre("Numar de activități în listă (1 - 10): ", 1, MAX_TASKS);
    if (nrTaskuri == -1) return;

    liste[nrListe].nrTaskuri = nrTaskuri;
    for (int i = 0; i < nrTaskuri; i++) {
        printf("%sDescrierea%s activității %d: ", COLOR_MAGENTA, COLOR_RESET, i + 1);
        fgets(liste[nrListe].taskuri[i].descriere, MAX_LENGTH, stdin);
        liste[nrListe].taskuri[i].descriere[strcspn(liste[nrListe].taskuri[i].descriere, "\n")] = '\0';
        liste[nrListe].taskuri[i].completata = 0;
    }
    nrListe++;
    salveazaDate();
    printf("Lista To-Do a fost adaugată cu %ssucces%s!", COLOR_GREEN, COLOR_RESET);
}

void afiseazaListe() {
    clearScreen();
    printf("%s===== LISTE TO-DO =====%s\n", COLOR_MAGENTA, COLOR_RESET);
    if (nrListe == 0) {
        printf("%sNu%s %sexista%s liste To-Do!", COLOR_RED, COLOR_RESET, COLOR_RED, COLOR_RESET);
        return;
    }
    for (int i = 0; i < nrListe; i++) {
        printf("-=%s %s %s=-\n", COLOR_CYAN, liste[i].titlu, COLOR_RESET);
        for (int j = 0; j < liste[i].nrTaskuri; j++) {
            printf("[%c] %s%s%s\n",
                   liste[i].taskuri[j].completata ? 'x' : ' ',
                   liste[i].taskuri[j].completata ? COLOR_GREEN : COLOR_YELLOW,
                   liste[i].taskuri[j].descriere,
                   COLOR_RESET);
        }
        printf("\n");
    }
}

void marcheazaActivitate() {
    clearScreen();
    printf("%s===== MARCHEAZĂ ACTIVITATE =====%s\n", COLOR_MAGENTA, COLOR_RESET);

    if (nrListe == 0) {
        printf("%sNu există liste disponibile.%s\n", COLOR_RED, COLOR_RESET);
        return;
    }

    char prompt1[100];
    sprintf(prompt1, "%sIntrodu indicele listei (0 - %d): %s", COLOR_CYAN, nrListe - 1, COLOR_RESET);
    int listaIndex = citesteNumarIntre(prompt1, 0, nrListe - 1);
    if (listaIndex == -1) return;

    if (liste[listaIndex].nrTaskuri == 0) {
        printf("%sAceastă listă nu are activități.%s\n", COLOR_RED, COLOR_RESET);
        return;
    }

    char prompt2[100];
    sprintf(prompt2, "%sIntrodu indicele activității (0 - %d): %s", COLOR_CYAN, liste[listaIndex].nrTaskuri - 1, COLOR_RESET);
    int taskIndex = citesteNumarIntre(prompt2, 0, liste[listaIndex].nrTaskuri - 1);
    if (taskIndex == -1) return;

    liste[listaIndex].taskuri[taskIndex].completata = 1;
    salveazaDate();
    printf("Activitate marcată ca %sefectuată%s!\n", COLOR_GREEN, COLOR_RESET);
}


int main() {
    incarcaDate();
    int optiune;
    while (1) {
        afiseazaMeniuPrincipal();
        scanf("%d", &optiune);
        getchar();
        switch (optiune) {
            case 1:
                adaugaListaMeniu();
                break;
            case 2:
                afiseazaListe();
                break;
            case 3:
                marcheazaActivitate();
                break;
            case 4:
                afiseazaHelp();
                break;
            case 5:
                printf("%sIesire...%s", COLOR_YELLOW, COLOR_RESET);
                return 0;
            case 6:
                afiseazaFisier();
                break;
            default:
                printf("%sOptiune invalida!%s\n", COLOR_RED, COLOR_RESET);
        }
        printf("Apasa %sEnter%s pentru a continua...", COLOR_MAGENTA, COLOR_RESET);
        getchar();
    }
    return 0;
}