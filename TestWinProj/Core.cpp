#include "pch.h"
#include "Core.h"
#include "Window.h"


void Core::Init(const WindowInfo& info)
{
	GWindow->Init(info);

}

void Core::Update()
{
	GWindow->Update();

	GWindow->Render();
}
