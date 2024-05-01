// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "SPAIController.h"
#include "SPCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	auto SPCharacter = Cast<ASPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == SPCharacter) return EBTNodeResult::Failed;

	SPCharacter->Attack();
	IsAttacking = true;
	SPCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
		});
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
