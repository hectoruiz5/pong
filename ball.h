/*PONG by Hector Ruiz
        ____  ____   _        ____
        \___\ \   \  \ \   \  \   __
         \     \   \  \  \  \  \  \ \
          \     \___\  \   \_\  \____\
This file has the definitions of the functions to init the ball, update it, and render it*/
#pragma once
#include "hitbox.h"

struct Ball
{
    float x;
    float y;
    float width;
    float height;
    float vx;
    float vy;
};

void init_ball(struct Ball* ball, float x, float y, float s);

void update_ball(struct Ball* ball, float dt);

void render_ball(struct Ball ball);

void build_ball_hitbox(struct Ball ball, struct Hitbox* hitbox);