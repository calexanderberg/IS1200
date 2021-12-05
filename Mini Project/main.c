//
// Created by Dennis Hadzialic on 2021-02-20.
//
#include <pic32mx.h>
#include <stdint.h>
#include "font.h"
#include "display.h"
#include "main.h"
#include "inputs.h"


//Creates a player
void create_player(int x, int y, int w, int h, const uint8_t state) {

    int i, j;

    int yj = y;

    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            setPixel(x, y, state);
            y++;
        }
        y = yj;
        x++;
    }
}

//Moves ball in x & y direction
void move_ball(int x, int y) { //Fixar så att bollen rör sig mot spelare 2
    quicksleep(100000);
    ballx += x;
    bally += y;
    setPixel(ballx, bally, 1);
}

void ball_colision(int last_player) { //Ball colision for player 1
    if (ballx == 3) { //The x-position where player 1 is
        int p1 = 0;
        int yvalue = player1y;
        int yvalueball = bally;

        for (p1 = 0; p1 < 8; p1++) {
            if (yvalueball == (yvalue + p1)) {
                move_ball(1, 0);
                slut1 = 1;

            }
        }

    }
    if (ballx == 125) { //The x-position where player 2 is

        if (player2y < 16) {
            int p2 = 0;
            int yvalue2 = player2y;
            int yvalueball2 = bally;

            for (p2 = 0; p2 < 8; p2++) {
                if (yvalueball2 == (yvalue2 + p2)) {
                    move_ball(-1, 0);
                    slut1 = 0;
                    break;
                }
            }
        }
    }

    if (last_player == 1) {
        if (bally == 0) {
            move_ball(1, 1);
            collision_value = 1;

        }
        if (bally == 32) {
            move_ball(1, -1);
            collision_value = 2;
        }
    } else if (last_player == 2) {
        if (bally == 0) {
            move_ball(-1, 1);
            collision_value = 1;
        }
        if (bally == 32) {
            move_ball(-1, -1);
            collision_value = 2;
        }
    }

    // https://stackoverflow.com/questions/15753937/pong-paddle-collision-velocity-and-rebound-angle
}

void move_player(int player, int y) {
    if (player == 1) {
        if (player1y < 26 && player1y >= 0 && y == 1) {
            quicksleep(100000);
            player1y += y;
        } else if (player1y > 0 && player1y < 26 && y == -1) {
            quicksleep(100000);
            player1y += y;
        }
    } else if (player == 2) {
        if (player2y < 26 && player2y >= 0 && y == 1) {
            quicksleep(100000);
            player2y += y;
        } else if (player2y > 0 && player2y < 26 && y == -1) {
            quicksleep(100000);
            player2y += y;
        }
    }
}

void move_AI(int difficulty) {
    if (difficulty == 1) {
        quicksleep(1000);
        if (bally > player2y) {
            player2y++;
        } else {
            player2y--;
        }
    } else if (difficulty == 2) {
        if (bally > player2y) {
            player2y++;
        } else {
            player2y--;
        }
    } else if (difficulty == 3) {
        if (bally != 12 && slut1 == 0) {
            if (bally < 12) {
                player2y--;
            } else if (bally > 12) {
                player2y++;
            }
        } else {
            if (bally > player2y) {
                player2y++;
            } else {
                player2y--;
            }
        }
    }
}

int player_score_board(int score1, int score2) {
    int i,j;
    for (i = 0; i < score1; i++) {
        setPixel(pixeltestx + (i*2), pixeltesty, 1);
        setPixel(pixeltestx + (i*2), pixeltesty + 1, 1);
        setPixel(pixeltestx + (i*2), pixeltesty + 2, 1);
        setPixel(pixeltestx + (i*2), pixeltesty + 3, 1);

    }


    for (j = 0; j < score2; j++) {
        setPixel(pixelP2x + (j*2), pixelP2y, 1);
        setPixel(pixelP2x + (j*2), pixelP2y + 1, 1);
        setPixel(pixelP2x + (j*2), pixelP2y + 2, 1);
        setPixel(pixelP2x + (j*2), pixelP2y + 3, 1);

    }

}

