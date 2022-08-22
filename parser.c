#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PLAYERS 30
#define MAX_MATCHES 22

unsigned short int games = 0, controlPlayers = 0;

typedef struct {
    char id[50];
    char name[50], oldNames[50];
    int kills;
} players;

typedef struct {
    unsigned short int totalKills;
    players acess[MAX_PLAYERS];
} quake;

void readLine(quake data[], FILE *txt, FILE *txtSaida);
bool checkInit(char *line);
void killCheck(char *word, quake data[]);
void writeJson(quake data[], FILE *txtSaida);
void sumKills(quake data[]);
void changeCheck(quake data[], FILE *txt);
void insertNames(char *vetChar, char *word, char *lineComplete);
void cleanData(quake data[]);

int main(int argc, char *argv[]) {
    FILE *arqv = fopen(argv[1], "r"), *arqvSaida;
    quake data[MAX_MATCHES];

    if (!arqv) {
        perror("error");
        exit(1);
    }

    for (size_t i = 0; i < MAX_MATCHES; i++) {
        data[i].totalKills = 0;
        for (size_t j = 0; j < MAX_PLAYERS; j++) {
            data[i].acess[j].kills = 0;
        }
    }
    arqvSaida = fopen(argv[2], "w+");

    if (!arqvSaida) {
        perror("error");
        exit(1);
    }
    fputs("[\n", arqvSaida);
    do {
        readLine(data, arqv, arqvSaida);
    } while (!feof(arqv));
    fputs("]\n", arqvSaida);
    fclose(arqv);
    fclose(arqvSaida);

    return 0;
}
void writeJson(quake data[], FILE *txtSaida) {
    sumKills(data);
    fputs("\t{\n", txtSaida);
    fprintf(txtSaida, "\t\t\"game\": %hd,\n", games);
    fputs("\t\t\"status\": {\n", txtSaida);
    fprintf(txtSaida, "\t\t\t\"total kills\": %hd,\n", data[games - 1].totalKills);
    fputs("\t\t\t\"players\": [\n", txtSaida);
    for (size_t j = 0; j < controlPlayers; j++) {
        fputs("\t\t\t\t{\n", txtSaida);
        fprintf(txtSaida, "\t\t\t\t\t\"id\": %s,\n", data[games - 1].acess[j].id);
        fprintf(txtSaida, "\t\t\t\t\t\"nome\": \"%s\",\n", data[games - 1].acess[j].name);
        fprintf(txtSaida, "\t\t\t\t\t\"kills\": %d,\n", data[games - 1].acess[j].kills);
        fprintf(txtSaida, "\t\t\t\t\t\"old_names\": [\"%s\"]\n", data[games - 1].acess[j].oldNames);
        (j == (controlPlayers - 1)) ? fputs("\t\t\t\t}\n", txtSaida) : fputs("\t\t\t\t},\n", txtSaida);
    }
    fputs("\t\t\t]\n", txtSaida);
    fputs("\t\t}\n", txtSaida);
    games == MAX_MATCHES - 1 ? fputs("\t}\n", txtSaida) : fputs("\t},\n", txtSaida);
}

void cleanData(quake data[]) {
    for (size_t i = 0; i < MAX_PLAYERS; i++) {
        memset(data[games - 1].acess[i].id, '\0', 50);
        memset(data[games - 1].acess[i].name, '\0', 50);
        memset(data[games - 1].acess[i].oldNames, '\0', 50);
        data[games - 1].acess[i].kills = 0;
    }
    data[games - 1].totalKills = 0;
    controlPlayers = 0;
}

void readLine(quake data[], FILE *txt, FILE *txtSaida) {
    char line[400], lineComplete[400];
    fgets(line, 400, txt);

    if (checkInit(line) == true) {
        char *word;
        word = strtok(line, " ");
        do {
            while (word) {
                if (!strcmp("ClientUserinfoChanged:", word)) {
                    insertNames(data[games - 1].acess[controlPlayers].oldNames, word, lineComplete);
                    changeCheck(data, txt);
                }
                if (!strcmp("Kill:", word)) {
                    killCheck(word, data);
                }
                if (!strcmp("ShutdownGame:\r\n", word)) {
                    writeJson(data, txtSaida);
                    cleanData(data);
                    return;
                }
                word = strtok(NULL, " ");
            }
            memset(line, '\0', 400);
            fgets(line, 400, txt);
            strcpy(lineComplete, line);
            word = strtok(line, " ");
        } while (true);
    }
}
void killCheck(char *word, quake data[]) {
    word = strtok(NULL, " ");

    if (strcmp("1022", word) != 0) {
        for (size_t j = 0; j < MAX_PLAYERS; j++) {
            if (!strcmp(data[games - 1].acess[j].id, word)) {
                data[games - 1].acess[j].kills += 1;
                return;
            }
        }

        strcpy(data[games - 1].acess[controlPlayers].id, word);
        word = strtok(NULL, " ");
        word = strtok(NULL, " ");
        word = strtok(NULL, " ");
        strcpy(data[games - 1].acess[controlPlayers].name, word);

        word = strtok(NULL, " ");
        char *pAux;
        while (strcmp(word, "killed") != 0) {
            pAux = strchr(data[games - 1].acess[controlPlayers].name, '\0');
            *pAux = ' ';
            strcat(data[games - 1].acess[controlPlayers].name, word);
            word = strtok(NULL, " ");
        }
        data[games - 1].acess[controlPlayers].kills += 1;
        controlPlayers++;
    } else {
        word = strtok(NULL, " ");
        word = strtok(NULL, " ");
        word = strtok(NULL, " ");
        word = strtok(NULL, " ");
        word = strtok(NULL, " ");
        data[games - 1].totalKills += 1;
        for (size_t j = 0; j < MAX_PLAYERS; j++) {
            if (!strcmp(data[games - 1].acess[j].name, word)) {
                data[games - 1].acess[j].kills -= 1;
                return;
            }
        }
    }
}
bool checkInit(char *line) {
    char *word = strtok(line, " ");
    while (word) {
        if (!strcmp("InitGame:", word)) {
            games += 1;
            return true;
        }
        word = strtok(NULL, " ");
    }
    return false;
}
void sumKills(quake data[]) {
    int sum = 0;
    for (size_t i = 0; i < controlPlayers; i++) {
        sum += data[games - 1].acess[i].kills;
    }
    data[games - 1].totalKills += sum;
}
void changeCheck(quake data[], FILE *txt) {
    char line[400], lineComplete[400], *word;

    fgets(line, 400, txt);
    strcpy(lineComplete, line);
    word = strtok(line, " ");
    word = strtok(NULL, " ");
    if (!strcmp("ClientUserinfoChanged:", word)) {
        word = strtok(NULL, " ");
        strcpy(data[games - 1].acess[controlPlayers].id, word);
        insertNames(data[games - 1].acess[controlPlayers].name, word, lineComplete);
        controlPlayers++;
    }
}
void insertNames(char *vetChar, char *word, char *lineComplete) {
    unsigned short int cont = 0;
    word = strchr(lineComplete, '\\');
    word += 1;
    strcpy(lineComplete, word);
    while (lineComplete[cont] != '\\') {
        vetChar[cont] = lineComplete[cont];
        cont++;
    }
    vetChar[cont] = '\0';
}