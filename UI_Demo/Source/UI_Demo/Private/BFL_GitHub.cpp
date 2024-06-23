// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_GitHub.h"



void UBFL_GitHub::SearchRepositories(const FString& Keyword)
{
    FString Url = FString::Printf(TEXT("https://api.github.com/search/repositories?q=%s"), *Keyword);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("User-Agent", "UnrealEngine");

    Request->OnProcessRequestComplete().BindUObject(this, &UBFL_GitHub::OnSearchCompleted);

    Request->ProcessRequest();
}

void UBFL_GitHub::OnSearchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response->GetResponseCode() == 200)
    {
        FString JsonResponse = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Response: %s"), *JsonResponse);
        //OnGitHubSearchCompleted.Broadcast(JsonResponse);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get response or response is invalid."));
        //OnGitHubSearchCompleted.Broadcast(TEXT(""));
    }
}


void UBFL_GitHub::TestSearchRepositories(const FString& Keyword)
{
    FString Url = FString::Printf(TEXT("https://api.github.com/search/repositories?q=%s"), *Keyword);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("User-Agent", "UnrealEngine");

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
