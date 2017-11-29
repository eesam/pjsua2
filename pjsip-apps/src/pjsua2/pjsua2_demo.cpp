#include <pjsua2.hpp>
#include <iostream>
#include <pj/file_access.h>

#include "pjsua2_server.h"
#include "pjsua2_account.h"
#include "pjsua2_call.h"


#define THIS_FILE 	"pjsua2_demo.cpp"

using namespace pj;

static void mainProg1(Endpoint &ep) throw(Error)
{
    // Init library
    EpConfig ep_cfg;
    ep_cfg.logConfig.level = 4;
    ep.libInit( ep_cfg );

    // Transport
    TransportConfig tcfg;
    tcfg.port = 5060;
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

    // Start library
    ep.libStart();
    std::cout << "*** PJSUA2 STARTED ***" << std::endl;

    // Add account
    AccountConfig acc_cfg;
    acc_cfg.idUri = "sip:test1@pjsip.org";
    acc_cfg.regConfig.registrarUri = "sip:sip.pjsip.org";
    acc_cfg.sipConfig.authCreds.push_back( AuthCredInfo("digest", "*",
                                                        "test1", 0, "test1") );
    Pjsua2_Account *acc(new Pjsua2_Account);
    acc->create(acc_cfg);
    
    pj_thread_sleep(2000);
    
    // Make outgoing call
    Call *call = new Pjsua2_Call(*acc);
    acc->calls.push_back(call);
    CallOpParam prm(true);
    prm.opt.audioCount = 1;
    prm.opt.videoCount = 0;
    call->makeCall("sip:test1@pjsip.org", prm);
    
    // Hangup all calls
    pj_thread_sleep(8000);
    ep.hangupAllCalls();
    pj_thread_sleep(4000);
    
    // Destroy library
    std::cout << "*** PJSUA2 SHUTTING DOWN ***" << std::endl;
    delete call;
    delete acc;
}

static void mainProg2() throw(Error)
{
    string json_str;
    {
	EpConfig epCfg;
	JsonDocument jDoc;

	epCfg.uaConfig.maxCalls = 61;
	epCfg.uaConfig.userAgent = "Just JSON Test";
	epCfg.uaConfig.stunServer.push_back("stun1.pjsip.org");
	epCfg.uaConfig.stunServer.push_back("stun2.pjsip.org");
	epCfg.logConfig.filename = "THE.LOG";

	jDoc.writeObject(epCfg);
	json_str = jDoc.saveString();
	std::cout << json_str << std::endl << std::endl;
    }

    {
	EpConfig epCfg;
	JsonDocument rDoc;
	string output;

	rDoc.loadString(json_str);
	rDoc.readObject(epCfg);

	JsonDocument wDoc;

	wDoc.writeObject(epCfg);
	json_str = wDoc.saveString();
	std::cout << json_str << std::endl << std::endl;

	wDoc.saveFile("jsontest.js");
    }

    {
	EpConfig epCfg;
	JsonDocument rDoc;

	rDoc.loadFile("jsontest.js");
	rDoc.readObject(epCfg);
	pj_file_delete("jsontest.js");
    }
}


static void mainProg3(Endpoint &ep) throw(Error)
{
    const char *paths[] = { "../../../../tests/pjsua/wavs/input.16.wav",
			    "../../tests/pjsua/wavs/input.16.wav",
			    "input.16.wav"};
    unsigned i;
    const char *filename = NULL;

    // Init library
    EpConfig ep_cfg;
    ep.libInit( ep_cfg );

    for (i=0; i<PJ_ARRAY_SIZE(paths); ++i) {
       if (pj_file_exists(paths[i])) {
          filename = paths[i];
          break;
       }
    }

    if (!filename) {
	PJSUA2_RAISE_ERROR3(PJ_ENOTFOUND, "mainProg3()",
			   "Could not locate input.16.wav");
    }

    // Start library
    ep.libStart();
    std::cout << "*** PJSUA2 STARTED ***" << std::endl;

    // Create player and recorder
    {
	AudioMediaPlayer amp;
	amp.createPlayer(filename);

	AudioMediaRecorder amr;
	amr.createRecorder("recorder_test_output.wav");

	amp.startTransmit(ep.audDevManager().getPlaybackDevMedia());
	amp.startTransmit(amr);

	pj_thread_sleep(5000);
    }
}


