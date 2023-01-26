/*PONG by Hector Ruiz
		____  ____   _        ____
		\___\ \   \  \ \   \  \   __
		 \     \   \  \  \  \  \  \ \
		  \     \___\  \   \_\  \____\
This file has the functions
to render text*/

#include "settings.h"
#include "fonts.h"

void create_fonts(struct Fonts* fonts)
{
    fonts->large_font = al_load_ttf_font("fonts/font.ttf", 16, ALLEGRO_TTF_MONOCHROME);
    fonts->score_font = al_load_ttf_font("fonts/font.ttf", 32, ALLEGRO_TTF_MONOCHROME);
}

void destroy_fonts(struct Fonts* fonts)
{
    al_destroy_font(fonts->large_font);
    al_destroy_font(fonts->score_font);
}