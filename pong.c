/*PONG by Hector Ruiz
		____  ____   _        ____
		\___\ \   \  \ \   \  \   __
		 \     \   \  \  \  \  \  \ \
		  \     \___\  \   \_\  \____\
This file has the definitions of the functions to init a pong game, update it, and render it*/

#include <stdio.h>

#include <allegro5/allegro_primitives.h>

#include "settings.h"
#include "fonts.h"
#include "pong.h"

void init_pong(struct Pong* pong, struct Sounds* sounds)
{
    init_paddle(&pong->player1, PADDLE_X_OFFSET, PADDLE_Y_OFFSET, PADDLE_WIDTH, PADDLE_HEIGHT);
    init_paddle(&pong->player2, TABLE_WIDTH - PADDLE_WIDTH - PADDLE_X_OFFSET, TABLE_HEIGHT - PADDLE_HEIGHT - PADDLE_Y_OFFSET, PADDLE_WIDTH, PADDLE_HEIGHT);
    init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
    pong->state = START;
    pong->player1_score = 0;
    pong->player2_score = 0;
    pong->serving_player = 0;
    pong->winning_player = 0;
    pong->num_players = 0;
    pong->sounds = sounds;
    srand(time(NULL));
}

void handle_input_pong(struct Pong* pong, ALLEGRO_KEYBOARD_STATE* state)
{
    if (pong->state == START)
    {
        if (al_key_down(state, ALLEGRO_KEY_ENTER))
        {
            pong->state = NUM_PLAYERS;
            if (pong->winning_player == 0) {
                pong->serving_player = rand() % 2 + 1;
            }
            init_paddle(&pong->player1, PADDLE_X_OFFSET, PADDLE_Y_OFFSET, PADDLE_WIDTH, PADDLE_HEIGHT);
            init_paddle(&pong->player2, TABLE_WIDTH - PADDLE_WIDTH - PADDLE_X_OFFSET, TABLE_HEIGHT - PADDLE_HEIGHT - PADDLE_Y_OFFSET, PADDLE_WIDTH, PADDLE_HEIGHT);
        }
    }
    else if (pong->state == NUM_PLAYERS) {
        if (al_key_down(state, ALLEGRO_KEY_W) || al_key_down(state, ALLEGRO_KEY_UP)) {
            pong->num_players--;
            if (pong->num_players < 0)
                pong->num_players = 3;
        }
        else if (al_key_down(state, ALLEGRO_KEY_S) || al_key_down(state, ALLEGRO_KEY_DOWN)) {
            pong->num_players++;
            if (pong->num_players > 3)
                pong->num_players = 0;
        }
        if (al_key_down(state, ALLEGRO_KEY_ENTER))
            pong->state = SERVE;
    }
    else if (pong->state == SERVE)
    {
        if (al_key_down(state, ALLEGRO_KEY_ENTER))
        {
            pong->state = PLAY;

            pong->ball.vx = rand() % 60 + 140;

            if (pong->serving_player == 2)
            {
                pong->ball.vx *= -1;
            }

            pong->ball.vy = rand() % 100 - 50;
        }
    }
    else if (pong->state == PLAY)
    {
        //Control player 1
        if (al_key_down(state, ALLEGRO_KEY_S) && (pong->num_players == 1 || pong->num_players == 3))
        {
            pong->player1.vy = PADDLE_SPEED;
        }
        else if (al_key_down(state, ALLEGRO_KEY_W) && (pong->num_players == 1 || pong->num_players == 3))
        {
            pong->player1.vy = -PADDLE_SPEED;
        }
        else
        {
            pong->player1.vy = 0;
        }

        //Control player 2
        if (al_key_down(state, ALLEGRO_KEY_DOWN) && (pong->num_players == 2 || pong->num_players == 3))
        {
            pong->player2.vy = PADDLE_SPEED;
        }
        else if (al_key_down(state, ALLEGRO_KEY_UP) && (pong->num_players == 2 || pong->num_players == 3))
        {
            pong->player2.vy = -PADDLE_SPEED;
        }
        else
        {
            pong->player2.vy = 0;
        }
    }
    else
    {
        if (al_key_down(state, ALLEGRO_KEY_ENTER))
        {
            pong->state = START;
            init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);

            pong->player1_score = 0;
            pong->player2_score = 0;

            if (pong->winning_player == 1)
            {
                pong->serving_player = 2;
            }
            else
            {
                pong->serving_player = 1;
            }
        }
    }
}