static void mainProg() throw(Error)
{
    string json_str;

    {
	JsonDocument jdoc;
	AccountConfig accCfg;

	accCfg.idUri = "\"Just Test\" <sip:test@pjsip.org>";
	accCfg.regConfig.registrarUri = "sip:sip.pjsip.org";
	SipHeader h;
	h.hName = "X-Header";
	h.hValue = "User header";
	accCfg.regConfig.headers.push_back(h);

	accCfg.sipConfig.proxies.push_back("<sip:sip.pjsip.org;transport=tcp>");
	accCfg.sipConfig.proxies.push_back("<sip:sip.pjsip.org;transport=tls>");

	accCfg.mediaConfig.transportConfig.tlsConfig.ciphers.push_back(1);
	accCfg.mediaConfig.transportConfig.tlsConfig.ciphers.push_back(2);
	accCfg.mediaConfig.transportConfig.tlsConfig.ciphers.push_back(3);

	AuthCredInfo aci;
	aci.scheme = "digest";
	aci.username = "test";
	aci.data = "passwd";
	aci.realm = "*";
	accCfg.sipConfig.authCreds.push_back(aci);

	jdoc.writeObject(accCfg);
	json_str = jdoc.saveString();
	std::cout << "Original:" << std::endl;
	std::cout << json_str << std::endl << std::endl;
    }

    {
	JsonDocument rdoc;

	rdoc.loadString(json_str);
	AccountConfig accCfg;
	rdoc.readObject(accCfg);

	JsonDocument wdoc;
	wdoc.writeObject(accCfg);
	json_str = wdoc.saveString();

	std::cout << "Parsed:" << std::endl;
	std::cout << json_str << std::endl << std::endl;
    }
}


static void mainProg4(Endpoint &ep) throw(Error)
{
    // Init library
    EpConfig ep_cfg;
    ep.libInit( ep_cfg );

    // Create transport
    TransportConfig tcfg;
    tcfg.port = 5060;
    ep.transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
    ep.transportCreate(PJSIP_TRANSPORT_TCP, tcfg);

    // Add account
    AccountConfig acc_cfg;
    acc_cfg.idUri = "sip:1021@120.27.249.129";
	acc_cfg.regConfig.registrarUri = "sip:120.27.249.129";
	AuthCredInfo cred("digest", "*", "1021", 0, "123456");
	acc_cfg.sipConfig.authCreds.push_back(cred);
    Pjsua2_Account *acc(new Pjsua2_Account);
    acc->create(acc_cfg);

    // Start library
    ep.libStart();
    std::cout << "*** PJSUA2 STARTED ***" << std::endl;

	Pjsua2_Server server;
	server.start();
	std::cout << "Press ENTER to quit..." << std::endl;
	std::cin.get();

    // Just wait for ENTER key
    std::cout << "Press ENTER to quit..." << std::endl;
    std::cin.get();

    delete acc;
}


// int main()
// {
//     int ret = 0;
//     Endpoint ep;

//     try {
// 	ep.libCreate();

// 	mainProg4(ep);

// 	ret = PJ_SUCCESS;
//     } catch (Error & err) {
// 	std::cout << "Exception: " << err.info() << std::endl;
// 	ret = 1;
//     }

//     try {
// 	ep.libDestroy();
//     } catch(Error &err) {
// 	std::cout << "Exception: " << err.info() << std::endl;
// 	ret = 1;
//     }

//     if (ret == PJ_SUCCESS) {
// 	std::cout << "Success" << std::endl;
//     } else {
// 	std::cout << "Error Found" << std::endl;
//     }

//     return ret;
// }


