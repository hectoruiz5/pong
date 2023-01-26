/*PONG by Hector Ruiz
		____  ____   _        ____
		\___\ \   \  \ \   \  \   __
		 \     \   \  \  \  \  \  \ \
		  \     \___\  \   \_\  \____\
This file has the definition of functions of the hitbox to determine wheter ywo hitboxes collide
										Algorithm AABB
*/

#include "hitbox.h"

int collides(struct Hitbox hitbox1, struct Hitbox hitbox2) {
	if (hitbox1.x1 > hitbox2.x2 || hitbox2.x1 > hitbox1.x2) {
		return 0;
	}
	if (hitbox1.y1 > hitbox2.y2 || hitbox2.y1 > hitbox1.y2) {
		return 0;
	}

	return 1;
}