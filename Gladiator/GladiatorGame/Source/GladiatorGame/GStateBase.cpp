// Fill out your copyright notice in the Description page of Project Settings.

#include "GStateBase.h"

void AGStateBase::SetBattleState(EBattleState p_newState)
{
	GameState = p_newState;
}

EBattleState AGStateBase::GetBattleState()
{
	return GameState;
}
