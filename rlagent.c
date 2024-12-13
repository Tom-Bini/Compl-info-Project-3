#include <stdlib.h>
#include "rlagent.h"
#include "board.h"
#include "agent.h"
#include "Dict.h"
#include "LinkedList.h"

Agent *createRlAgent(void) {
    return NULL;
}

void setTrainingModeRlAgent(Agent *agent, bool training) { //Training = True -> mode apprentissage, Training = False -> mode exploitation
    return;
}


float getMoveScoreRlAgent(Agent *agent, Board b, Move m) { //Renvoie le score moyen du move en fonction des données déjà accumulées
    return 0.0;
}


