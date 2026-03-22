// Copyright NALWorlds Team. All Rights Reserved.

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Data/GenAIOrgs.h"
#include "Data/Anthropic/GenClaudeChatStructs.h"
#include "Data/OpenAI/GenOAIChatStructs.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#if WITH_DEV_AUTOMATION_TESTS

// ---------------------------------------------------------------------------
// EGenAIOrgs enum coverage
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIOrgsEnumTest,
	"UEWorlds.GenAI.Types.OrganizationEnumCoverage",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIOrgsEnumTest::RunTest(const FString& Parameters)
{
	TSet<uint8> OrgValues;
	OrgValues.Add(static_cast<uint8>(EGenAIOrgs::OpenAI));
	OrgValues.Add(static_cast<uint8>(EGenAIOrgs::DeepSeek));
	OrgValues.Add(static_cast<uint8>(EGenAIOrgs::Anthropic));
	OrgValues.Add(static_cast<uint8>(EGenAIOrgs::Meta));
	OrgValues.Add(static_cast<uint8>(EGenAIOrgs::Google));
	OrgValues.Add(static_cast<uint8>(EGenAIOrgs::XAI));
	OrgValues.Add(static_cast<uint8>(EGenAIOrgs::Unknown));

	TestEqual(TEXT("All 7 org values are distinct"), OrgValues.Num(), 7);

	return true;
}

// ---------------------------------------------------------------------------
// EClaudeModels enum coverage
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIClaudeModelsEnumTest,
	"UEWorlds.GenAI.Claude.ModelEnumCoverage",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIClaudeModelsEnumTest::RunTest(const FString& Parameters)
{
	TSet<uint8> ModelValues;
	ModelValues.Add(static_cast<uint8>(EClaudeModels::Claude_4_Opus));
	ModelValues.Add(static_cast<uint8>(EClaudeModels::Claude_4_Sonnet));
	ModelValues.Add(static_cast<uint8>(EClaudeModels::Claude_3_5_Sonnet));
	ModelValues.Add(static_cast<uint8>(EClaudeModels::Claude_3_7_Sonnet));
	ModelValues.Add(static_cast<uint8>(EClaudeModels::Claude_3_5_Haiku));
	ModelValues.Add(static_cast<uint8>(EClaudeModels::Claude_3_Opus));

	TestEqual(TEXT("All 6 Claude models are distinct"), ModelValues.Num(), 6);

	return true;
}

// ---------------------------------------------------------------------------
// FGenClaudeChatSettings defaults
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIClaudeChatSettingsDefaultsTest,
	"UEWorlds.GenAI.Claude.ChatSettingsDefaults",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIClaudeChatSettingsDefaultsTest::RunTest(const FString& Parameters)
{
	FGenClaudeChatSettings Settings;

	TestEqual(TEXT("Default model is Claude_4_Sonnet"), Settings.Model, EClaudeModels::Claude_4_Sonnet);
	TestEqual(TEXT("Default MaxTokens is 1024"), Settings.MaxTokens, 1024);
	TestTrue(TEXT("Default Temperature near 0.7"),
		FMath::IsNearlyEqual(Settings.Temperature, 0.7f));
	TestFalse(TEXT("Stream response disabled by default"), Settings.bStreamResponse);
	TestEqual(TEXT("No messages by default"), Settings.Messages.Num(), 0);

	// Validate ranges
	TestTrue(TEXT("MaxTokens > 0"), Settings.MaxTokens > 0);
	TestTrue(TEXT("Temperature >= 0"), Settings.Temperature >= 0.f);
	TestTrue(TEXT("Temperature <= 2"), Settings.Temperature <= 2.f);

	return true;
}

// ---------------------------------------------------------------------------
// FGenChatMessage structure
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIChatMessageTest,
	"UEWorlds.GenAI.Types.ChatMessageDefaults",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIChatMessageTest::RunTest(const FString& Parameters)
{
	FGenChatMessage Message;

	TestEqual(TEXT("Default role is user"), Message.Role, TEXT("user"));
	TestTrue(TEXT("Default content is empty"), Message.Content.IsEmpty());

	// Set values
	Message.Role = TEXT("assistant");
	Message.Content = TEXT("Hello, I can help you with that.");

	TestEqual(TEXT("Role set to assistant"), Message.Role, TEXT("assistant"));
	TestFalse(TEXT("Content is not empty"), Message.Content.IsEmpty());

	return true;
}

