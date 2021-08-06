#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_CARDS 28

#define MAX_OWNED_FACTORIES 3

const char * planet_array[] = {
    "Alhuna",
    "Thelae",
    "Jynide",
    "Queron",
    "Uanov",
    "Hether",
};

typedef struct Factory {
    char type[16];
    char owned; // -1 : false else index of player; ie: owned = 2, factory owned by player 3
    char rarity; // 0 : common, 1 : Epic, 2 : Legendary
    unsigned int gas_income;
    unsigned int minerals_income;
    unsigned int coin_income;

    char upgrade; //default : 1, goes up to 5; ie: upgrade = 2 -> 2x prod
} Factory;

typedef struct Factories_array {
    Factory** array;
    size_t len;
} Factories_array;

typedef struct Player {
    char playerName[16];
    char planet[8];
    unsigned int gas;
    unsigned int minerals;
    unsigned int coins;

    int factory_owned[MAX_OWNED_FACTORIES];
    unsigned int len_owned;
} Player;

void add_factory(Factories_array* game_factories,char* type, char rarity, unsigned int gas_income, unsigned int minerals_income, unsigned int coin_income){
    Factory* new = malloc(sizeof(Factory));

    strcpy(new->type,type);
    new->owned = -1;
    new->rarity = rarity;
    new->gas_income = gas_income;
    new->minerals_income = minerals_income;
    new->coin_income = coin_income;
    new->upgrade = 1;

    game_factories->array[game_factories->len++] = new;
}

void init_game_factories(Factories_array* game_factories){
    for (size_t i = 0; i < 3; i++){
        add_factory(game_factories,"fact_c_mn_1",0,0,1,0);
    }
    for (size_t i = 0; i < 3; i++){
        add_factory(game_factories,"fact_c_mngd_1",0,0,1,1);
    }
    for (size_t i = 0; i < 2; i++){
        add_factory(game_factories,"fact_c_mngd_2",0,0,1,2);
    }
    for (size_t i = 0; i < 3; i++){
        add_factory(game_factories,"fact_e_mn_1",1,0,2,0);
    }
    for (size_t i = 0; i < 2; i++){
        add_factory(game_factories,"fact_e_mngd_1",1,0,2,1);
    }
    add_factory(game_factories,"fact_l_mngd_1",2,0,3,2);

    for (size_t i = 0; i < 3; i++){
        add_factory(game_factories,"fact_c_gs_1",0,1,0,0);
    }
    for (size_t i = 0; i < 3; i++){
        add_factory(game_factories,"fact_c_gsgd_1",0,1,0,1);
    }
    for (size_t i = 0; i < 2; i++){
        add_factory(game_factories,"fact_c_gsgd_2",0,1,0,2);
    }
    for (size_t i = 0; i < 3; i++){
        add_factory(game_factories,"fact_e_gs_1",1,2,0,0);
    }
    for (size_t i = 0; i < 2; i++){
        add_factory(game_factories,"fact_e_gsgd_1",1,2,0,1);
    }
    add_factory(game_factories,"fact_l_mngd_1",2,3,0,2);
}

void free_factories(Factories_array* game_factories){
    for (size_t i = 0; i < game_factories->len; i++){
        free(game_factories->array[i]);
    }
    free(game_factories->array);
    free(game_factories);
}

void free_players(Player** player_array,unsigned int nb_players){
    for (size_t i = 0; i < nb_players; i++){
        free(player_array[i]);
    }
    free(player_array);
}

void debug_factories(Factories_array* game_factories){
    for (size_t i = 0; i < NB_CARDS; i++){
        printf("Index : %lu\n",i);
        printf("Type : %s\n",game_factories->array[i]->type);
        printf("Owned : %u\n",game_factories->array[i]->owned);
        printf("Rarity : %u\n",game_factories->array[i]->rarity);
        printf("Gas income : %u\n",game_factories->array[i]->gas_income);
        printf("Minerals Income : %u\n",game_factories->array[i]->minerals_income);
        printf("Coin income : %u\n",game_factories->array[i]->coin_income);
        printf("Upgrade : %u\n",game_factories->array[i]->upgrade);

        printf("\n");
    }
}

void debug_player_array(Player** player_array,Factories_array* game_factories,unsigned int nb_players){
    for (size_t i = 0; i < nb_players; i++){
        printf("Index : %lu\n",i);
        printf("Name : %s",player_array[i]->playerName);
        printf("Planet Name : %s\n",player_array[i]->planet);
        printf("Gas : %u\n",player_array[i]->gas);
        printf("Minerals : %u\n",player_array[i]->minerals);
        printf("Coins : %u\n",player_array[i]->coins);
        printf("Owned Factories : %lu\n",player_array[i]->len_owned);
        for (size_t j = 0; j < player_array[i]->len_owned; j++){
            unsigned int index = player_array[i]->factory_owned[j];
            printf("    Index : %lu\n",index);
            printf("    Type : %s\n",game_factories->array[index]->type);
            printf("    Owned : %u\n",game_factories->array[index]->owned);
            printf("    Rarity : %u\n",game_factories->array[index]->rarity);
            printf("    Gas income : %u\n",game_factories->array[index]->gas_income);
            printf("    Minerals Income : %u\n",game_factories->array[index]->minerals_income);
            printf("    Coin income : %u\n",game_factories->array[index]->coin_income);
            printf("    Upgrade : %u\n",game_factories->array[index]->upgrade);
            printf("\n");
        }
        printf("\n");
    }
}

