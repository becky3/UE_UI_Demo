// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "JsonObjectConverter.h"
#include "BFL_GitHub.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGitHubApiCompleted, const FString&, JsonResponse);

/**
 * 
 */
UCLASS()
class UI_DEMO_API UBFL_GitHub : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "GitHub")
    void SearchRepositories(const FString& Keyword);

    //UPROPERTY(BlueprintAssignable, Category = "GitHub")
    //FOnGitHubApiCompleted OnGitHubSearchCompleted;

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void TestSearchRepositories(const FString& Keyword);

private:
    void OnSearchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};

