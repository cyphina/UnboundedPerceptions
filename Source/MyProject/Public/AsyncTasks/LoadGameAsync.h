// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * AsyncTask version of casting a spell
 * NonAbandonableTask means the task can't be removed from thread pool after started, not that it can't be stopped
 */

class MYPROJECT_API FAsyncLoadGameTask : public FNonAbandonableTask
{
public:
	FAsyncLoadGameTask();
	~FAsyncLoadGameTask();

protected:

	void DoWork()
	{
		// Place the Async Code here.  This function runs automatically.
	}
 
	// This next section of code needs to be here.  Not important as to why.
 
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FCastSpellTask, STATGROUP_ThreadPoolAsyncTasks);
	}
};
