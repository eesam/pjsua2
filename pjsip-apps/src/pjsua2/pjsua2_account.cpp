#include "pjsua2_account.h"
#include "pjsua2_call.h"
#include <pj/log.h>

Pjsua2_Account::Pjsua2_Account()
{
}

Pjsua2_Account::~Pjsua2_Account()
{
	std::cout << "*** Account is being deleted: No of calls=" << calls.size() << std::endl;
}

void Pjsua2_Account::removeCall(Call *call)
{
	std::vector<Call *>::iterator it = calls.begin();
	for (; it != calls.end(); ++it) {
		if (*it == call) {
			calls.erase(it);
			break;
		}
	}
}

void Pjsua2_Account::onRegState(OnRegStateParam &prm)
{
	AccountInfo ai = getInfo();
	std::cout << (ai.regIsActive? "*** Register: code=" : "*** Unregister: code=") << prm.code << std::endl;
}

void Pjsua2_Account::onIncomingCall(OnIncomingCallParam &iprm)
{
	Call *call = new Pjsua2_Call(*this, iprm.callId);
	CallInfo ci = call->getInfo();
	CallOpParam prm;

	std::cout << "*** Incoming Call: " <<  ci.remoteUri << " [" << ci.stateText << "]" << std::endl;

	calls.push_back(call);
	prm.statusCode = (pjsip_status_code)200;
	call->answer(prm);
}