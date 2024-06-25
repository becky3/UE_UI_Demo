// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GitHubRepositoryInfo.generated.h"

USTRUCT(BlueprintType)
struct FGitHubRepositoryInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FString HtmlUrl;

	UPROPERTY(BlueprintReadOnly)
	FString AvatarUrl;

};
