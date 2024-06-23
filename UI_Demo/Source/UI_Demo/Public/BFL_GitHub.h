// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GitHubManager.h"
#include "BFL_GitHub.generated.h"


/**
 * 
 */
UCLASS()
class UI_DEMO_API UBFL_GitHub : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    // APIリクエスト管理クラスのインスタンスを作成し、APIを呼び出す関数
    UFUNCTION(BlueprintCallable, Category = "API")
    static UGitHubManager* CreateGitHubManager();

};

