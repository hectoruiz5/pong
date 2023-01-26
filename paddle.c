/*PONG by Hector Ruiz
        ____  ____   _        ____
        \___\ \   \  \ \   \  \   __
         \     \   \  \  \  \  \  \ \
          \     \___\  \   \_\  \____\
This file has the definitions of the functions to init a paddle, update it, and render it*/
#include <allegro5/allegro_primitives.h>

#include "paddle.h"
#include "settings.h"

void init_paddle(struct Paddle* paddle, float x, float y, float w, float h)
{
    paddle->x = x;
    paddle->y = y;
    paddle->width = w;
    paddle->height = h;
    paddle->vy = 0;
}

void update_paddle(struct Paddle* paddle, float dt)
{
    //float p0 = paddle->y;
    //float p1 = paddle->vy * dt +p0;
    //paddle->y = p1;
    paddle->y += paddle->vy * dt;
    
    paddle->y = MAX(0, MIN(paddle->y, TABLE_HEIGHT - PADDLE_HEIGHT)); //restringimos los limites de la paleta
}

void render_paddle(struct Paddle paddle)
{
    al_draw_filled_rectangle(
        paddle.x, paddle.y, paddle.x + paddle.width, paddle.y + paddle.height,
        al_map_rgb(255, 255, 255)
    );
}

void build_paddle_hitbox(struct Paddle paddle, struct Hitbox* hitbox){
    hitbox->x1 = paddle.x;
    hitbox->y1 = paddle.y;
    hitbox->x2 = paddle.x + paddle.width;
    hitbox->y2 = paddle.y + paddle.height;
}