void update_pong(struct Pong* pong, double dt)
{
    if (pong->state == PLAY)
    {
        update_paddle(&pong->player1, dt);
        update_paddle(&pong->player2, dt);
        update_ball(&pong->ball, dt);

        struct Hitbox ball_hitbox;
        build_ball_hitbox(pong->ball, &ball_hitbox);

        struct Hitbox player1_hitbox;
        build_paddle_hitbox(pong->player1, &player1_hitbox);

        struct Hitbox player2_hitbox;
        build_paddle_hitbox(pong->player2, &player2_hitbox);

        ////////////////////////////////////////////////////////////INICIO Seccion de la IA////////////////////////////////////////////////////////////
        //Control PC 1
        if ((pong->num_players == 0 || pong->num_players == 2) && pong->ball.vx < 0) {
            if (player1_hitbox.y1 > ball_hitbox.y1) {
                pong->player1.vy = -PADDLE_SPEED;
            }
            else if (player1_hitbox.y2 < ball_hitbox.y2) {
                pong->player1.vy = PADDLE_SPEED;
            }
            else {
                pong->player1.vy = 0;
            }
        }

        //Control PC 2
        if ((pong->num_players == 0 || pong->num_players == 1) && pong->ball.vx > 0) {
            if (player2_hitbox.y1 > ball_hitbox.y1) {
                pong->player2.vy = -PADDLE_SPEED;
            }
            else if (player2_hitbox.y2 < ball_hitbox.y2) {
                pong->player2.vy = PADDLE_SPEED;
            }
            else {
                pong->player2.vy = 0;
            }
        }
        ////////////////////////////////////////////////////////////FIN Seccion de la IA////////////////////////////////////////////////////////////

        if (ball_hitbox.x1 > TABLE_WIDTH)
        {
            al_play_sample(pong->sounds->score, /* gain */ 1.0, /* center */ 1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            ++pong->player1_score;
            pong->serving_player = 2;

            if (pong->player1_score == MAX_POINTS)
            {
                pong->winning_player = 1;
                pong->state = DONE;
            }
            else
            {
                pong->state = SERVE;
                init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
            }
        }
        else if (ball_hitbox.x2 < 0)
        {
            al_play_sample(pong->sounds->score, /* gain */ 1.0, /* center */ -1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            ++pong->player2_score;
            pong->serving_player = 1;

            if (pong->player2_score == MAX_POINTS)
            {
                pong->winning_player = 2;
                pong->state = DONE;
                
            }
            else
            {
                pong->state = SERVE;
                init_ball(&pong->ball, TABLE_WIDTH / 2 - BALL_SIZE / 2, TABLE_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE);
            }
        }

        if (ball_hitbox.y1 <= 0)
        {
            al_play_sample(pong->sounds->wall_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.y = 0;
            pong->ball.vy *= -1;
        }
        else if (ball_hitbox.y2 >= TABLE_HEIGHT)
        {
            al_play_sample(pong->sounds->wall_hit, /* gain */ 1.0, /* center */ 0.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.y = TABLE_HEIGHT - pong->ball.height;
            pong->ball.vy *= -1;
        }

        if (collides(ball_hitbox, player1_hitbox))
        {
            al_play_sample(pong->sounds->paddle_hit, /* gain */ 1.0, /* center */ -1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.x = player1_hitbox.x2;
            pong->ball.vx *= -1.03;

            if (pong->ball.vy < 0)
            {
                pong->ball.vy = -(rand() % 140 + 10);
            }
            else
            {
                pong->ball.vy = rand() % 140 + 10;
            }
        }
        else if (collides(ball_hitbox, player2_hitbox))
        {
            al_play_sample(pong->sounds->paddle_hit, /* gain */ 1.0, /* center */ 1.0, /* speed */ 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            pong->ball.x = player2_hitbox.x1 - pong->ball.width;
            pong->ball.vx *= -1.03;

            if (pong->ball.vy < 0)
            {
                pong->ball.vy = -(rand() % 140 + 10);
            }
            else
            {
                pong->ball.vy = rand() % 140 + 10;
            }
        }
    }
}

void render_pong(struct Pong pong, struct Fonts fonts)
{
    if (pong.state == PLAY || pong.state == SERVE) {
        al_draw_filled_rectangle(
            TABLE_WIDTH / 2 - MID_LINE_WIDTH / 2, 0,
            TABLE_WIDTH / 2 + MID_LINE_WIDTH / 2, TABLE_HEIGHT,
            al_map_rgb(255, 255, 255)
        );
        render_paddle(pong.player1);
        render_paddle(pong.player2);
        render_ball(pong.ball);

        if (pong.num_players == 0 || pong.num_players == 2) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6 - 20, ALLEGRO_ALIGN_CENTER, "PC 1");
        }
        else if (pong.num_players == 1 || pong.num_players == 3) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6 - 20, ALLEGRO_ALIGN_CENTER, "Player 1");
        }
        
        if (pong.num_players == 0 || pong.num_players == 1) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6 - 20, ALLEGRO_ALIGN_CENTER, "PC 2");
        }
        if (pong.num_players == 2 || pong.num_players == 3) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6 - 20, ALLEGRO_ALIGN_CENTER, "Player 2");
        }

        //Esta parte del codigo tuve que modificarla y escribirla de esta manera porque no me estaba funcionando la funcion sprintf y no encontre forma de solucionarlo
        if (pong.player1_score == 0) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "0");
        }
        else if (pong.player1_score == 1) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "1");
        }
        else if (pong.player1_score == 2) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "2");
        }
        else if (pong.player1_score == 3) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "3");
        }
        else if (pong.player1_score == 4) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "4");
        }
        else if (pong.player1_score == 5) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "5");
        }

        if (pong.player2_score == 0) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "0");
        }
        else if (pong.player2_score == 1) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "1");
        }
        else if (pong.player2_score == 2) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "2");
        }
        else if (pong.player2_score == 3) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "3");
        }
        else if (pong.player2_score == 4) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "4");
        }
        else if (pong.player2_score == 5) {
            al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 + 50, TABLE_HEIGHT / 6, ALLEGRO_ALIGN_CENTER, "5");
        }

        
    }

    if (pong.state == START)
    {
        al_draw_text(fonts.score_font, al_map_rgb(100, 100, 100), TABLE_WIDTH / 2 - 2, 50 + 2, ALLEGRO_ALIGN_CENTER, "PONG");
        al_draw_text(fonts.score_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, 50, ALLEGRO_ALIGN_CENTER, "PONG");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press enter to start");
    }
    else if (pong.state == NUM_PLAYERS)
    {
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2 - 70, ALLEGRO_ALIGN_CENTER, "Enter number of players");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 30, TABLE_HEIGHT / 2 - 30, ALLEGRO_ALIGN_LEFT, "PC VS PC");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 30, TABLE_HEIGHT / 2 - 10, ALLEGRO_ALIGN_LEFT, "Player VS PC");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 30, TABLE_HEIGHT / 2 - 10, ALLEGRO_ALIGN_LEFT, "Player VS PC");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 30, TABLE_HEIGHT / 2 + 10, ALLEGRO_ALIGN_LEFT, "PC VS Player");
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 30, TABLE_HEIGHT / 2 + 30, ALLEGRO_ALIGN_LEFT, "Player VS Player");
        if (pong.num_players == 0) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 35, TABLE_HEIGHT / 2 - 30, ALLEGRO_ALIGN_RIGHT, "->");
        }
        else if (pong.num_players == 1) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 35, TABLE_HEIGHT / 2 - 10, ALLEGRO_ALIGN_RIGHT, "->");
        }
        else if (pong.num_players == 2) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 35, TABLE_HEIGHT / 2 + 10, ALLEGRO_ALIGN_RIGHT, "->");
        }
        else if (pong.num_players == 3) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2 - 35, TABLE_HEIGHT / 2 + 30, ALLEGRO_ALIGN_RIGHT, "->");
        }
    }
    else if (pong.state == SERVE)
    {
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press enter to serve");
    }
    else if (pong.state == DONE)
    {
        //char winner_message[14];
        //sprintf(winner_message, "Player %d won!", pong.winning_player);
        //no funciona el codigo anterior, me sale error de que la funcion es muy antigua
        if (pong.winning_player == 1) {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 3, ALLEGRO_ALIGN_CENTER, "Player 1 WON");
        }
        else {
            al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 3, ALLEGRO_ALIGN_CENTER, "Player 2 WON");
        }
        al_draw_text(fonts.large_font, al_map_rgb(255, 255, 255), TABLE_WIDTH / 2, TABLE_HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "Press enter to restart");
    }
}