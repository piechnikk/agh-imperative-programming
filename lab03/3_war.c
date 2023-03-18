// card game called war
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define CARDS 52

int players[2][CARDS];                // players' hands
int out[2] = {0, 0}, len[2] = {0, 0}; // variables for cyclic queue
int conflict_counter;

int table[2][CARDS]; // table stack
int top[2] = {0, 0};

int rand_from_interval(int a, int b)
{
    unsigned ba = (b - a);
    if (a > b)
    {
        return INT_MIN;
    }
    else if (ba > RAND_MAX)
    {
        return INT_MAX;
    }
    else if (a == b)
    {
        return a;
    }
    else
    {
        return (rand() % (b - a + 1)) + a;
    }
}

void rand_permutation(int n, int tab[])
{
    if (n >= 0)
    {
        for (int i = 0; i < n; i++)
        {
            tab[i] = i;
        }
        for (int i = 0; i < n - 1; i++)
        {
            int k = rand_from_interval(i, n - 1);
            int helper = tab[i];
            tab[i] = tab[k];
            tab[k] = helper;
        }
    }
}

// push card to player's hand
double push_card(int card_id, int player_id)
{
    if (len[player_id] + 1 > CARDS)
    {
        return INFINITY;
    }
    else
    {
        int next_id = (out[player_id] + len[player_id]) % CARDS;
        players[player_id][next_id] = card_id;
        len[player_id]++;
        return 0;
    }
}

// take card from player's hand
int pop_card(int player_id)
{
    if (len[player_id] <= 0)
    {
        return -1;
    }
    else
    {
        int old = players[player_id][out[player_id]];
        out[player_id] = (out[player_id] + 1) % CARDS;
        len[player_id]--;
        return old;
    }
}

// number of cards in player's hand
int cards_count(int player_id)
{
    return len[player_id];
}

// display cards in player's hand
void print_cards(int player_id)
{
    for (int i = 0; i < len[player_id]; i++)
    {
        printf("%i ", players[player_id][(out[player_id] + i) % CARDS]);
    }
}

// shuffle cards and distribute them to players
void shuffle_cards()
{
    for (int i = 0; i < CARDS; i++)
    {
        push_card(i, 1);
    }
    for (int i = 0; i < CARDS - 1; i++)
    {
        int k = rand_from_interval(i, CARDS - 1);
        int helper = players[1][i];
        players[1][i] = players[1][k];
        players[1][k] = helper;
    }

    for (int i = 0; i < CARDS / 2; i++)
    {
        push_card(pop_card(1), 0);
    }
}

// put the card on the table
double table_push(int card, int player_id)
{
    table[player_id][top[player_id]] = card;
    top[player_id] += 1;
}

// take the card from the table
double table_pop(int winner_id)
{
    int looser_id = (winner_id + 1) % 2;
    for (int i = 0; i < top[winner_id]; i++)
    {
        push_card(table[winner_id][i], winner_id);
    }
    top[winner_id] = 0;
    for (int i = 0; i < top[looser_id]; i++)
    {
        push_card(table[looser_id][i], winner_id);
    }
    top[looser_id] = 0;
}

// easier version of the game without wars
int easier_game(int conflict_count)
{
    for (conflict_counter = 1; conflict_counter <= conflict_count; conflict_counter++)
    {
        int p0 = pop_card(0);
        int p1 = pop_card(1);
        int v0 = p0 / 4;
        int v1 = p1 / 4;
        if (v0 == v1)
        {
            push_card(p0, 0);
            push_card(p1, 1);
        }
        else if (v0 > v1)
        {
            push_card(p0, 0);
            push_card(p1, 0);
        }
        else if (v0 < v1)
        {
            push_card(p1, 1);
            push_card(p0, 1);
        }
        if (cards_count(1) == 0)
        {
            return 2;
        }
        else if (cards_count(0) == 0)
        {
            return 3;
        }
    }
    return 0;
}

// return the last card placed on the table
int top_card_on_table(int player_id)
{
    return table[player_id][top[player_id] - 1];
}

int number_of_cards_on_table()
{
    return top[0] + top[1];
}

// return last cards to players' hands
void return_top_cards()
{
    top[0]--;
    top[1]--;
    push_card(table[0][top[0]], 0);
    push_card(table[1][top[1]], 1);
}

int normal_game(int conflict_count)
{
    for (conflict_counter = 1; conflict_counter <= conflict_count; conflict_counter++)
    {
        table_push(pop_card(0), 0);
        table_push(pop_card(1), 1);
        int v0 = top_card_on_table(0) / 4;
        int v1 = top_card_on_table(1) / 4;
        if (v0 == v1)
        {
            if (cards_count(1) > 1 && cards_count(0) > 1)
            {
                table_push(pop_card(0), 0);
                table_push(pop_card(1), 1);
            }
            else
            {
                return_top_cards();
                return 1;
            }
        }
        else if (v0 > v1)
        {
            table_pop(0);
        }
        else if (v0 < v1)
        {
            table_pop(1);
        }
        if (cards_count(1) == 0 && number_of_cards_on_table() == 0)
        {
            return 2;
        }
        else if (cards_count(0) == 0 && number_of_cards_on_table() == 0)
        {
            return 3;
        }
    }
    return 0;
}

int main(void)
{
    int seed, version, conflict_count;
    scanf("%i %i %i", &seed, &version, &conflict_count);
    srand(seed);
    shuffle_cards();
    int result;
    if (version == 0)
    {
        result = normal_game(conflict_count);
    }
    if (version == 1)
    {
        result = easier_game(conflict_count);
    }
    printf("%i ", result);
    if (result == 0 || result == 1)
    {
        printf("%i %i", cards_count(0), cards_count(1));
    }
    else if (result == 2)
    {
        printf("%i", conflict_counter);
    }
    else if (result == 3)
    {
        print_cards(1);
    }

    return 0;
}