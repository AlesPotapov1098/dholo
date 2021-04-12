#pragma once

#include <CL/cl2.h>
#include <codecvt>
#include <string>
#include <vector>

class DHOCLHost
{
public:
	DHOCLHost() = default;
	DHOCLHost(const cl::Platform&, const cl::Device&);
	~DHOCLHost() = default;

	void SetPlatform(const cl::Platform&);
	void SetDevice(const cl::Device&);

	const cl::Platform& GetPlatform() const;
	const cl::Device& GetDevice() const;

private:
	cl::Platform m_Platform;
	cl::Device   m_Device;
};

class DHOCLHard
{
public:
	DHOCLHard() = default;
	DHOCLHard(const cl::Platform&);
	~DHOCLHard() = default;

	std::size_t GetCountDevices() const;
	const cl::Device& GetDevice(unsigned int index) const;
	const cl::Platform& GetPlatform() const;
	const cl::Device& operator[](int index) const;

private:
	cl::Platform m_Platform;
	cl::vector<cl::Device> m_Devices;
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

	std::wstring GetPlatformName(const cl::Platform&) const;
	std::wstring GetPlatformVendor(const cl::Platform&) const;
	std::wstring GetPlatformVersion(const cl::Platform&) const;
	std::wstring GetPlatformExtensions(const cl::Platform&) const;

	std::wstring GetDeviceName(const cl::Device&) const;
	std::wstring GetDeviceVendor(const cl::Device&) const;
	std::wstring GetDeviceVersion(const cl::Device&) const;
	std::wstring GetDeviceExtensions(const cl::Device&) const;

private:
	cl::vector<DHOCLHard> m_Hardware;
};