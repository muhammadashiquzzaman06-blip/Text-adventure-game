#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define MAX_NAME_LENGTH 50
#define FILE_NAME "game_history.txt"
#define MAX_RESULT_LENGTH 20

#define INIT_CORN_COUNT 2
#define INIT_PAN_COUNT 1
#define INIT_LEMBAS_COUNT 1

#define SCORE_STANDARD 20
#define SCORE_BONUS 25

typedef struct
{
    int id;
    char player_name[MAX_NAME_LENGTH];
    int score;
    char result[MAX_RESULT_LENGTH];
} GameRecord; // saving player information input

typedef struct
{
    int corn_count;
    int pan_count;
    int lembas_count;
} Inventory; // accounting for the count of inventory items

GameRecord history[MAX_RECORDS];
int record_count = 0;

void load_history_from_file(void);
void save_history_to_file(void);
void sort_history_by_score(void);
void view_scoreboard(void);
void clear_scoreboard(void);
void search_history_by_name(void);

void init_inventory(Inventory *inv);
void display_inventory(const Inventory *inv);
int check_and_use_lembas(Inventory *inv);
int get_valid_choice(int max_option);
void add_record(const char *name, int score, const char *result);

int play_prologue(Inventory *pack);
int play_act1(Inventory *pack, int *score);
int play_act2(Inventory *pack, int *score);
int play_act3(Inventory *pack, int *score);
int play_act4(Inventory *pack, int *score);
void play_game(void);
void show_rules(void);
//this function load txt from the file
void load_history_from_file(void)
{
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL)
    {
        return;
    }
    record_count = 0;
    while (record_count < MAX_RECORDS && fscanf(file, "%d %49s %d %19s",
           &history[record_count].id,
           history[record_count].player_name,
           &history[record_count].score,
           history[record_count].result) == 4)
    {
        record_count++;
    }
    fclose(file);
}
//this function save scoreboard to file
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
        fprintf(file, "%d %s %d %s\n", history[i].id, history[i].player_name, history[i].score, history[i].result);
    }
    fclose(file);
    printf("\n\t[System Note] Saved %d game history records.\n\n", record_count);
}

//this function is only for sorting
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

//this function is for view the score board again
void view_scoreboard(void)
{
    if (record_count == 0)
    {
        printf("\n\t[System Note] No game history found. Please play a game first.\n");
        return;
    }

    sort_history_by_score();

    printf("\n\t[Scoreboard]\t\n");
    printf("---------------------------------------------------\n");
    printf("| %-5s | %-20s | %-5s | %-10s |\n", "ID", "Player Name", "Score", "Result");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < record_count; i++)
    {
        printf("| %-5d | %-20s | %-5d | %-10s |\n", history[i].id, history[i].player_name, history[i].score, history[i].result);
    }
    printf("---------------------------------------------------\n\n");
}

//this function helps us to clear the scoreboard
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
        printf("\n\t[System Note] Unable to clear game history file.\n\n");
    }
}

//by the help of this function we can search history
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
    printf("---------------------------------------------------\n");
    printf("| %-5s | %-20s | %-5s | %-10s |\n", "ID", "Player Name", "Score", "Result");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < record_count; i++)
    {
        if (strcmp(history[i].player_name, search_name) == 0)
        {
            printf("| %-5d | %-20s | %-5d | %-10s |\n", history[i].id, history[i].player_name, history[i].score, history[i].result);
            found = 1;
        }
    }
    if (!found)
    {
        printf("\t[System Note] No records found for player name '%s'.\n\n", search_name);
    }
    else
    {
        printf("---------------------------------------------------\n\n");
    }
}

//after game play this function save record to file
void add_record(const char *name, int score, const char *result)
{
    if (record_count >= MAX_RECORDS)
    {
        printf("\n\t[System Note] Game history log is full.\n");
        return;
    }

    GameRecord new_rec;
    new_rec.id = record_count + 1;
    strncpy(new_rec.player_name, name, MAX_NAME_LENGTH - 1);
    new_rec.player_name[MAX_NAME_LENGTH - 1] = '\0';

    new_rec.score = score;
    strncpy(new_rec.result, result, MAX_RESULT_LENGTH - 1);
    new_rec.result[MAX_RESULT_LENGTH - 1] = '\0';

    history[record_count++] = new_rec;
    save_history_to_file();
}

void init_inventory(Inventory *inv) // inventory function
{
    inv->corn_count = INIT_CORN_COUNT;
    inv->pan_count = INIT_PAN_COUNT;
    inv->lembas_count = INIT_LEMBAS_COUNT;
}

void display_inventory(const Inventory *inv) //inventory item count
{
    printf("\n  ================ INVENTORY PACK ================");
    printf("\n   Shire Corn  : %d bag(s)", inv->corn_count);
    printf("\n   Frying Pan  : %d unit(s)", inv->pan_count);
    printf("\n   Lembas Bread: %d piece(s) (Auto-revive)", inv->lembas_count);
    printf("\n  ================================================\n");
}

