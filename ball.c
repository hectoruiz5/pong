/*PONG by Hector Ruiz
        ____  ____   _        ____
        \___\ \   \  \ \   \  \   __
         \     \   \  \  \  \  \  \ \
          \     \___\  \   \_\  \____\
This file has the definitions of the functions to init the ball, update it, and render it*/

#include <allegro5/allegro_primitives.h>

#include "ball.h"

void init_ball(struct Ball* ball, float x, float y, float s)
{
    ball->x = x;
    ball->y = y;
    ball->width = s;
    ball->height = s;
    ball->vx = 0;
    ball->vy = 0;
}

void update_ball(struct Ball* ball, float dt)
{
    ball->x += ball->vx * dt;
    ball->y += ball->vy * dt;
}

void render_ball(struct Ball ball)
{
    al_draw_filled_rectangle(
        ball.x, ball.y, ball.x + ball.width, ball.y + ball.height, al_map_rgb(255, 255, 255)
    );
}

void build_ball_hitbox(struct Ball ball, struct Hitbox* hitbox) {
    hitbox->x1 = ball.x;
    hitbox->y1 = ball.y;
    hitbox->x2 = ball.x + ball.width;
    hitbox->y2 = ball.y + ball.height;
}