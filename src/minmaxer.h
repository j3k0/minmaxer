#ifndef MINMAXER_H
#define MINMAXER_H

#ifdef __cplusplus
extern "C" {
#endif

// Give a evaluation of the state in current players' perspective
typedef int (*MinMaxerEvaluator)(void *state);

// Returns the number of generated moves.
// Note that this could be bigger than the size of the moves array given,
// in that case make sure you're not writing in memory!
// The minmaxer will increase the size of the array and get back to you with more space.
typedef unsigned int (*MinMaxerGenerator)(void *state, void *moves, unsigned int size);

// Play a move. Store the result in outState.
// return +1 if it's now the same player's turn
// return -1 if it's now the other player's turn
typedef int (*MinMaxerPlay)(void *inState, void *move, void *outState);

typedef struct {
    MinMaxerEvaluator evaluator;
    MinMaxerGenerator generator;
    MinMaxerPlay play;
    unsigned int sizeOfState;
    unsigned int sizeOfMove;

    void *moves;  // stack of generated moves
    unsigned int movesCapacity;
    unsigned int movesUsed;

    void *states; // stack of states
    unsigned int statesCapacity;
    unsigned int statesUsed;
} MinMaxer;

void mmInit(MinMaxer *mm, unsigned int sizeOfState, unsigned int sizeOfMove);
void mmSetEvaluator(MinMaxer *mm, MinMaxerEvaluator evaluator);
void mmSetGenerator(MinMaxer *mm, MinMaxerGenerator generator);
void mmSetPlay(MinMaxer *mm, MinMaxerPlay play);
int  mmBest(MinMaxer *mm, void *state, void *move, unsigned int maxDepth);

#ifdef __cplusplus
};
#endif

#endif
