// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidget/SceneAssetViewer.h"
#include "EngineUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"

void SSceneAssetViewerWidget::Construct(const FArguments& inArgs)
{
	PopulateAssetSet();
	ChildSlot
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SAssignNew(ListViewWidget, SListView<TSharedPtr<ListAsset>>)
						.ListItemsSource(&AssetsList)
						.OnGenerateRow(this, &SSceneAssetViewerWidget::OnGenerateRow)
				]
		];
}

void SSceneAssetViewerWidget::PopulateAssetSet()
{
	BlueprintSet.Empty();
	MeshSet.Empty();
	MatSet.Empty();
	AssetsList.Empty();

	if (UWorld* world = GEditor->GetEditorWorldContext().World())
	{
		for (TActorIterator<AActor> iterator(world); iterator; ++iterator)
		{
			AActor* actor = *iterator;
			if (UBlueprint* bp = Cast<UBlueprint>(actor->GetClass()->ClassGeneratedBy))
			{
				FString bpPathName = bp->GetPathName();
				if (!BlueprintSet.Contains(bpPathName))
				{
					AssetsList.Add(MakeShared<ListAsset>(TEXT("Blueprint"), bpPathName));
					BlueprintSet.Add(bp->GetPathName());
				}
			}
			TArray<UActorComponent*> actorComponents;
			actor->GetComponents(actorComponents);
			for (UActorComponent* actorComponent : actorComponents)
			{
				if (UStaticMeshComponent* staticMeshComponent = Cast<UStaticMeshComponent>(actorComponent))
				{
					if (UStaticMesh* staticMesh = staticMeshComponent->GetStaticMesh())
					{
						FString staticMeshPathName = staticMesh->GetPathName();
						if (!MeshSet.Contains(staticMeshPathName))
						{
							AssetsList.Add(MakeShared<ListAsset>(TEXT("Mesh"), staticMeshPathName));
							MeshSet.Add(staticMesh->GetPathName());
						}
					}
					for (int32 materialIndex = 0; materialIndex < staticMeshComponent->GetNumMaterials(); ++materialIndex)
					{
						if (UMaterialInterface* material = staticMeshComponent->GetMaterial(materialIndex))
						{
							FString materialPathName = material->GetPathName();
							if (!MatSet.Contains(materialPathName))
							{
								AssetsList.Add(MakeShared<ListAsset>(TEXT("Material"), materialPathName));
								MatSet.Add(material->GetPathName());
							}
						}
					}
				}
			}
		}
	}
}

TSharedRef<ITableRow> SSceneAssetViewerWidget::OnGenerateRow(TSharedPtr<ListAsset> item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().FillWidth(0.3f)
				[
					SNew(STextBlock)
						.Text(FText::FromString((*item).Type))
						.Justification(ETextJustify::InvariantLeft)
				]
				+ SHorizontalBox::Slot().FillWidth(0.3f)
				[
					SNew(STextBlock)
						.Text(FText::FromString((*item).Path))
						.Justification(ETextJustify::InvariantLeft)
				]
				+ SHorizontalBox::Slot().FillWidth(0.3f)
				[
					SNew(SButton)
						.Text(FText::FromString("Show in Content Browser"))
						.OnClicked_Lambda([item]()->FReply {
							IAssetRegistry& AssetRegistryInterface  = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
							FAssetData AssetData = AssetRegistryInterface.GetAssetByObjectPath((*item).Path);
							if (GEditor && AssetData.IsValid())
							{
								GEditor->SyncBrowserToObject(AssetData);
							}
							return FReply::Handled(); 
						})
				]
		];
}
