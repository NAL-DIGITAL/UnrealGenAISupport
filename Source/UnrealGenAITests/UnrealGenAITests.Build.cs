using UnrealBuildTool;

public class UnrealGenAITests : ModuleRules
{
    public UnrealGenAITests(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "GenerativeAISupport",
            "HTTP",
            "Json",
            "JsonUtilities",
        });
    }
}
