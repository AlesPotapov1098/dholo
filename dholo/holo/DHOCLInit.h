#pragma once

#include <CL/cl.h>
#include <codecvt>
#include <string>
#include <vector>

#include "DHGPGPUExp.h"

namespace dholo
{
	namespace gpgpu
	{
		class DHOCLHost
		{
		public:
			DHOCLHost();
			DHOCLHost(cl_platform_id, cl_device_id);
			~DHOCLHost();

			void SetPlatform(cl_platform_id);
			void SetDevice(cl_device_id);

			cl_platform_id GetPlatform() const;
			cl_device_id GetDevice() const;

		private:
			cl_platform_id m_Platform;
			cl_device_id   m_Device;
		};

		class DHOCLHard
		{
		public:
			DHOCLHard(cl_platform_id);
			~DHOCLHard();

			cl_uint GetCountDevices() const;
			cl_device_id GetDevice(unsigned int index) const;
			cl_platform_id GetPlatform() const;
			cl_device_id operator[](int index) const;

		private:
			cl_platform_id m_Platform;
			cl_device_id* m_Devices;

			cl_uint m_Size;
		};

		class DHOCLInit
		{
		public:
			DHOCLInit() = default;
			~DHOCLInit() = default;

			bool Init();
			std::size_t GetSize() const;
			const DHOCLHard& GetHardware(unsigned int index) const;
			const DHOCLHard& operator[](int index) const;

		private:
			std::vector<DHOCLHard> m_Hardware;
		};

		class DHOCLInfo
		{
		public:
			DHOCLInfo() = default;
			~DHOCLInfo() = default;

			/// <summary>
			/// ?????????? ? ??????? ??? ?????????
			/// </summary>
			/// <param name="pl">????????? ?? ????????? OpenCL</param>
			static std::wstring GetPlatformName(cl_platform_id pl);

			/// <summary>
			/// ?????????? ? ??????? ????????????? ?????????
			/// </summary>
			/// <param name="pl">????????? ?? ????????? OpenCL</param>
			static std::wstring GetPlatformVendor(cl_platform_id pl);

			/// <summary>
			/// ?????????? ? ??????? ?????? ?????????
			/// </summary>
			/// <param name="pl">????????? ?? ????????? OpenCL</param>
			static std::wstring GetPlatformVersion(cl_platform_id pl);

			/// <summary>
			/// ?????????? ? ??????? ?????????? ?????????
			/// </summary>
			/// <param name="pl">????????? ?? ????????? OpenCL</param>
			static std::wstring GetPlatformExtensions(cl_platform_id pl);
			
			/// <summary>
			/// ?????????? ? ??????? ??? ??????????
			/// </summary>
			/// <param name="dev">????????? ?? ?????????? OpenCL</param>
			static std::wstring GetDeviceName(cl_device_id dev);

			/// <summary>
			/// ?????????? ? ??????? ??? ??????????
			/// </summary>
			/// <param name="dev">????????? ?? ?????????? OpenCL</param>
			static std::wstring GetDeviceVendor(cl_device_id dev);

			/// <summary>
			/// ?????????? ? ??????? ????????????? ??????????
			/// </summary>
			/// <param name="dev">????????? ?? ?????????? OpenCL</param>
			static std::wstring GetDeviceVersion(cl_device_id dev);

			/// <summary>
			/// ?????????? ? ??????? ?????????? ??????????
			/// </summary>
			/// <param name="dev">????????? ?? ?????????? OpenCL</param>
			static std::wstring GetDeviceExtensions(cl_device_id dev);

			/// <summary>
			/// ?????????? ? ??????? ??? ??????????
			/// </summary>
			/// <param name="dev">????????? ?? ?????????? OpenCL</param>
			static std::wstring GetDeviceType(cl_device_id dev);

			/// <summary>
			/// ?????????? ? ??????? ??????? ??????????
			/// </summary>
			/// <param name="dev">????????? ?? ?????????? OpenCL</param>
			static std::wstring GetDeviceFrequency(cl_device_id dev);

			/// <summary>
			/// ?????????? ? ??????? ?????? ?????????? ?????? ??????????
			/// </summary>
			/// <param name="dev">????????? ?? ?????????? OpenCL</param>
			static std::wstring GetDeviceGlobalSize(cl_device_id dev);

			/// <summary>
			/// ?????????? ? ??????? ?????? ????????? ?????? ??????????
			/// </summary>
			/// <param name="dev">????????? ?? ?????????? OpenCL</param>
			static std::wstring GetDeviceLocalSize(cl_device_id dev);
			 
			static std::wstring GetPlatformInfo(cl_platform_id pl, cl_platform_info inf);
			static std::wstring GetDeviceInfo(cl_device_id dev, cl_device_info inf);
		};
	}
}