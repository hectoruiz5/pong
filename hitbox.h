/*PONG by Hector Ruiz
		____  ____   _        ____
		\___\ \   \  \ \   \  \   __
		 \     \   \  \  \  \  \  \ \
		  \     \___\  \   \_\  \____\
This file has the definition of the hitbox and the function to determine wheter ywo hitboxes collide*/

#pragma once

struct Hitbox{
	float x1;
	float y1;
	float x2;
	float y2;
};

int collides(struct Hitbox hitbox1, struct Hitbox hitbox2);