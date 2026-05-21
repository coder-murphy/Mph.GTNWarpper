#pragma once

#ifdef API_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#ifdef  __cplusplus
extern "C" {
#endif
// 链接EC
API int  _stdcall Connect(short Channel = 5, short Param = 1);

// 断开EC链接
API int _stdcall Disconnect();

// 写入Pdo
API short _stdcall WritePdo(uint8_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t Size, const void* Data);

// 读取Pdo
API short _stdcall ReadPdo(uint8_t Slave, uint16_t Index, uint8_t SubIndex, uint8_t Size, void* Data);
#ifdef __cplusplus
}
#endif
