#pragma once

#include <pjsua2.hpp>
#include <iostream>

using namespace pj;

class Pjsua2_Account : public Account
{
public:
	std::vector<Call *> calls;

public:
	Pjsua2_Account();
	~Pjsua2_Account();

	void removeCall(Call *call);

	virtual void onRegState(OnRegStateParam &prm);
	virtual void onIncomingCall(OnIncomingCallParam &iprm);
};
