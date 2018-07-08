#include "Core.h"
#include "Engine.h"

void main()
{
	Engine::instance().initialize();

	Engine::instance().gameLoop();
}
