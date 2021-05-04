#include "pch.h"
#include "DHOCLInit.h"

namespace dholo
{
	namespace ocl
	{
		DHOCLHard::DHOCLHard(cl_platform_id platform)
		{
			m_Platform = platform;
			m_Size = 0;

			cl_int err = clGetDeviceIDs(m_Platform, CL_DEVICE_TYPE_ALL, 0, nullptr, &m_Size);
			if (err != CL_SUCCESS || m_Size == 0)
				m_Devices = nullptr;

			m_Devices = new cl_device_id[m_Size];

			err = clGetDeviceIDs(m_Platform, CL_DEVICE_TYPE_ALL, m_Size, m_Devices, nullptr);
			if (err != CL_SUCCESS)
				m_Devices = nullptr;
		}

		DHOCLHard::~DHOCLHard()
		{
			if (m_Devices && m_Size > 0)
				for (int i = 0; i < m_Size; i++)
					if (m_Devices[i])
						clReleaseDevice(m_Devices[i]);
		}

		cl_uint DHOCLHard::GetCountDevices() const
		{
			return m_Size;
		}

		cl_device_id DHOCLHard::GetDevice(unsigned int index) const
		{
			return m_Devices[index];
		}

		cl_platform_id DHOCLHard::GetPlatform() const
		{
			return m_Platform;
		}

		cl_device_id DHOCLHard::operator[](int index) const
		{
			return m_Devices[index];
		}

		bool DHOCLInit::Init()
		{
			cl_platform_id* platfroms = nullptr;
			cl_uint num_platfroms = 0;

			cl_int err = clGetPlatformIDs(0, platfroms, &num_platfroms);
			if (err != CL_SUCCESS || num_platfroms == 0)
				return false;

			platfroms = new cl_platform_id[num_platfroms];

			err = clGetPlatformIDs(num_platfroms, platfroms, NULL);
			if (err != CL_SUCCESS)
				return false;

			for (int i = 0; i < num_platfroms; i++)
			{
				DHOCLHard hard(platfroms[i]);
				m_Hardware.push_back(hard);
			}

			return true;
		}

		std::size_t DHOCLInit::GetSize() const
		{
			return m_Hardware.size();
		}

		const DHOCLHard& DHOCLInit::GetHardware(unsigned int index) const
		{
			return m_Hardware[index];
		}

		const DHOCLHard& DHOCLInit::operator[](int index) const
		{
			return m_Hardware[index];
		}

		DHOCLHost::DHOCLHost()
		{
			m_Platform = NULL;
			m_Device = NULL;
		}

		DHOCLHost::DHOCLHost(cl_platform_id pl, cl_device_id dev)
		{
			m_Device = dev;
			m_Platform = pl;
		}

		DHOCLHost::~DHOCLHost()
		{
			if (m_Device)
				clReleaseDevice(m_Device);
		}

		void DHOCLHost::SetPlatform(cl_platform_id pl)
		{
			m_Platform = pl;
		}

		void DHOCLHost::SetDevice(cl_device_id dev)
		{
			m_Device = dev;
		}

		cl_platform_id DHOCLHost::GetPlatform() const
		{
			return m_Platform;
		}

		cl_device_id DHOCLHost::GetDevice() const
		{
			return m_Device;
		}

		std::wstring DHOCLInfo::GetPlatformName(cl_platform_id pl)
		{
			return GetPlatformInfo(pl, CL_PLATFORM_NAME);
		}

		std::wstring DHOCLInfo::GetPlatformVendor(cl_platform_id pl)
		{
			return GetPlatformInfo(pl, CL_PLATFORM_VENDOR);
		}

		std::wstring DHOCLInfo::GetPlatformVersion(cl_platform_id pl)
		{
			return GetPlatformInfo(pl, CL_PLATFORM_VERSION);
		}

		std::wstring DHOCLInfo::GetPlatformExtensions(cl_platform_id pl)
		{
			return GetPlatformInfo(pl, CL_PLATFORM_EXTENSIONS);
		}

		std::wstring DHOCLInfo::GetDeviceName(cl_device_id dev)
		{
			return GetDeviceInfo(dev, CL_DEVICE_NAME);
		}

		std::wstring DHOCLInfo::GetDeviceVendor(cl_device_id dev)
		{
			return GetDeviceInfo(dev, CL_DEVICE_VENDOR);
		}

		std::wstring DHOCLInfo::GetDeviceVersion(cl_device_id dev)
		{
			return GetDeviceInfo(dev, CL_DEVICE_VERSION);
		}

		std::wstring DHOCLInfo::GetDeviceExtensions(cl_device_id dev)
		{
			return GetDeviceInfo(dev, CL_DEVICE_EXTENSIONS);
		}

		std::wstring DHOCLInfo::GetDeviceType(cl_device_id dev)
		{
			std::size_t size = 0;
			cl_device_type* type = nullptr;
			cl_int err = clGetDeviceInfo(dev, CL_DEVICE_TYPE, 0, NULL, &size);

			type = new cl_device_type();

			err = clGetDeviceInfo(dev, CL_DEVICE_TYPE, size, (void*)type, NULL);

			switch (*type)
			{
			case CL_DEVICE_TYPE_CPU:
				return L"CPU";

			case CL_DEVICE_TYPE_GPU:
				return L"GPU";

			default:
				return L"NONE";
			}
		}

		std::wstring DHOCLInfo::GetPlatformInfo(cl_platform_id pl, cl_platform_info inf)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

			std::size_t size = 0;

			cl_int err = clGetPlatformInfo(pl, inf, 0, NULL, &size);
			if (err != CL_SUCCESS || size == 0)
				return std::wstring();

			char* info = new char[size];

			err = clGetPlatformInfo(pl, inf, size, (void*)info, NULL);
			if (err != CL_SUCCESS)
				return std::wstring();

			return conv.from_bytes(info);
		}

		std::wstring DHOCLInfo::GetDeviceInfo(cl_device_id dev, cl_device_info inf)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
			std::size_t size = 0;

			cl_int err = clGetDeviceInfo(dev, inf, 0, NULL, &size);
			if (err != CL_SUCCESS || size == 0)
				return std::wstring();

			char* info = new char[size];

			err = clGetDeviceInfo(dev, inf, size, (void*)info, NULL);
			if (err != CL_SUCCESS)
				return std::wstring();

			return conv.from_bytes(info);
		}
	}
}