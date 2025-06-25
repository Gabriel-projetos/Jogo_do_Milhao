#ifndef LEADERSYSTEM_H
    #define LEADERSYSTEM_H

    #define LEADER_SIZE 10
    #define MAX_LENGTH 16

    typedef struct{
        char name[MAX_LENGTH];
        int score;
    } Ranking;



    //------------------------------------------------------------------------------------------
    // Leader methods
    //------------------------------------------------------------------------------------------
    Ranking* readRanking();


    void writeRanking(Ranking *rankingboard);

    int addRanking(Ranking* rankingboard, char* name, int score);

    void printRanking(Ranking *rankingboard);

    int minRanking(Ranking *rankingboard);

#endif