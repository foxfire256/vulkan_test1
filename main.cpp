#include "gfx.hpp"
gfx *g;

int main(int argc, char **agrv)
{
	g = new gfx();

	g->init();

	while(!g->done())
	{
		g->render();
	}

	g->deinit();
	
	return 0;
}
