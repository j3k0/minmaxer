#include "minmaxer.h"
#include <stdio.h>

// IA for a guessing game

typedef struct {
    int player;
    int score[2];
    int nPlayLeft;
} State;

typedef struct {
    int guess;
} Move;

static int play(void *pInState, void *pMove, void *pOutState) {
    Move *move = (Move*)pMove;
    State *inState = (State*)pInState;
    State *outState = (State*)pOutState;
    //printf("player %d move %d\n", inState->player, move->guess);
    *outState = *inState;
    outState->score[outState->player] += move->guess;
    if (outState->score[outState->player] == outState->score[1-outState->player])
        outState->score[1 - outState->player] -= move->guess;
    outState->player = 1 - inState->player;
    outState->nPlayLeft = inState->nPlayLeft - 1;
    return -1;
}

static unsigned int generator(void *pState, void *pMoves, unsigned int size) {
    State *state = (State*)pState;
    Move *moves = (Move*)pMoves;
    if (state->nPlayLeft == 0)
        return 0;
    if (size >= 2) {
        moves[0].guess = 1;
        moves[1].guess = 2;
    }
    else {
        printf("not enough space\n");
    }
    return 2;
}

static int evaluator(void *pState) {
    State *state = (State*)pState;
    int eval = state->score[state->player] - state->score[1 - state->player];
    //printf("evaluate state for %d is %d\n", state->player, eval);
    return eval;
}

int main() {

    MinMaxer mm;
    State state;
    state.player = 0;
    state.score[0] = state.score[1] = 0;
    state.nPlayLeft = 20;
    Move move;
    move.guess = -1;
    mmInit(&mm, sizeof(State), sizeof(Move));
    mmSetGenerator(&mm, generator);
    mmSetEvaluator(&mm, evaluator);
    mmSetPlay(&mm, play);
    mmBest(&mm, &state, &move, 24);

    printf("Best move is guess: %d\n", move.guess);

    return 0;
}
