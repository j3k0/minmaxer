#include "minmaxer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mmInit(MinMaxer *mm, unsigned int sizeOfState, unsigned int sizeOfMove) {
    mm->evaluator = (MinMaxerEvaluator)0;
    mm->generator = (MinMaxerGenerator)0;
    mm->play= (MinMaxerPlay)0;
    mm->sizeOfState = sizeOfState;
    mm->sizeOfMove = sizeOfMove;
    mm->movesCapacity = 32;
    mm->statesCapacity = 32;
    mm->movesUsed = 0;
    mm->statesUsed = 0;
    mm->states = malloc(mm->sizeOfState * mm->statesCapacity);
    mm->moves = malloc(mm->sizeOfMove * mm->movesCapacity);
}

void mmSetEvaluator(MinMaxer *mm, MinMaxerEvaluator evaluator) {
    mm->evaluator = evaluator;
}

void mmSetGenerator(MinMaxer *mm, MinMaxerGenerator generator) {
    mm->generator = generator;
}

void mmSetPlay(MinMaxer *mm, MinMaxerPlay play) {
    mm->play = play;
}

void *mmStatePush(MinMaxer *mm) {
    unsigned int statesUsed = mm->statesUsed;
    mm->statesUsed ++;
    if (mm->statesUsed > mm->statesCapacity) {
        mm->statesCapacity *= 2;
        mm->states = realloc(mm->states, mm->statesCapacity * mm->sizeOfState);
    }
    return ((char*)mm->states + mm->sizeOfState * statesUsed);
}

void mmStatePop(MinMaxer *mm) {
    mm->statesUsed--;
}

void *mmMovePush(MinMaxer *mm) {
    unsigned int movesUsed = mm->movesUsed;
    mm->movesUsed ++;
    if (mm->movesUsed > mm->movesCapacity) {
        mm->movesCapacity *= 2;
        mm->moves = realloc(mm->moves, mm->movesCapacity * mm->sizeOfMove);
        printf("new capacity: %d\n", mm->movesCapacity);
    }
    return ((char*)mm->moves + mm->sizeOfMove * movesUsed);
}

void mmMovePop(MinMaxer *mm) {
    mm->movesUsed--;
}

static void *mmMove(MinMaxer *mm, unsigned int index) {
    return ((char*)mm->moves + mm->sizeOfMove * index);
}

int mmBest(MinMaxer *mm, void *state, void *move, unsigned int maxDepth) {
    unsigned int movesAvailable = mm->movesCapacity - mm->movesUsed;
    unsigned int movesBaseIndex = mm->movesUsed;
    unsigned int nMoves = mm->generator(state, (char*)mm->moves + mm->sizeOfMove * movesBaseIndex, movesAvailable);
    if (nMoves == 0) {
        return mm->evaluator(state);
    }
    else if (nMoves <= movesAvailable) {
        mm->movesUsed += nMoves;
        void *nextState = mmStatePush(mm);
        int maxEval = -999999;
        int maxEvalIndex = -1;
        if (maxDepth > 1) {
            for (unsigned int i = 0; i < nMoves; ++i) {
                mm->play(state, mmMove(mm, movesBaseIndex + i), nextState);
                void *move = mmMovePush(mm);
                int eval = -mmBest(mm, nextState, move, maxDepth - 1);
                if (eval > maxEval) {
                    maxEval = eval;
                    maxEvalIndex = movesBaseIndex + i;
                }
                mmMovePop(mm);
            }
            if (maxEvalIndex >= 0)
                memcpy(move, mmMove(mm, maxEvalIndex), mm->sizeOfMove);
        }
        else {
            for (unsigned int i = 0; i < nMoves; ++i) {
                mm->play(state, mmMove(mm, movesBaseIndex + i), nextState);
                int eval = -mm->evaluator(nextState);
                if (eval > maxEval) {
                    maxEval = eval;
                    maxEvalIndex = movesBaseIndex + i;
                }
            }
            if (maxEvalIndex >= 0)
                memcpy(move, mmMove(mm, maxEvalIndex), mm->sizeOfMove);
        }
        mmStatePop(mm);
        mm->movesUsed -= nMoves;
        return maxEval;
    }
    else {
        while (mm->movesCapacity <= mm->movesUsed + nMoves)
            mm->movesCapacity *= 2;
        mm->moves = realloc(mm->moves, mm->movesCapacity * mm->sizeOfMove);
        printf("new capacity: %d\n", mm->movesCapacity);
        return mmBest(mm, state, move, maxDepth);
    }
}
