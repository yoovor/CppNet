#include <string>
#include <vector>
#include <iostream>

#include "include/cppnet.h"
#include "common/util/time.h"

using namespace cppnet;

void ConnectFunc(Handle handle, uint32_t err) {
	if (err == CEC_SUCCESS) {
		std::string msg("it is a test message.");
		std::string str = msg;
		str.reserve(msg.size() * 100);
		for (int i = 0; i < 100; i++)
			str += msg;

		str += "}";
		handle->Write(str.c_str(), (uint32_t)str.length());
		//handle->Close();
	}
	else {
		std::cout << " [ConnectFunc] some thing error : " << err << std::endl;
	}
}

void ReadFunc(Handle handle, std::shared_ptr<Buffer> data, uint32_t len)
{
    char msg_buf[128] = {0};
    // get recv data to send back.
    std::string str;
    uint32_t    size = 0;

    do {
        uint32_t size = data->Read(msg_buf, 128);
        str += std::string(msg_buf, size);
    } while (size > 0);

    std::cout << "from server, size=" << str.length() << ", " << str.c_str() << std::endl;
}

void DisConnectFunc(Handle handle, uint32_t err)
{
    if (err == CEC_SUCCESS) {
        std::cout << handle->GetListenPort() << " DisConnectFunc: " << handle->GetSocket() << std::endl;

    } else {
        std::cout << "[DisConnectFunc] some thing error : " << err << std::endl;
    }
}


int main() {

    cppnet::CppNet net;
    net.Init(0);
    net.SetConnectionCallback(ConnectFunc);
    net.SetDisconnectionCallback(DisConnectFunc);
    net.SetReadCallback(ReadFunc);

    net.Connection("127.0.0.1", 8921);
    net.Connection("127.0.0.1", 8922);

    net.Join();
}