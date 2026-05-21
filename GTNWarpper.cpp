#pragma once

#include <cstdlib>
#include <stdexcept>
#include "GTNWarpper.h"
#include <shared_mutex>
#include "include/gts.h"

#pragma comment(lib, "gts.lib")

std::shared_mutex mtx;

int _stdcall Connect(short Channel, short Param)
{
	auto ret = GTN_Open(Channel, Param);

	// 总线初始化
	ret = GTN_InitEcatComm(1);
	if (ret)
	{
		printf("ec communication error\n");
		return -1;
	}
	short status;
	do 
	{
		ret = GTN_IsEcatReady(1, &status);
	} while (status != 1 || ret != 0);
	// 启动总线通讯
	ret = GTN_StartEcatComm(1);
	ret = GTN_Reset(1);
	return 0;
}

int _stdcall Disconnect()
{
	auto ret = GTN_TerminateEcatComm(1);
	ret = GTN_Close();
	return 0;
}

short _stdcall WritePdo(uint8_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t Size, const void* Data)
{
	if (Data == nullptr || Size == 0)
	{
		return -1;
	}
	auto offset = GetOffsetByIndex(Slave, Index, SubIndex);
	auto p = const_cast<unsigned char*>(static_cast<const unsigned char*>(Data));
	std::unique_lock<std::shared_mutex> lock(mtx);
	//auto ret = GTN_SetEcatRawData(1, offset, Size, p);
	auto ret = GTN_SetEcatSlavePdo(1, Slave, Index, SubIndex, p, Size);
	return ret;
}

short _stdcall ReadPdo(uint8_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t Size, void* Data)
{
	auto offset = GetOffsetByIndex(Slave, Index, SubIndex);
	std::shared_lock<std::shared_mutex> lock(mtx);
	/*auto ret = GTN_GetEcatRawData(1, offset,
		static_cast<unsigned short>(Size),
		static_cast<unsigned char*>(Data));*/
	auto ret = GTN_GetEcatSlavePdo(1, Slave, Index, SubIndex, static_cast<unsigned char*>(Data), Size);
	return ret;
}
