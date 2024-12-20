#include <stdlib.h>
#include "rlagent.h"
#include "board.h"
#include "agent.h"
#include "Dict.h"
#include "LinkedList.h"

struct Node_t
{
    void *data;
    Node *next;
};

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
    srand();
    AgentData *data = (AgentData *)agentGetData(agent);

    Dict *hashTable = data->hashTable;
    List *playedMoves = data->playedMoves;
    bool isTraining = data->isTraining;
    Move chosenMove = 0;
    float bestScore = -1;
    int epsilon = rand() % 4;

    if(isTraining == true && epsilon == 0){ //Apprentissage aléatoire

        int nbValidMoves = 0;
        for(int i = 0; i < 9; i++){
            if(boardValidMove(b, i)){
                nbValidMoves++;
            }
        }
        int randomMove = rand() % nbValidMoves;
        for(int i = 0; i < 9; i++){
            if(boardValidMove(b, i)){
                randomMove--;
            }
            if(randomMove == 0){
                chosenMove = i;
                break;
            }
        }

    } else { //Exploitation ou apprentissage renforcé

        if(dictContains(hashTable, b)){
            DataNode *datanode = dictSearch(hashTable, b);
            for(int i = 0; i < 9; i++){
                if(datanode->averageScore[i] > bestScore){
                    bestScore = datanode->averageScore[i];
                    chosenMove = i;
                }
            }
        }
    }

    Move *node = malloc(sizeof(Move));
    *node = chosenMove;
    llInsertFirst(playedMoves, node);
    return chosenMove;
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

    if(isTraining == false){
        return;
    }

    Node *currentNode = malloc(sizeof(Node *));
    Move currentMove;

    while(llLength(playedMoves) != 0){
        currentNode = llPopFirst(playedMoves);
        currentMove = *(Move *)currentNode->data;
        DataNode *datanode = malloc(sizeof(DataNode));
        if(dictContains(hashTable, b)){
            datanode = dictSearch(hashTable, b);
            datanode->NumberOfTimePlayed[currentMove] += 1;
        } else {
            for(int i = 0; i < 9; i++){
                datanode->NumberOfTimePlayed[i] = 1;
                datanode->averageScore[i] = 0;
            }
        }
        if(player == winner){
            datanode->averageScore[currentMove] = (datanode->NumberOfTimePlayed[currentMove] * datanode->averageScore[currentMove] + 1) / (datanode->NumberOfTimePlayed[currentMove] + 1);
        } else if(winner == E){
            datanode->averageScore[currentMove] = datanode->NumberOfTimePlayed[currentMove] * datanode->averageScore[currentMove] / (datanode->NumberOfTimePlayed[currentMove] + 1);
        } else {
            datanode->averageScore[currentMove] = (datanode->NumberOfTimePlayed[currentMove] * datanode->averageScore[currentMove] - 1) / (datanode->NumberOfTimePlayed[currentMove] + 1);
        }
    }
    llFree(playedMoves);
}