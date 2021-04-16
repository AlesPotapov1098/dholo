#pragma once

#include <CL/cl.h>
#include <codecvt>
#include <string>
#include <vector>

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
	DHOCLHard() = default;
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

	std::wstring GetPlatformName(cl_platform_id) const;
	std::wstring GetPlatformVendor(cl_platform_id) const;
	std::wstring GetPlatformVersion(cl_platform_id) const;
	std::wstring GetPlatformExtensions(cl_platform_id) const;

	std::wstring GetDeviceName(cl_device_id) const;
	std::wstring GetDeviceVendor(cl_device_id) const;
	std::wstring GetDeviceVersion(cl_device_id) const;
	std::wstring GetDeviceExtensions(cl_device_id) const;

	std::wstring GetPlatformInfo(cl_platform_id, cl_platform_info) const;
	std::wstring GetDeviceInfo(cl_device_id, cl_device_info) const;

private:
	std::vector<DHOCLHard> m_Hardware;
};