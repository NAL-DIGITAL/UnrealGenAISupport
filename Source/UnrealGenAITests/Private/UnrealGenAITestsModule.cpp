// Copyright NALWorlds Team. All Rights Reserved.

#include "Modules/ModuleManager.h"

class FUnrealGenAITestsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FUnrealGenAITestsModule, UnrealGenAITests)
