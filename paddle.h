/*PONG by Hector Ruiz
        ____  ____   _        ____
        \___\ \   \  \ \   \  \   __
         \     \   \  \  \  \  \  \ \
          \     \___\  \   \_\  \____\
This file has the definitions of the functions to init a paddle, update it, and render it*/
#pragma once
#include "hitbox.h"

struct Paddle
{
    float x;
    float y;
    float width;
    float height;
    float vy;
};

void init_paddle(struct Paddle* paddle, float x, float y, float w, float h);

void update_paddle(struct Paddle* paddle, float dt);

void render_paddle(struct Paddle paddle);

void build_paddle_hitbox(struct Paddle paddle, struct Hitbox* hitbox);