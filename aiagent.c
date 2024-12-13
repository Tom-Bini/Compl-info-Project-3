#include <stdlib.h>
#include "aiagent.h"
#include "board.h"
#include "agent.h"
#include "Dict.h"
#include "Dict.c"
#include "LinkedList.h"

Agent *createAiAgent(void) 
{
    Dict *hashTable = dictCreate(12000); //Création d'une table de hashage de taille 12000 (Recommandation sur Ecampus)

    
    return agentCreate("Minimax Guy", , noEnd((Agent *)NULL ,(Board)NULL , ),);
}

int GetMinimaxScore(Agent *agent, Board b) //Fonction renvoyant la valeur de S(b)
{
    int minimaxScore = -1;
    for(Move i = 0; i < 9; i++){
        if(boardValidMove(b, i)){
            if(dictContains(agent->data, (char *)(b))){
                return *(int *)dictSearch(agent->data, (char *)(b));
            } else {
                if(boardIsFull(b) && boardWin(b) == 'E'){ //Cas où le board est full + draw

                    if(minimaxScore < 0){
                        minimaxScore = 0;
                    }

                } else if(boardWin(b) != 'E'){ //Cas où la game est lose

                    //On fait rien parce que minimaxScore est déjà égal à -1 dans le pire des cas

                } else { //Cas où la game est pas finie
                    int temp = -GetMinimaxScore(agent, boardNext(b, i, agent)); //Formule de Minimax
                    if(temp > minimaxScore){
                        minimaxScore = temp;
                    }
                }
                
                dictInsert(agent->data, b, minimaxScore);
                return minimaxScore;
            }
        }
    }
}