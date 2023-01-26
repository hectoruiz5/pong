/*PONG by Hector Ruiz
		____  ____   _        ____
		\___\ \   \  \ \   \  \   __
		 \     \   \  \  \  \  \  \ \
		  \     \___\  \   \_\  \____\
This file has the variables to render text*/
#pragma once

#include <allegro5/allegro_ttf.h>

struct Fonts
{
	ALLEGRO_FONT* large_font;
	ALLEGRO_FONT* score_font;
};

void create_fonts(struct Fonts* fonts);

void destroy_fonts(struct Fonts* fonts);