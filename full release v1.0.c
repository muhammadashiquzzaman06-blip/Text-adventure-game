#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_RECORDS 100
#define MAX_NAME_LENGTH 50
#define FILE_NAME "game_history.txt"
#define MAX_DATE_LENGTH 20
#define MAX_RESULT_LENGTH 20

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

void load_history_from_file(void);
void save_history_to_file(void);
void sort_history_by_score(void);
void view_scoreboard(void);
void clear_scoreboard(void);
void search_history_by_name(void);
void play_game(void);
void show_rules(void);
int get_choice(void);
void add_record(const char *name, int score, const char *result);

void load_history_from_file(void)
{
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL)
    {
        return;
    }
    record_count = 0;
    while (record_count < MAX_RECORDS && fscanf(file, "%d %49s %19s %d %19s",
           &history[record_count].id,
           history[record_count].player_name,
           history[record_count].date,
           &history[record_count].score,
           history[record_count].result) == 5)
    {
        record_count++;
    }
    fclose(file);
}

void save_history_to_file(void)
{
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL)
    {
        printf("\n\t[System Note] Failed to open file for writing.\n");
        return;
    }
    for (int i = 0; i < record_count; i++)
    {
        fprintf(file, "%d %s %s %d %s\n", history[i].id, history[i].player_name, history[i].date, history[i].score, history[i].result);
    }
    fclose(file);
    printf("\n\t[System Note] Saved %d game history records.\n\n", record_count);
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
        printf("\n\t[System Note] No game history found. Please play a game first.\n");
        return;
    }

    sort_history_by_score();

    printf("\n\t[Scoreboard]\t\n");
    printf("----------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-10s | %-5s | %-10s |\n", "ID", "Player Name", "Date", "Score", "Result");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < record_count; i++)
    {
        printf("| %-5d | %-20s | %-10s | %-5d | %-10s |\n", history[i].id, history[i].player_name, history[i].date, history[i].score, history[i].result);
    }
    printf("----------------------------------------------------------------------\n\n");
}

void clear_scoreboard(void)
{
    record_count = 0;
    FILE *file = fopen(FILE_NAME, "w");
    if (file != NULL)
    {
        fclose(file);
        printf("\n\t[System Note] Game history cleared successfully.\n\n");
    }
    else
    {
        printf("\n\t[System Note] NO GAME HISTORY FOUND.\n\n");
    }
}

void search_history_by_name(void)
{
    if (record_count == 0)
    {
        printf("\n\t[System Note] No game history found. Please play a game first.\n");
        return;
    }

    char search_name[MAX_NAME_LENGTH];
    printf("\n\t[Search Game History by Player Name]\t\n");
    printf("Enter player name to search: ");
    scanf("%49s", search_name);

    int found = 0;
    printf("----------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-10s | %-5s | %-10s |\n", "ID", "Player Name", "Date", "Score", "Result");
    printf("----------------------------------------------------------------------\n");
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
        printf("\t[System Note] No records found for player name '%s'.\n\n", search_name);
    }
    else
    {
        printf("----------------------------------------------------------------------\n\n");
    }
}

int get_choice(void)
{
    int choice;
    printf("\nEnter your choice (1 or 2): ");
    while (scanf("%d", &choice) != 1 || (choice != 1 && choice != 2))
    {
        while (getchar() != '\n'); // clear input buffer
        printf("Invalid input. Please enter 1 or 2: ");
    }
    return choice;
}