// ---------------------------------------------------------------------------
// FGenChatSettings (OpenAI) defaults
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIOAIChatSettingsDefaultsTest,
	"UEWorlds.GenAI.OpenAI.ChatSettingsDefaults",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIOAIChatSettingsDefaultsTest::RunTest(const FString& Parameters)
{
	FGenChatSettings Settings;

	TestEqual(TEXT("Default MaxTokens is 10000"), Settings.MaxTokens, 10000);
	TestTrue(TEXT("Default Temperature is 1.0"),
		FMath::IsNearlyEqual(Settings.Temperature, 1.0f));
	TestTrue(TEXT("Default TopP is 1.0"),
		FMath::IsNearlyEqual(Settings.TopP, 1.0f));
	TestEqual(TEXT("No messages by default"), Settings.Messages.Num(), 0);
	TestEqual(TEXT("Default ReasoningEffort"),
		Settings.ReasoningEffort, EGenAIOpenAIReasoningEffort::Default);
	TestEqual(TEXT("Default Verbosity"),
		Settings.Verbosity, EGenAIOpenAIVerbosity::Default);

	return true;
}

// ---------------------------------------------------------------------------
// FGenChatSettings UpdateModel
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIOAIUpdateModelTest,
	"UEWorlds.GenAI.OpenAI.UpdateModelFromEnum",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIOAIUpdateModelTest::RunTest(const FString& Parameters)
{
	FGenChatSettings Settings;

	// UpdateModel should populate the Model string from ModelEnum
	Settings.ModelEnum = EGenOAIChatModel::GPT_35_Turbo;
	Settings.UpdateModel();
	TestFalse(TEXT("Model string not empty after UpdateModel"), Settings.Model.IsEmpty());

	return true;
}

// ---------------------------------------------------------------------------
// Chat request JSON structure — OpenAI format
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIOpenAIJsonRequestTest,
	"UEWorlds.GenAI.OpenAI.RequestJsonStructure",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIOpenAIJsonRequestTest::RunTest(const FString& Parameters)
{
	// Build a JSON request matching OpenAI chat completion format
	TSharedPtr<FJsonObject> Request = MakeShareable(new FJsonObject);
	Request->SetStringField(TEXT("model"), TEXT("gpt-4o"));
	Request->SetNumberField(TEXT("max_tokens"), 1024);
	Request->SetNumberField(TEXT("temperature"), 0.7);

	TArray<TSharedPtr<FJsonValue>> Messages;
	TSharedPtr<FJsonObject> Msg = MakeShareable(new FJsonObject);
	Msg->SetStringField(TEXT("role"), TEXT("user"));
	Msg->SetStringField(TEXT("content"), TEXT("Hello"));
	Messages.Add(MakeShareable(new FJsonValueObject(Msg)));
	Request->SetArrayField(TEXT("messages"), Messages);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	TestTrue(TEXT("Request serializes"), FJsonSerializer::Serialize(Request.ToSharedRef(), Writer));
	TestFalse(TEXT("Output not empty"), OutputString.IsEmpty());

	// Re-parse and verify structure
	TSharedPtr<FJsonObject> Parsed;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(OutputString);
	TestTrue(TEXT("Re-parses"), FJsonSerializer::Deserialize(Reader, Parsed));

	FString Model;
	TestTrue(TEXT("Has model field"), Parsed->TryGetStringField(TEXT("model"), Model));
	TestEqual(TEXT("Model is gpt-4o"), Model, TEXT("gpt-4o"));

	double MaxTokens;
	TestTrue(TEXT("Has max_tokens"), Parsed->TryGetNumberField(TEXT("max_tokens"), MaxTokens));
	TestEqual(TEXT("max_tokens is 1024"), static_cast<int32>(MaxTokens), 1024);

	const TArray<TSharedPtr<FJsonValue>>* ParsedMsgs;
	TestTrue(TEXT("Has messages array"), Parsed->TryGetArrayField(TEXT("messages"), ParsedMsgs));
	TestEqual(TEXT("1 message"), ParsedMsgs->Num(), 1);

	return true;
}

// ---------------------------------------------------------------------------
// Chat response JSON parsing — OpenAI format
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIOpenAIJsonResponseTest,
	"UEWorlds.GenAI.OpenAI.ResponseJsonParsing",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIOpenAIJsonResponseTest::RunTest(const FString& Parameters)
{
	const FString ResponseJson = TEXT(R"({
		"choices": [{
			"message": {
				"role": "assistant",
				"content": "Hello! How can I help you today?"
			}
		}]
	})");

	TSharedPtr<FJsonObject> Parsed;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseJson);
	TestTrue(TEXT("Response parses"), FJsonSerializer::Deserialize(Reader, Parsed));

	const TArray<TSharedPtr<FJsonValue>>* Choices;
	TestTrue(TEXT("Has choices"), Parsed->TryGetArrayField(TEXT("choices"), Choices));
	TestEqual(TEXT("1 choice"), Choices->Num(), 1);

	const TSharedPtr<FJsonObject>* MessageObj;
	TestTrue(TEXT("Choice has message"),
		(*Choices)[0]->AsObject()->TryGetObjectField(TEXT("message"), MessageObj));

	FString Role;
	TestTrue(TEXT("Message has role"), (*MessageObj)->TryGetStringField(TEXT("role"), Role));
	TestEqual(TEXT("Role is assistant"), Role, TEXT("assistant"));

	FString Content;
	TestTrue(TEXT("Message has content"), (*MessageObj)->TryGetStringField(TEXT("content"), Content));
	TestFalse(TEXT("Content is not empty"), Content.IsEmpty());

	return true;
}

