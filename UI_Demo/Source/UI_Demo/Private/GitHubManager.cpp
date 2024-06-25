// Fill out your copyright notice in the Description page of Project Settings.


#include "GitHubManager.h"
#include "GitHubRepositoryInfo.h"

void UGitHubManager::SearchRepositories(const FString& Keyword)
{
    FString Url = FString::Printf(TEXT("https://api.github.com/search/repositories?q=%s"), *Keyword);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetVerb("GET");
    Request->SetHeader("Content-Type", "application/json");
    Request->SetHeader("User-Agent", "UnrealEngine");

    Request->OnProcessRequestComplete().BindUObject(this, &UGitHubManager::OnSearchCompleted);

    Request->ProcessRequest();
}

void UGitHubManager::OnSearchCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    TArray<FGitHubRepositoryInfo> GitHubInfos;

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

                TSharedPtr<FJsonObject> OwnerObject = RepositoryObject->GetObjectField("owner");
                
                FString OwnerAvatarUrl = OwnerObject->GetStringField("avatar_url");

                UE_LOG(LogTemp, Log, TEXT("Repository Name: %s, URL: %s, Owner's Avatar URL: %s"), *Name, *HtmlUrl, *OwnerAvatarUrl);

                FGitHubRepositoryInfo GitHubInfo;
                GitHubInfo.Name = Name;
                GitHubInfo.HtmlUrl = HtmlUrl;
                GitHubInfo.AvatarUrl = OwnerAvatarUrl;
                GitHubInfos.Add(GitHubInfo);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get response or response is invalid."));
        
    }

    OnGitHubSearchCompleted.Broadcast(GitHubInfos);
}


void UGitHubManager::TestSearchRepositories(const FString& Keyword)
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

void UGitHubManager::DownloadImageAndApplyToImageWidget(const FString& ImageUrl, UImage* ImageWidget)
{
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(ImageUrl);
    HttpRequest->SetVerb("GET");
    HttpRequest->OnProcessRequestComplete().BindLambda([ImageWidget](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid() && Response->GetContentLength() > 0)
            {
                IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
                TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

                if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(Response->GetContent().GetData(), Response->GetContentLength()))
                {
                    TArray64<uint8> RawData;
                    if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
                    {
                        UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
                        if (Texture)
                        {
                            void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
                            FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
                            Texture->GetPlatformData()->Mips[0].BulkData.Unlock();

                            // Update the texture with new data
                            Texture->UpdateResource();

                            // Apply the texture to the image widget
                            ImageWidget->SetBrushFromTexture(Texture);
                        }
                    }
                }
            }
        });
    HttpRequest->ProcessRequest();
    
}
