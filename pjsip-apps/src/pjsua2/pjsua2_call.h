#pragma once

#include <pjsua2.hpp>
#include <iostream>
#include "pjsua2_account.h"

using namespace pj;

class Pjsua2_Call : public Call
{
public:
	Pjsua2_Call(Account &acc, int call_id = PJSUA_INVALID_ID)
	: Call(acc, call_id)
	{
		mAcc = (Pjsua2_Account *)&acc;
	}
	~Pjsua2_Call();
    
	virtual void onCallState(OnCallStateParam &prm);
private:
	Pjsua2_Account *mAcc;
};
