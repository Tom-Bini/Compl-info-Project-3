#include <stdlib.h>
#include "rlagent.h"
#include "board.h"
#include "agent.h"
#include "Dict.h"
#include "LinkedList.h"

typedef struct DataNode_T{
    bool possibleMoves[9];
    float averageScore[9];
    int NumberOfTimePlayed[9];
} DataNode;

typedef struct AgentData_T{
    Dict *hashTable;
    List *playedMoves;
    bool isTraining;
} AgentData;

Move rlPlay(Agent *agent, Board b);
void rlFreedata(void *dict);
void freeValues(void *node);
void rlEnd(Agent *agent, Board b, Player winner);

Agent *createRlAgent(void) {
    AgentData *data = malloc(sizeof(AgentData));

    Dict *hashTable = dictCreate(12000); //Création d'une table de hashage de taille 12000 (Recommandation sur Ecampus)

    List *playedMoves = llCreateEmpty();

    bool isTraining = true;

    data->hashTable = hashTable;
    data->playedMoves = playedMoves;
    data->isTraining = isTraining;

    Agent *rlAgent = agentCreate("Learning Guy", rlPlay , rlEnd, rlFreedata);

    agentSetData(rlAgent, data); // Ajout de la Hashtable à l'agent à l'emplacement data

    return rlAgent;
}

void setTrainingModeRlAgent(Agent *agent, bool training) { //Training = True -> mode apprentissage, Training = False -> mode exploitation
    AgentData *data = agentGetData(agent);
    data->isTraining = training;
}


float getMoveScoreRlAgent(Agent *agent, Board b, Move m) { //Renvoie le score moyen du move en fonction des données déjà accumulées
    AgentData *data = (AgentData *)agentGetData(agent);

    Dict *hashTable = data->hashTable;

    Player p = agentGetPlayer(agent);

    if(boardValidMove(b, m) && dictContains(hashTable, b)){
        DataNode *datanode = dictSearch(hashTable, b);

        return datanode->averageScore[m];
    } else {
        exit(1);
    }
}

Move rlPlay(Agent *agent, Board b){
    AgentData *data = (AgentData *)agentGetData(agent);

    Dict *hashTable = data->hashTable;
    List *playedMoves = data->playedMoves;
    bool isTraining = data->isTraining;
    Move chosenMove = 0;

    llInsertFirst(playedMoves, chosenMove);
    return choseMove;
}

void rlFreedata(void *dict){
    dictFreeValues((Dict *)dict, freeValues);
    return;
}

void rlEnd(Agent *agent, Board b, Player winner)
{
    AgentData *data = (AgentData *)agentGetData(agent);
    Player player = agentGetPlayer(agent);

    Dict *hashTable = data->hashTable;
    List *playedMoves = data->playedMoves;
    bool isTraining = data->isTraining;

    Node *currentNode = malloc(sizeof(Node));
    Move currentMove;

    while(llLength(playedMoves) != 0){
        currentNode = llPopFirst(playedMoves);
        currentMove = currentNode->data;
        if(dictContains(hashTable, b)){
            DataNode *datanode = dictSearch(hashTable, b);
            datanode->NumberOfTimePlayed[currentMove] += 1;
            if(player == winner){
                datanode->averageScore[currentMove] = (datanode->NumberOfTimePlayed[currentMove] * datanode->averageScore[currentMove] + 1) / (datanode->NumberOfTimePlayed[currentMove] + 1);
            } else if(winner == E){
                datanode->averageScore[currentMove] = datanode->NumberOfTimePlayed[currentMove] * datanode->averageScore[currentMove] / (datanode->NumberOfTimePlayed[currentMove] + 1);
            } else {
                datanode->averageScore[currentMove] = (datanode->NumberOfTimePlayed[currentMove] * datanode->averageScore[currentMove] - 1) / (datanode->NumberOfTimePlayed[currentMove] + 1);
            }
        } else {
            exit(1);
        }
    }
    return;
}