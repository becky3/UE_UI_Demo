// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_GitHub.h"

UGitHubManager* UBFL_GitHub::CreateGitHubManager()
{
    return NewObject<UGitHubManager>();
}