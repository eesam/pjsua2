#include "pjsua2_call.h"
#include <pj/log.h>

#define THIS_FILE 	"pjsua2_server"

Pjsua2_Call::~Pjsua2_Call()
{
}

void Pjsua2_Call::onCallState(OnCallStateParam &prm)
{
	PJ_UNUSED_ARG(prm);

	CallInfo ci = getInfo();
	std::cout << "*** Call: " <<  ci.remoteUri << " [" << ci.stateText << "]" << std::endl;

	if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
		mAcc->removeCall(this);
		/* Delete the call */
		delete this;
	}
}
