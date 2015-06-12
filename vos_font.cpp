#include "vos_font.h"

vos_font::vos_font(const char *font_name, int size)
{
    font = TTF_OpenFont(font_name, size);
}

vos_font::~vos_font()
{
	TTF_CloseFont( font );
}

TTF_Font *vos_font::get_font()
{
	return font;
}