int main(void) {
    while (1) {
        init_mcu(); //Initilize basic osv
        display_init(); //Initilize displayen
        clear();
        TRISD = 0x0fe0;
        int GAME = 0;
        int difficulty;

        while (GAME == 0) {
            int butten;
            butten = getbtns();

            display_string(0, "press 1");
            display_string(1, "for 1 player");
            display_string(2, "press 2");
            display_string(3, "for 2 player");
            display_update();

            if (butten == 2) { // Går direkt till spelet (multi)
                GAME = 1;
            }
            if (butten == 4) { //Kör singleplayer mode
                GAME = 4;
                gameMode = 1;
            }

        }
        while (GAME == 4) {
            int sw = getsw();

            display_string(0, "Choose difficulty");
            display_string(1, "");
            display_string(2, "Use Switches: ");
            display_string(3, "1-3");
            display_update();

            if (sw == 1) {
                difficulty = 1;
                GAME = 1;
            } else if (sw == 2) {
                difficulty = 2;
                GAME = 1;
            } else if (sw == 4) {
                difficulty = 3;
                GAME = 1;
            }
        }
        while (GAME == 1) { //Actual game running
            if (Player1Score == 3) { //Om player1 vinner, gå till player1 victoryscreen
                GAME = 2;
            }
            if (Player2Score == 3) { //Om player2 vinner, gå till player2 Victoryscreen
                GAME = 3;
            } else { // Else, kör spelet
                clear();
                button = getbtns();
                button1 = getbtn1();
                player_score_board(Player1Score, Player2Score); //checking to update score
                if (ballx == 0) { //Check for score for player 2
                    Player2Score++;
                    ballx = 64;
                    bally = 16;
                }
                if (ballx == 128) { //Check for score for player 1
                    Player1Score++;
                    ballx = 64;
                    bally = 16;
                }
                create_player(player1x, player1y, player1w, player1h, 1); //Skapar spelare 1
                create_player(player2x, player2y, player2w, player2h, 1); //skapar spelare 2
                setPixel(ballx, bally, 1); //Sätter ut bollen i mitten
                ball_colision(3); //Constantly check collision
                if (button == 4) {
                    move_player(1, -1);
                }
                if (button == 2) {
                    move_player(1, 1);
                }
                if (gameMode == 0) { //If its multiplayer mode
                    if (button1 == 1) {
                        move_player(2, 1);
                    }
                    if (button == 1) {
                        move_player(2, -1);
                    }
                } else if (gameMode == 1) { //If it's singlepalyer
                    move_AI(difficulty); //Calls for the AI constantly
                }
                if (slut1 == 1) { //If it's succesfully colided with player 1
                    score_player_1 += 100;
                    ball_colision(1); // Keepchecking collision
                    if (collision_value == 1) { // Checking if it's on the top part of screen
                        move_ball(1, 1);
                    }
                    if (collision_value == 2) { //Checking if it's on the bottom part of screen
                        move_ball(1, -1);
                    }
                }
                if (slut1 == 0) { //If it's succesfully colided with player 2
                    score_player_2 += 100;
                    ball_colision(2); //Keep checking collision
                    if (collision_value == 1) { // Checking if it's on the top part of screen
                        move_ball(-1, 1);
                    }
                    if (collision_value
                        == 2) { // Checking if it's on the bottom part of screen
                        move_ball(-1, -1);
                    }
                }
                display_image(0, bufferstate);
            }
        }
        while (GAME == 2) { //Player 1 victory screen
            button = getbtns();

            display_string(0, "Winner Player 2!");
            display_string(1, "");
            display_string(2, "");
            display_string(3, "Restart: 1");
            display_update();
            if (button == 4) {
                Player1Score = 0;
                Player2Score = 0;
                GAME = 0;
                gameMode = 0;
            }
        }
        while (GAME == 3) { //Player 2 victory screen
            button = getbtns();

            display_string(0, "Winner Player 2!");
            display_string(1, "");
            display_string(2, "");
            display_string(3, "Restart: 1");
            display_update();

            if (button == 4) {
                Player1Score = 0;
                Player2Score = 0;
                GAME = 0;
                gameMode = 0;
            }
        }

        //https://stackoverflow.com/questions/66710953/c-how-to-add-a-into-into-a-string-input?noredirect=1#comment117925953_66710953
    }
    return 0;
}
