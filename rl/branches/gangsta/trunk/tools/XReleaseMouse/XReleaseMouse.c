/* XReleaseMouse.c - Grabs the input and release it.
 * (C) 2004. Team Pantheon. www.team-pantheon.de
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Perl Artistic License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Perl Artistic License for more details.
 *
 *  You should have received a copy of the Perl Artistic License
 *  along with this program; if not you can get it here
 *  http://www.perldoc.com/perl5.6/Artistic.html.
 */
#include <SDL/SDL.h>

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_WM_GrabInput(SDL_GRAB_ON);
        SDL_WM_GrabInput(SDL_GRAB_OFF);
        SDL_Quit();
        return 0;
    }
    return 1;
}
