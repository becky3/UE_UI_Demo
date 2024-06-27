// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "UObject/NoExportTypes.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonObjectConverter.h"
#include "JsonUtilities.h"
#include "GitHubRepositoryInfo.h"
#include "GitHubManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGitHubApiCompleted, const TArray<FGitHubRepositoryInfo>&, GithubInfos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetTextureCompleted, UTexture2D*, Texture);

/**
 * 
 */
UCLASS(BlueprintType)
class UI_DEMO_API UGitHubManager : public UObject
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "GitHub")
    void SearchRepositories(const FString& Keyword);

    UPROPERTY(BlueprintAssignable, Category = "GitHub")
    FOnGitHubApiCompleted OnGitHubSearchCompleted;

    UFUNCTION(BlueprintCallable, Category = "GitHub")
    void GetTextureFromUrl(const FString& ImageUrl);

	UPROPERTY(BlueprintAssignable, Category = "GitHub")
    FOnGetTextureCompleted OnGetTextureCompleted;

private:
    void OnSearchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void OnGetImageCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
