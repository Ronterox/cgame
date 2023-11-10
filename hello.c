#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//MAP SIZE
#define ROWS 8
#define COLUMNS 8

//INVENTORY SIZE
#define INV_ROWS 2
#define INV_COLUMNS 3

//GAME OBJECTS
#define PLAYER 'P'
#define UNKNOWN 'o'
#define ENEMY 'E'
#define DOOR 'D'
#define EMPTY '.'

//APPEARENCE PERCENTAGE
#define PER_EMPTY 75 
#define PER_ENEMY 20
#define PER_DOOR 5

//DAMAGE PERCENTAGE
#define PER_DMG_ENEMY 50

//REWARD PERCENTAGE
#define PER_REWARD_ENEMY 60
#define PER_REWARD_CHEST 80

//ITEM PERCENTAGE

//ACTION KEYS
#define UP 'w'
#define DOWN 's'
#define LEFT 'a' 
#define RIGHT 'd' 
#define INTERACT 'i' 
#define INVENTORY 'e' 
#define EXIT 'q'

//COLORS FOR PRINTF CHANGE
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define CLEAR "\033[0m"

//TODO: create a class/struct
//This is not going to work, you really need object oriented programming for this kind of game

int main()
{
    bool isOver = false;
    //TODO: Look at how to concatenate strings for these
    char actionText[] = "\tGame Started!";

    void set_text(char text[])
    {
        actionText = "\n\t" + text + '\n';
    }

    //MAP DATA
    char map[ROWS][COLUMNS];
    char actualMap[ROWS][COLUMNS];

    //RANDOM DATA
    int doors, enemies, spaces = 0;
    int totalPercentage = PER_EMPTY + PER_DOOR + PER_ENEMY;

    //SET SEED FOR RANDOM
    srand(time(NULL));

    //PLAYER POS
    int playerX = 0, playerY = 0;

    //INVENTORY SETTINGS
    char* inventory[INV_ROWS * INV_COLUMNS];
    bool showInventory = false;

    for(int i=0; i<INV_ROWS * INV_COLUMNS; ++i) inventory[i] = "Empty";
    
    void set_player_initpos()
    {
        playerX = rand() % COLUMNS;
        playerY = rand() % ROWS;
    }

    char get_object()
    {
        int rng = rand() % totalPercentage + 1;
        if (rng <= PER_DOOR)
        {
            doors++;
            return DOOR;
        }
        else if(rng <= PER_ENEMY)
        {
            enemies++;
            return ENEMY;
        }
        else
        {
            spaces++;
            return EMPTY;
        }
    }

    void generate_map()
    {
        for(int i=0; i<ROWS; i++)
        {
            for(int j=0; j<COLUMNS; j++)
            {
                map[i][j] = UNKNOWN;
                actualMap[i][j] = get_object();
            }
        }
    }

    void check_position()
    {
        char preText[] = "You move and find..."; 

        switch(map[playerY][playerX])
        {
            case EMPTY:
                set_text("Nothing on this block!");
                break;
            case ENEMY:
                set_text("An Enemy!");
                break;
            case DOOR:
                set_text("A Door!");
                break;
        }
        printf("\n");
    }

    void update_close_cells()
    {
        //CLEAR AREA AROUND PLAYER
        //TODO: Fix out of bonds, check for inside of map
        map[playerY][playerX] = actualMap[playerY][playerX];

        map[playerY + 1][playerX] = actualMap[playerY + 1][playerX];
        map[playerY - 1][playerX] = actualMap[playerY - 1][playerX];

        map[playerY][playerX + 1] = actualMap[playerY][playerX + 1];
        map[playerY][playerX - 1] = actualMap[playerY][playerX - 1];
    }

    void use(int item)
    {
       printf("Use item %d [%s]", item, inventory[item]); 
    }

    void draw_stats()
    {
        printf("%s\n\tStats:\n\t\tDoors: %d\n\t\tEnemies:%d\n\t\tSpaces:%d\n\t\tTotal Percentage: %d%\n\n%s",
               YELLOW,
               doors, enemies, spaces, totalPercentage,
               CLEAR);
    }

    void draw_map()
    {
        printf(PURPLE);
        printf("\n\tFirst Room(%d, %d)\n", playerX, playerY);
        printf("\t---------------------------------------------\n");
        printf(CLEAR);

        for(int i=0; i<ROWS; i++)
        { 
            printf("\t");
            for(int j=0; j<COLUMNS; j++)
            {
                if (i == playerY && j == playerX)
                {
                    printf(CYAN);
                    printf(" %c", PLAYER);
                    printf(CLEAR);
                }
                else
                {
                    printf("  %c",map[i][j]);
                }
            }
            printf("\t\n");
        }

        printf(PURPLE);
        printf("\t---------------------------------------------\n");
        printf(CLEAR);
    }

    void draw_inventory()
    {
        printf(YELLOW);
        printf("\n\tINVENTORY:\n");
        for(int i=0; i<INV_ROWS; ++i)
        {
            printf("\t\t");
            for(int j=0; j<INV_COLUMNS; ++j)
            {
                printf("[%s]\t", inventory[i+j]);
            }
            printf("\n\n");
        }
        printf(CLEAR);
    }

    void draw_body()
    {
        printf("%s\tBody State\n%s", RED, CLEAR);
        printf("\t\t  0\n");
        printf("\t\t--0--\n");
        printf("\t\t  0\n");
        printf("\t\t | |\n\n");
    }

    void draw()
    {
        // system("clear");
        printf("\n\n");

        check_position();
        printf(actionText);
        draw_map();

        if (showInventory) draw_inventory();
        else draw_stats();

        draw_body();
    }

    void logic()
    {
        //GET DECISION
        char move = EXIT;

        printf("\tUp    [%c], Down  [%c], Left  [%c], Right [%c], Interact [%c], Inventory [%c]", 
                UP, DOWN, LEFT, RIGHT, INTERACT, INVENTORY);
        printf("\n\n\tExit  [%c]", EXIT);
        printf("\n\nWhere do you wanna move?: ");
        scanf("%c", &move);

        switch(move)
        {
            case UP:
                playerY = playerY - 1;
                if (playerY < 0) playerY = ROWS - 1;
                break;
            case DOWN:
                playerY = (playerY + 1) % ROWS;
                break;
            case LEFT:
                playerX = playerX - 1;
                if (playerX < 0) playerX = COLUMNS - 1;
                break;
            case RIGHT:
                playerX = (playerX + 1) % COLUMNS;
                break;
            case INTERACT:
                switch(actualMap[playerY][playerX])
                {
                    case ENEMY:
                        actualMap[playerY][playerX] = 'x';
                        break;
                    case DOOR:
                        set_player_initpos();
                        generate_map();
                        break;
                    case EMPTY:
                        break;
                }
                break;
            case INVENTORY:
                showInventory = !showInventory;
                break;
            case EXIT:
                isOver = true;
                break;
            default:
                if(isdigit(move)) use(move - '0' - 1);
                break;
        }

        update_close_cells();
    }

    set_player_initpos();
    generate_map();
    update_close_cells();
    draw();

    while (!isOver)
    {
        logic();
        draw();
    }

    return 0;
}

void method()
{
    int a = 152
    int b = 153
    printf("Hello there, this is a test");

    char[] reasons = "I'll love to actually write something on c++");
}
