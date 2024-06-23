// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"
#include "Kismet/KismetSystemLibrary.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();

	UKismetSystemLibrary::PrintString(this, "C++ Hello World!", true, true, FColor::Cyan, 2.f, TEXT("None"));

    SearchGitHubRepositories(TEXT("UnrealEngine"));
	
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor::SearchGitHubRepositories(const FString& Keyword)
{
    FString Url = FString::Printf(TEXT("https://api.github.com/search/repositories?q=%s"), *Keyword);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("User-Agent", "UnrealEngine"); // GitHub API‚ÍUser-Agent‚ð•K—v‚Æ‚·‚é

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response->GetResponseCode() == 200)
            {
                TSharedPtr<FJsonObject> JsonObject;
                TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
                if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
                {
                    const TArray<TSharedPtr<FJsonValue>>& Items = JsonObject->GetArrayField("items");
                    for (const TSharedPtr<FJsonValue>& Item : Items)
                    {
                        TSharedPtr<FJsonObject> RepositoryObject = Item->AsObject();
                        FString Name = RepositoryObject->GetStringField("name");
                        FString HtmlUrl = RepositoryObject->GetStringField("html_url");
                        UE_LOG(LogTemp, Log, TEXT("Repository Name: %s, URL: %s"), *Name, *HtmlUrl);
                    }
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to get response or response is invalid."));
            }
        });

    Request->ProcessRequest();
}