int check_and_use_lembas(Inventory *inv) //inventory item count for lembas
{
    if (inv->lembas_count > 0)
    {
        inv->lembas_count--;
        printf("\n[SPECIAL ITEM ACTIVATED!] You consumed 1x Lembas Bread!");
        printf("\nIts elven vitality revives you from fatal danger! You barely survive!\n");
        return 1;
    }
    return 0;
}

int get_valid_choice(int max_option) //gameplay choice option validation
{
    int choice = 0;
    while (1)
    {
        printf("Enter your choice (1-%d): ", max_option);
        if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= max_option)
        {
            return choice;
        }
        while (getchar() != '\n');
        printf("Invalid input! Please enter a number between 1 and %d.\n", max_option);
    }
}

int play_prologue(Inventory *pack) // prologue act
{
    printf("\n--- PROLOGUE: RIVENDELL ---\n");
    printf("The Ring is tied around Barnaby's neck. Elrond turns to you:\n");
    printf("\"The task falls upon you, Samwise Gamgee. Follow the fowl.\"\n");
    printf("1. Pursue Barnaby into the wilderness immediately.\n");
    printf("2. Stop to search for extra salt and frying pans.\n");

    if (get_valid_choice(2) == 2)
    {
        printf("\nWhile searching, Barnaby gets too far ahead and a fox corners her!\n");
        if (!check_and_use_lembas(pack))
        {
            printf("Sauron reclaims the Ring!\n\nGAME OVER!\n");
            return 0;
        }
    }
    printf("\nYou pursue Barnaby and manage to keep up with her speed!\n");
    return 1;
}

int play_act1(Inventory *pack, int *score) //actI: BREE
{
    printf("\n--- ACT I: BREE ---\n");
    printf("In the Prancing Pony, a Nazgul corners Barnaby near the hearth!\n");
    printf("1. Let Barnaby handle it; she looks furious.\n");
    printf("2. Draw your sword and jump between them.\n");
    printf("3. Throw Shire Corn from your pack to distract the Nazgul (Remaining: %d).\n", pack->corn_count);

    int choice = get_valid_choice(3);
    if (choice == 2)
    {
        printf("\nYou step between them, but the Nazgul ignores you and grabs Barnaby!\n");
        if (!check_and_use_lembas(pack))
        {
            printf("GAME OVER!\n");
            return 0;
        }
    }
    else if (choice == 3)
    {
        if (pack->corn_count > 0)
        {
            pack->corn_count--;
            printf("\nYou scatter Shire Corn! The Nazgul trips on the grains and crashes!\n");
            *score += SCORE_BONUS;
        }
        else
        {
            printf("\nYour corn bag is empty! The Nazgul grabs Barnaby!\n");
            if (!check_and_use_lembas(pack))
            {
                printf("GAME OVER!\n");
                return 0;
            }
        }
    }
    else
    {
        printf("\nBarnaby pecks the Wraith's shin! The Nazgul flees through a window.\n");
        *score += SCORE_STANDARD;
    }
    return 1;
}

int play_act2(Inventory *pack, int *score) //actII: MORIA
{
    printf("\n--- ACT II: MORIA ---\n");
    printf("In Moria, Barnaby lays an egg. The noise awakens a Balrog!\n");
    printf("1. Tell Barnaby to flap her wings harder to raise a dust storm.\n");
    printf("2. Throw a stone to distract the demon.\n");
    printf("3. Smite the ground with your Heavy Frying Pan (Remaining: %d).\n", pack->pan_count);

    int choice = get_valid_choice(3);
    if (choice == 2)
    {
        printf("\nThe rock strikes the Balrog harmlessly. It destroys the bridge!\n");
        if (!check_and_use_lembas(pack))
        {
            printf("You fall into the abyss.\n\nGAME OVER!\n");
            return 0;
        }
    }
    else if (choice == 3)
    {
        if (pack->pan_count > 0)
        {
            pack->pan_count--;
            printf("\nCLANG! The pan vibration echoes loudly, breaking the Balrog's concentration!\n");
            *score += SCORE_BONUS;
        }
        else
        {
            printf("\nYou have no frying pan! The Balrog crushes the path!\n");
            if (!check_and_use_lembas(pack))
            {
                printf("GAME OVER!\n");
                return 0;
            }
        }
    }
    else
    {
        printf("\nBarnaby causes a dust cloud! The Balrog sneezes and falls off the cliff.\n");
        *score += SCORE_STANDARD;
    }
    return 1;
}

