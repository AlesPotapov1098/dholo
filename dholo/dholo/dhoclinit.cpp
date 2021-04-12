#include "stdafx.h"
#include "dhoclinit.h"

DHOCLHard::DHOCLHard(const cl::Platform& platform)
{
	m_Platform = platform;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &m_Devices);
}

std::size_t DHOCLHard::GetCountDevices() const
{
	return m_Devices.size();
}

const cl::Device& DHOCLHard::GetDevice(unsigned int index) const
{
	return m_Devices[index];
}

const cl::Platform& DHOCLHard::GetPlatform() const
{
	return m_Platform;
}

const cl::Device& DHOCLHard::operator[](int index) const
{
	return m_Devices[index];
}

bool DHOCLInit::Init()
{
	cl::vector<cl::Platform> platfroms;
	cl::Platform::get(&platfroms);

	for (auto pl : platfroms)
	{
		DHOCLHard hard(pl);
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

std::wstring DHOCLInit::GetPlatformName(const cl::Platform& pl) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(pl.getInfo<CL_PLATFORM_NAME>());
}

std::wstring DHOCLInit::GetPlatformVendor(const cl::Platform& pl) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(pl.getInfo<CL_PLATFORM_VENDOR>());
}

std::wstring DHOCLInit::GetPlatformVersion(const cl::Platform& pl) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(pl.getInfo<CL_PLATFORM_VERSION>());
}

std::wstring DHOCLInit::GetPlatformExtensions(const cl::Platform& pl) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(pl.getInfo<CL_PLATFORM_EXTENSIONS>());
}

std::wstring DHOCLInit::GetDeviceName(const cl::Device& dev) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(dev.getInfo<CL_DEVICE_NAME>());
}

std::wstring DHOCLInit::GetDeviceVendor(const cl::Device& dev) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(dev.getInfo<CL_DEVICE_VENDOR>());
}

std::wstring DHOCLInit::GetDeviceVersion(const cl::Device& dev) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(dev.getInfo<CL_DEVICE_VERSION>());
}

std::wstring DHOCLInit::GetDeviceExtensions(const cl::Device& dev) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(dev.getInfo<CL_DEVICE_EXTENSIONS>());
}

DHOCLHost::DHOCLHost(const cl::Platform& pl, const cl::Device& dev)
{
	m_Device = dev;
	m_Platform = pl;
}

void DHOCLHost::SetPlatform(const cl::Platform& pl)
{
	m_Platform = pl;
}

void DHOCLHost::SetDevice(const cl::Device& dev)
{
	m_Device = dev;
}

const cl::Platform& DHOCLHost::GetPlatform() const
{
	return m_Platform;
}

const cl::Device& DHOCLHost::GetDevice() const
{
	return m_Device;
}
