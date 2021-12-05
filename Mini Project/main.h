//
// Created by Dennis Hadzialic on 2021-02-20.
//
#ifndef    MAIN_H
#define    MAIN_H

void create_player(int x, int y, int w, int h, const uint8_t state);
void move_ball(int x, int y);
void ball_colision();
void move_player(int player, int y);
void move_AI();
int player_score_board(int score1, int score2);


//Test   // E - kod för scoreboard
int pixeltestx = 32;
int pixeltesty = 2;

int pixelP2x = 94;
int pixelP2y = 2;

//scores   // E - bytt till player/aiscore
int Player1Score = 0;
int Player2Score = 0;

//player 1
int player1x = 0;
int player1y = 12; // 10
int player1w = 3;
int player1h = 8;

//player 2
int player2x = 125;
int player2y = 12;
int player2w = 3;
int player2h = 8;

//ball
int ballx = 64;
int bally = 16;


int slut1 = 0;
int slut2 = 0;
int gameMode = 0;
int collision_value = 1;
int score_player_1 = 0;
int score_player_2 = 0;

#endif /*MAIN_H*/