int play_act3(Inventory *pack, int *score) //actIII: GONDOR
{
    printf("\n--- ACT III: GONDOR ---\n");
    printf("Steward Denethor demands roasted fowl and dives at Barnaby with a fork!\n");
    printf("1. Do nothing and let Barnaby react.\n");
    printf("2. Throw Shire Corn to lure Denethor away (Remaining: %d).\n", pack->corn_count);

    int choice = get_valid_choice(2);
    if (choice == 2)
    {
        if (pack->corn_count > 0)
        {
            pack->corn_count--;
            printf("\nDenethor gets distracted pecking for corn on the floor! Barnaby escapes!\n");
            *score += SCORE_STANDARD;
        }
        else
        {
            printf("\nYou have no Shire corn left! Denethor catches Barnaby!\n");
            if (!check_and_use_lembas(pack))
            {
                printf("GAME OVER!\n");
                return 0;
            }
        }
    }
    else
    {
        printf("\nBarnaby mounts Denethor's scalp! He trips and falls into the fountain.\n");
        *score += SCORE_STANDARD;
    }
    return 1;
}

int play_act4(Inventory *pack, int *score) //actIV: MOUNT DOOM
{
    printf("\n--- ACT IV: MOUNT DOOM ---\n");
    printf("At the volcanic ledge, Barnaby refuses to let go of the Ring.\n");
    printf("1. Tackle Barnaby to snatch the Ring.\n");
    printf("2. Lure Barnaby toward the lava ledge with Shire Corn.\n");

    if (get_valid_choice(2) == 1)
    {
        printf("\nYou tackle Barnaby, but you both tumble into the magma!\n");
        if (!check_and_use_lembas(pack))
        {
            printf("GAME OVER!\n");
            return 0;
        }
    }

    printf("\nBarnaby lunges for corn, slips into the magma, destroys the Ring,\n");
    printf("and rides a pumice rock back up completely unharmed!\n");
    *score += SCORE_STANDARD;
    return 1;
}

void play_game(void) //gameplay function
{
    char player_name[MAX_NAME_LENGTH];
    int score = 0;
    Inventory pack;

    init_inventory(&pack);

    printf("\nEnter your player name (no spaces): ");
    scanf("%49s", player_name);

    printf("\n--- STARTING INVENTORY ---");
    display_inventory(&pack);

    if (!play_prologue(&pack))
    {
        add_record(player_name, score, "DEFEAT");
        return;
    }
    score += SCORE_STANDARD;

    if (!play_act1(&pack, &score))
    {
        add_record(player_name, score, "DEFEAT");
        return;
    }

    if (!play_act2(&pack, &score))
    {
        add_record(player_name, score, "DEFEAT");
        return;
    }

    if (!play_act3(&pack, &score))
    {
        add_record(player_name, score, "DEFEAT");
        return;
    }

    if (!play_act4(&pack, &score))
    {
        add_record(player_name, score, "DEFEAT");
        return;
    }

    printf("\nVICTORY! You and Barnaby saved Middle-earth!\n");
    printf("Final Score: %d\n", score);
    add_record(player_name, score, "VICTORY");
}

void show_rules(void) // game rule and inventory guide function
{
    printf("\n=====================================================\n");
    printf("              GAME RULES & INVENTORY GUIDE           \n");
    printf("=====================================================\n");
    printf("1. OBJECTIVE: Guide Samwise and Barnaby the chicken across 5 Acts\n");
    printf("   to destroy the Ring and reach Mount Doom.\n\n");

    printf("2. INVENTORY ITEMS & THEIR USEFULNESS:\n");
    printf("   [1] Shire Corn (x2 Start Qty)\n");
    printf("       - USEFULNESS: Distraction tool against enemies and lures Barnaby.\n");
    printf("       - ADVANTAGE: Grants highest point bonus when used correctly.\n\n");

    printf("   [2] Frying Pan (x1 Start Qty)\n");
    printf("       - USEFULNESS: Acoustic impact device for heavy combat.\n");
    printf("       - ADVANTAGE: Provides alternative survival paths in Act II.\n\n");

    printf("   [3] Lembas Bread (x1 Start Qty - Safety Net)\n");
    printf("       - USEFULNESS: Grants passive protection.\n");
    printf("       - ADVANTAGE: AUTOMATICALLY CONSUMED upon choosing a wrong path,\n");
    printf("         reviving you from death once per playthrough!\n");
    printf("=====================================================\n\n");
}

int main(void) //main function
{
    load_history_from_file(); //for showing play history
    int choice = 0;

    while (choice != 6)
    {
        //start menu
        printf("=====================================================\n");
        printf("              THE CHICKEN OF GONDOR                  \n");
        printf("=====================================================\n");
        printf("1. Start Game (Play Quest)\n");
        printf("2. View Game Rules & Hints\n");
        printf("3. View History Records\n");
        printf("4. Search Player Records\n");
        printf("5. Clear Scoreboard\n");
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
                play_game(); //start playing
                break;
            case 2:
                show_rules(); //showing game and inventory rules
                break;
            case 3:
                view_scoreboard(); //view scoreboard
                break;
            case 4:
                search_history_by_name(); //searching player history by name
                break;
            case 5:
                clear_scoreboard(); //initialize scoreboard
                break;
            case 6:
                printf("\nThank you for playing!\n"); //exiting game
                break;
            default:
                printf("\nInvalid option. Please choose between 1 and 6.\n\n"); //invalid input response
                break;
        }
    }

    return 0;
}
