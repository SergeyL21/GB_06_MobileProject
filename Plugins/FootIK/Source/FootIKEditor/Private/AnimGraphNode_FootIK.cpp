// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimGraphNode_FootIK.h"

#define LOCTEXT_NAMESPACE "FFootIKEditorModule"

// --------------------------------------------------------------------------------------
FText UAnimGraphNode_FootIK::GetTooltipText() const
{
	return LOCTEXT("FootIK", "FootIK");
}

// --------------------------------------------------------------------------------------
FText UAnimGraphNode_FootIK::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("FootIK", "FootIK");
}

// --------------------------------------------------------------------------------------
FString UAnimGraphNode_FootIK::GetNodeCategory() const
{
	return TEXT("FootIK");
}

// --------------------------------------------------------------------------------------
const FAnimNode_SkeletalControlBase* UAnimGraphNode_FootIK::GetNode() const
{
	return &Node;
}