void set_factory_ownership(char current_player,Player** player_array,Factories_array* game_factories,unsigned int index_factory){
    if (player_array[current_player]->len_owned == MAX_OWNED_FACTORIES){
        printf("You already have the maximum number of factories you can own !\nYou can sell one of your factories to buy another one !\n");
        return;    
    }
    if (game_factories->array[index_factory]->owned > -1){
        printf("The factory you selected is already owned by player %d.\n",game_factories->array[index_factory]->owned + 1);
        return; 
    }

    game_factories->array[index_factory]->owned = current_player;
    
    for (size_t i = 0; i < MAX_OWNED_FACTORIES; i++){
        if (player_array[current_player]->factory_owned[i] == -1){
            player_array[current_player]->factory_owned[i] = index_factory;
            player_array[current_player]->len_owned++;
            break;
        }
    }

    printf("You now have in your possession factory %d\n",index_factory);
    return;
}

void unset_factory_ownership(char current_player,Player** player_array,Factories_array* game_factories,unsigned int index_factory){
    if (player_array[current_player]->len_owned <= 0){
        printf("You do not owned any factories !\n");
        return;
    }
    if (current_player != game_factories->array[index_factory]->owned){
        if (game_factories->array[index_factory]->owned != -1)
            printf("You cannot unowned a factory that is owned by another player ! Fucking Cheater !\n");
        else{
            printf("You cannot unowned a factory that is not owned by anyone !\n");
        }
        return;
    }

    for (size_t i = 0; i < MAX_OWNED_FACTORIES; i++){
        if (player_array[current_player]->factory_owned[i] == index_factory){
            game_factories->array[index_factory]->owned = -1;
            game_factories->array[index_factory]->upgrade = 1;
            player_array[current_player]->factory_owned[i] = -1;
            player_array[current_player]->len_owned--;
            printf("You do not anymore factory %d\n",index_factory);
            return;
        }
    }
    printf("You do not own factory %d\n",index_factory);
    return;
}

void set_coin(char current_player,Player** player_array,unsigned int amount){
    if (amount < 0){
        printf("How did you break my code lil hoe ?\n");
        return;
    }
    player_array[current_player]->coins = amount;
    printf("You now have %lu coin(s) !\n");

    return;
}

int main(int argc, char const *argv[]){
    unsigned int nb_players = 2;

    printf("\e[1;1H\e[2J");
    printf("Welcome to Orbital Heat !\n");
    printf("How many players do you want ? (min. 2 - max 6)\n");
    
    scanf("%u",&nb_players);
    getchar();
    printf("You choosed : %u \n",nb_players);

    if (nb_players < 2 || nb_players > 6){
        printf("Please input a correct number of players !");
        return 1;
    }
    Player** player_array = malloc(nb_players * sizeof(Player));
    for (size_t i = 0; i < nb_players; i++){
        player_array[i] = malloc(sizeof(Player));
        printf("\e[1;1H\e[2J");
        printf("Player %lu - Please input your name : (max 16 caracters)\n",i+1);
        fgets(player_array[i]->playerName,15,stdin);
        strcpy(player_array[i]->planet,planet_array[i]);
        player_array[i]->coins = 6;
        player_array[i]->gas = 0;
        player_array[i]->minerals = 0;
        memset(player_array[i]->factory_owned,-1,MAX_OWNED_FACTORIES * sizeof(int));
        player_array[i]->len_owned = 0;
    }
    printf("\e[1;1H\e[2J");

    Factories_array* game_factories = malloc(sizeof(Factories_array));
    game_factories->array = malloc(NB_CARDS * sizeof(Factory));
    game_factories->len = 0;
    init_game_factories(game_factories);

    char currentPlayer = 0;
    unsigned int turn = 0;

    set_factory_ownership(currentPlayer,player_array,game_factories,6);
    debug_player_array(player_array,game_factories,nb_players);

    set_factory_ownership(currentPlayer,player_array,game_factories,7);
    debug_player_array(player_array,game_factories,nb_players);

    unset_factory_ownership(currentPlayer,player_array,game_factories,7);
    debug_player_array(player_array,game_factories,nb_players);

    unset_factory_ownership(currentPlayer,player_array,game_factories,6);
    debug_player_array(player_array,game_factories,nb_players);

    unset_factory_ownership(currentPlayer,player_array,game_factories,6);
    debug_player_array(player_array,game_factories,nb_players);

    free_factories(game_factories);
    free_players(player_array,nb_players);
    return 0;
}
