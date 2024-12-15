#include <stdlib.h>
#include "aiagent.h"
#include "board.h"
#include "agent.h"
#include "Dict.h"
#include <stdio.h>

Move aiPlay(Agent *agent, Board b);
void aiFreedata(void *data);
void freeValues(void *value);
void noEnd(Agent *agent, Board b, Player winner);
int GetMinimaxScore(Agent *agent, Board b);

typedef struct Node_T{
    Move bestMove;
    int minimaxScore;
} Node;

Agent *createAiAgent(void) 
{
    Dict *hashTable = dictCreate(12000); //Création d'une table de hashage de taille 12000 (Recommandation sur Ecampus)

    Agent *aiAgent = agentCreate("Minimax Guy", aiPlay , noEnd, aiFreedata);

    agentSetData(aiAgent, hashTable); // Ajout de la Hashtable à l'agent à l'emplacement data

    return aiAgent;
}

int GetMinimaxScore(Agent *agent, Board b) //Fonction renvoyant la valeur de S(b)
{
    Dict *dict = (Dict *)agentGetData(agent);

    if(dictContains(dict, b)){
        return *(int *)dictSearch(dict, b);
    }

    int minimaxScore = -1;
    Move bestMove = 0;
    for(Move i = 0; i < 9; i++){
        if(boardValidMove(b, i)){
            if(boardIsFull(b) && boardWin(b) == 'E'){ //Cas où le board est full + draw

                if(minimaxScore < 0){
                    minimaxScore = 0;
                }

            } else if(boardWin(b) != 'E'){ //Cas où la game est lose

                //On fait rien parce que minimaxScore est déjà égal à -1 dans le pire des cas

            } else { //Cas où la game est pas finie
                int temp = -GetMinimaxScore(agent, boardNext(b, i, boardGetPlayer(b))); //Formule de Minimax
                if(temp > minimaxScore){
                    minimaxScore = temp;
                    bestMove = i;
                }
            }
        }
    }

    for (Move i = 0; i < 9; i++){
        if (boardValidMove(b, i)){
            bestMove = i;
            break;
        }
    }

    Node *node = malloc(sizeof(Node));
    if(!node)
        return -2;
    node->minimaxScore = minimaxScore;
    node->bestMove = bestMove;
    dictInsert(agentGetData(agent), b, node);
    return minimaxScore;
}

Move aiPlay(Agent *agent, Board b){
    Dict *dict = agentGetData(agent);
    if(dictContains(dict, b)){
        Node *node = dictSearch(dict, b);
        return node->bestMove;
    }
    GetMinimaxScore(agent, b);
    Node *node = dictSearch(dict, b);
    return node->bestMove;
}

void aiFreedata(void *dict){
    dictFreeValues((Dict *)dict, freeValues);
    return;
}

void freeValues(void *node){
    free(node);
    return;
}

void noEnd(Agent *agent, Board b, Player winner)
{
    (void)agent;
    (void)b;
    (void)winner;
    return;
}