// ---------------------------------------------------------------------------
// Claude API JSON request structure
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIClaudeJsonRequestTest,
	"UEWorlds.GenAI.Claude.RequestJsonStructure",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIClaudeJsonRequestTest::RunTest(const FString& Parameters)
{
	// Build a JSON request matching Anthropic Messages API format
	TSharedPtr<FJsonObject> Request = MakeShareable(new FJsonObject);
	Request->SetStringField(TEXT("model"), TEXT("claude-sonnet-4-20250514"));
	Request->SetNumberField(TEXT("max_tokens"), 1024);

	TArray<TSharedPtr<FJsonValue>> Messages;
	TSharedPtr<FJsonObject> Msg = MakeShareable(new FJsonObject);
	Msg->SetStringField(TEXT("role"), TEXT("user"));
	Msg->SetStringField(TEXT("content"), TEXT("What is UE5?"));
	Messages.Add(MakeShareable(new FJsonValueObject(Msg)));
	Request->SetArrayField(TEXT("messages"), Messages);

	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	TestTrue(TEXT("Claude request serializes"), FJsonSerializer::Serialize(Request.ToSharedRef(), Writer));

	// Re-parse
	TSharedPtr<FJsonObject> Parsed;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(OutputString);
	TestTrue(TEXT("Re-parses"), FJsonSerializer::Deserialize(Reader, Parsed));

	FString Model;
	Parsed->TryGetStringField(TEXT("model"), Model);
	TestTrue(TEXT("Model contains claude"), Model.Contains(TEXT("claude")));

	return true;
}

// ---------------------------------------------------------------------------
// Structured output settings defaults
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIStructuredOutputSettingsTest,
	"UEWorlds.GenAI.OpenAI.StructuredOutputSettingsDefaults",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIStructuredOutputSettingsTest::RunTest(const FString& Parameters)
{
	FGenOAIStructuredChatSettings Settings;

	TestTrue(TEXT("bUseSchema is true by default"), Settings.bUseSchema);
	TestTrue(TEXT("Name is empty by default"), Settings.Name.IsEmpty());
	TestTrue(TEXT("SchemaJson is empty by default"), Settings.SchemaJson.IsEmpty());

	// Set a schema
	Settings.Name = TEXT("ExtractPerson");
	Settings.SchemaJson = TEXT(R"({"type":"object","properties":{"name":{"type":"string"}}})");

	TestEqual(TEXT("Schema name set"), Settings.Name, TEXT("ExtractPerson"));
	TestFalse(TEXT("Schema JSON not empty"), Settings.SchemaJson.IsEmpty());

	// Validate the schema JSON is parseable
	TSharedPtr<FJsonObject> SchemaObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Settings.SchemaJson);
	TestTrue(TEXT("Schema JSON is valid JSON"), FJsonSerializer::Deserialize(Reader, SchemaObj));

	FString SchemaType;
	SchemaObj->TryGetStringField(TEXT("type"), SchemaType);
	TestEqual(TEXT("Schema type is object"), SchemaType, TEXT("object"));

	return true;
}

// ---------------------------------------------------------------------------
// EOpenAIModels enum
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIOpenAIModelsEnumTest,
	"UEWorlds.GenAI.OpenAI.ModelEnumCoverage",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIOpenAIModelsEnumTest::RunTest(const FString& Parameters)
{
	TSet<uint8> ModelValues;
	ModelValues.Add(static_cast<uint8>(EOpenAIModels::GPT4o));
	ModelValues.Add(static_cast<uint8>(EOpenAIModels::GPT4oMini));
	ModelValues.Add(static_cast<uint8>(EOpenAIModels::GPT3_5Turbo));
	ModelValues.Add(static_cast<uint8>(EOpenAIModels::DaVinci));
	ModelValues.Add(static_cast<uint8>(EOpenAIModels::Unknown));

	TestEqual(TEXT("All 5 OpenAI models are distinct"), ModelValues.Num(), 5);

	return true;
}

// ---------------------------------------------------------------------------
// EDeepSeekModels enum
// ---------------------------------------------------------------------------

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FGenAIDeepSeekModelsEnumTest,
	"UEWorlds.GenAI.DeepSeek.ModelEnumCoverage",
	EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter)

bool FGenAIDeepSeekModelsEnumTest::RunTest(const FString& Parameters)
{
	TSet<uint8> ModelValues;
	ModelValues.Add(static_cast<uint8>(EDeepSeekModels::Chat));
	ModelValues.Add(static_cast<uint8>(EDeepSeekModels::Reasoner));
	ModelValues.Add(static_cast<uint8>(EDeepSeekModels::Unknown));

	TestEqual(TEXT("All 3 DeepSeek models are distinct"), ModelValues.Num(), 3);

	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