void add_record(const char *name, int score, const char *result)
{
    if (record_count >= MAX_RECORDS)
    {
        printf("\n\t[System Note] Game history log is full.\n");
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    GameRecord new_rec;
    new_rec.id = record_count + 1;
    strncpy(new_rec.player_name, name, MAX_NAME_LENGTH - 1);
    new_rec.player_name[MAX_NAME_LENGTH - 1] = '\0';
    snprintf(new_rec.date, MAX_DATE_LENGTH, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    new_rec.score = score;
    strncpy(new_rec.result, result, MAX_RESULT_LENGTH - 1);
    new_rec.result[MAX_RESULT_LENGTH - 1] = '\0';

    history[record_count++] = new_rec;
    save_history_to_file();
}

void play_game(void)
{
    char player_name[MAX_NAME_LENGTH];
    int score = 0;

    printf("\nEnter your character/player name: ");
    scanf("%49s", player_name);

    printf("\n--- PROLOGUE: RIVENDELL ---\n");
    printf("The Ring is tied around Barnaby's neck. Elrond turns to you:\n");
    printf("\"The task falls upon you, Samwise Gamgee. Follow the fowl.\"\n");
    printf("1. Pursue Barnaby into the wilderness immediately.\n");
    printf("2. Stop to pack a fourth frying pan and extra salt.\n");

    if (get_choice() == 2)
    {
        printf("\nWhile searching for salt, Barnaby gets too far ahead.\n");
        printf("Barnaby is caught by a fox. Sauron reclaims the Ring!\n");
        printf("\nGAME OVER!\n");
        add_record(player_name, score, "DEFEAT");
        return;
    }

    printf("\nYou pursue Barnaby and keep up with her speed!\n");
    score += 25;

    printf("\n--- ACT I: BREE ---\n");
    printf("In the Prancing Pony, a Nazgul corners Barnaby near the hearth!\n");
    printf("Barnaby aggressively pecks the Wraith's iron boots.\n");
    printf("1. Let Barnaby handle it; she looks furious.\n");
    printf("2. Draw your sword and jump between them.\n");

    if (get_choice() == 2)
    {
        printf("\nYou step between them. The Nazgul ignores you and grabs Barnaby!\n");
        printf("Sauron reclaims the Ring!\n");
        printf("\nGAME OVER!\n");
        add_record(player_name, score, "DEFEAT");
        return;
    }

    printf("\nBarnaby pecks the Wraith's shin! The Nazgul flees through a window.\n");
    score += 25;

    printf("\n--- ACT II: MORIA ---\n");
    printf("In Moria, Barnaby lays an egg. The noise awakens a Balrog!\n");
    printf("Barnaby ruffles her feathers and kicks up ancient dust.\n");
    printf("1. Tell Barnaby to flap her wings harder.\n");
    printf("2. Throw a stone to distract the demon.\n");

    if (get_choice() == 2)
    {
        printf("\nThe rock harmlessly strikes the Balrog. It destroys the bridge!\n");
        printf("You both fall into the abyss.\n");
        printf("\nGAME OVER!\n");
        add_record(player_name, score, "DEFEAT");
        return;
    }

    printf("\nBarnaby causes a dust cloud! The Balrog sneezes and falls off the cliff.\n");
    score += 25;

    printf("\n--- ACT III: GONDOR ---\n");
    printf("Steward Denethor demands a roasted bird and dives at Barnaby with a fork!\n");
    printf("1. Throw Shire corn to distract Denethor.\n");
    printf("2. Do nothing and let Barnaby react.\n");

    if (get_choice() == 1)
    {
        printf("\nDenethor ignores the corn and catches Barnaby!\n");
        printf("\nGAME OVER!\n");
        add_record(player_name, score, "DEFEAT");
        return;
    }

    printf("\nBarnaby mounts Denethor's scalp! He trips and falls into the fountain.\n");
    score += 25;

    printf("\n--- ACT IV: MOUNT DOOM ---\n");
    printf("At the volcanic ledge, Barnaby refuses to let go of the Ring.\n");
    printf("1. Tackle Barnaby to snatch the Ring.\n");
    printf("2. Lure Barnaby toward the ledge with Shire corn.\n");

    if (get_choice() == 1)
    {
        printf("\nYou tackle Barnaby, but you both tumble into the lava!\n");
        printf("\nGAME OVER!\n");
        add_record(player_name, score, "DEFEAT");
        return;
    }

    printf("\nBarnaby lunges for corn, slips into the magma, destroys the Ring,\n");
    printf("and rides a pumice rock back up completely unharmed!\n");
    score += 25;

    printf("\nVICTORY! You and Barnaby saved Middle-earth!\n");
    add_record(player_name, score, "VICTORY");
}

void show_rules(void)
{
    printf("\n--- GAME RULES & HINTS ---\n");
    printf("1. Trust the instincts of Barnaby the chicken.\n");
    printf("2. Every correct choice progresses the story and awards 25 points.\n");
    printf("3. Making a wrong choice results in immediate failure.\n");
    printf("4. Reach the end of Act IV with 100 points to secure victory!\n\n");
}

int main(void)
{
    load_history_from_file();
    int choice = 0;

    while (choice != 6)
    {
        printf("=====================================================\n");
        printf("     THE CHICKEN OF GONDOR\n");
        printf("=====================================================\n");
        printf("1. Start Game (Play Quest)\n");
        printf("2. View Game Rules & Hints\n");
        printf("3. View History Records\n");
        printf("4. Search Player Records\n");
        printf("5. Sort Records Table\n");
        printf("6. Quit Game\n");
        printf("=====================================================\n");
        printf("Select an option (1-6): ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            continue;
        }

        switch (choice)
        {
            case 1:
                play_game();
                break;
            case 2:
                show_rules();
                break;
            case 3:
                view_scoreboard();
                break;
            case 4:
                search_history_by_name();
                break;
            case 5:
                sort_history_by_score();
                printf("\n\t[System Note] Records sorted by score.\n\n");
                break;
            case 6:
                printf("\nThank you for playing!\n");
                break;
            default:
                printf("\nInvalid option. Please choose between 1 and 6.\n\n");
                break;
        }
    }

    return 0;
}
