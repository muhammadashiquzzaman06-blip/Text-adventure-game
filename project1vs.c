#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>



#define MAX_RECORDS 100
#define MAX_NAME_LENGTH 50
#define FILE_NAME "game_history.txt"
#define MAX_DATE_LENGTH 20
#define MAX_RESULT_LENGTH 20

void load_history_from_file(void);
void save_history_to_file(void);
void sort_history_by_score(void);
void view_scoreboard(void);
void clear_scoreboard(void);
void search_history_by_name(void);

typedef struct
{
    int id;
    char player_name[MAX_NAME_LENGTH];
    char date[MAX_DATE_LENGTH];
    int score;
    char result[MAX_RESULT_LENGTH];
} GameRecord;

GameRecord history[MAX_RECORDS];
int record_count = 0;

void load_history_from_file(void)
{
    FILE *file = fopen("FILE_NAME ", "r");
    if (file == NULL)
    {
        printf("\t[System Note] \t\nNo game history found. Please play a game first.\n");
        return;
    }
    record_count = 0;
    while (record_count < MAX_RECORDS && fscanf(file, "%d %49s %19s %d %19s", &history[record_count].id, history[record_count].player_name, history[record_count].date, & history[record_count].score, history[record_count].result) == 5)
    {
        record_count++;
    }
fclose(file);
printf("\t[System Note]\t\nLoaded %d game history records from file.\n\n", record_count);

}

void save_history_to_file(void)
{
    FILE *file = fopen("FILE_NAME ", "w");
    if (file == NULL)
    {
        printf("\t[System Note]\t\nFailed to open file for writing.\n");
        return;
    }
    for (int i = 0; i < record_count; i++)
    {
        fprintf(file, "%d %s %s %d %s\n", history[i].id, history[i].player_name, history[i].date, history[i].score, history[i].result);
    }
    fclose(file);
    printf("\t[System Note]\t\nSaved %d game history records to '%s'.\n\n", record_count , FILE_NAME);
}

void sort_history_by_score(void)
{
    for (int i = 0; i < record_count - 1; i++)
    {
        for (int j = 0; j < record_count - i - 1; j++)
        {
            if (history[j].score < history[j + 1].score)
            {
                GameRecord temp = history[j];
                history[j] = history[j + 1];
                history[j + 1] = temp;
            }
        }
    }
}
void view_scoreboard(void)
{
    if (record_count == 0)
    {
        printf("\t[System Note]\t\nNo game history found. Please play a game first.\n");
        return;
    }

    sort_history_by_score();

    printf("\t[Scoreboard]\t\n");
    printf("--------------------------------------------------\n");
    printf("| %-5s | %-20s | %-10s | %-5s | %-10s |\n", "ID", "Player Name", "Date", "Score", "Result");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < record_count; i++)
    {
        printf("| %-5d | %-20s | %-10s | %-5d | %-10s |\n", history[i].id, history[i].player_name, history[i].date, history[i].score, history[i].result);
    }
    printf("--------------------------------------------------\n\n");
}

void clear_scoreboard(void)
{
    record_count = 0;
    FILE *file = fopen("FILE_NAME ", "w");
    if (file != NULL)
    {
        fclose(file);
        printf("\t[System Note]\t\nGame history cleared successfully.\n\n");
    }
    else
    {
        printf("\t[System Note]\t\nNO GAME HISTORY FOUND.\n\n");
    }
}


void search_history_by_name(void)
{
    if (record_count == 0)
    {
        printf("\t[System Note]\t\nNo game history found. Please play a game first.\n");
        return;
    }

    char search_name[MAX_NAME_LENGTH];
    printf("\t[Search Game History by Player Name]\t\n\n");
    printf("\t[!!Use same name as entered when playing!!]\t\n");
    printf("Enter player name to search: ");
    scanf("%49s", search_name);

    int found = 0;
    printf("--------------------------------------------------\n");
    printf("| %-5s | %-20s | %-10s | %-5s | %-10s |\n", "ID", "Player Name", "Date", "Score", "Result");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < record_count; i++)
    {
        if (strcmp(history[i].player_name, search_name) == 0)
        {
            printf("| %-5d | %-20s | %-10s | %-5d | %-10s |\n", history[i].id, history[i].player_name, history[i].date, history[i].score, history[i].result);
            found = 1;
        }
    }
    if (!found)
    {
        printf("\t[System Note]\t\nNo records found for player name '%s'.\n\n", search_name);
    }
    else
    {
        printf("--------------------------------------------------\n\n");
    }
}
