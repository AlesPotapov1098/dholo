#include "stdafx.h"
#include "dhoclinit.h"

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

std::wstring DHOCLInit::GetPlatformName(cl_platform_id pl) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::size_t size = 0;

	cl_int err = clGetPlatformInfo(pl, CL_PLATFORM_NAME, 0, NULL, &size);
	if (err != CL_SUCCESS || size == 0)
		return std::wstring();

	char * info = new char[size];

	err = clGetPlatformInfo(pl, CL_PLATFORM_NAME, size, (void*)info, NULL);
	if (err != CL_SUCCESS)
		return std::wstring();

	return conv.from_bytes(info);
}

std::wstring DHOCLInit::GetPlatformVendor(cl_platform_id pl) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::size_t size = 0;

	cl_int err = clGetPlatformInfo(pl, CL_PLATFORM_VENDOR, 0, NULL, &size);
	if (err != CL_SUCCESS || size == 0)
		return std::wstring();

	char * info = new char[size];

	err = clGetPlatformInfo(pl, CL_PLATFORM_VENDOR, size, (void*)info, NULL);
	if (err != CL_SUCCESS)
		return std::wstring();

	return conv.from_bytes(info);
}

std::wstring DHOCLInit::GetPlatformVersion(cl_platform_id pl) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::size_t size = 0;

	cl_int err = clGetPlatformInfo(pl, CL_PLATFORM_VERSION, 0, NULL, &size);
	if (err != CL_SUCCESS || size == 0)
		return std::wstring();

	char * info = new char[size];

	err = clGetPlatformInfo(pl, CL_PLATFORM_VERSION, size, (void*)info, NULL);
	if (err != CL_SUCCESS)
		return std::wstring();

	return conv.from_bytes(info);
}

std::wstring DHOCLInit::GetPlatformExtensions(cl_platform_id pl) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::size_t size = 0;

	cl_int err = clGetPlatformInfo(pl, CL_PLATFORM_EXTENSIONS, 0, NULL, &size);
	if (err != CL_SUCCESS || size == 0)
		return std::wstring();

	char * info = new char[size];

	err = clGetPlatformInfo(pl, CL_PLATFORM_EXTENSIONS, size, (void*)info, NULL);
	if (err != CL_SUCCESS)
		return std::wstring();

	return conv.from_bytes(info);
}

std::wstring DHOCLInit::GetDeviceName(cl_device_id dev) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::size_t size = 0;

	cl_int err = clGetDeviceInfo(dev, CL_DEVICE_NAME, 0, NULL, &size);
	if (err != CL_SUCCESS || size == 0)
		return std::wstring();

	char * info = new char[size];

	err = clGetDeviceInfo(dev, CL_DEVICE_NAME, size, (void*)info, NULL);
	if (err != CL_SUCCESS)
		return std::wstring();

	return conv.from_bytes(info);
}

std::wstring DHOCLInit::GetDeviceVendor(cl_device_id dev) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::size_t size = 0;

	cl_int err = clGetDeviceInfo(dev, CL_DEVICE_VENDOR, 0, NULL, &size);
	if (err != CL_SUCCESS || size == 0)
		return std::wstring();

	char * info = new char[size];

	err = clGetDeviceInfo(dev, CL_DEVICE_VENDOR, size, (void*)info, NULL);
	if (err != CL_SUCCESS)
		return std::wstring();

	return conv.from_bytes(info);
}

std::wstring DHOCLInit::GetDeviceVersion(cl_device_id dev) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::size_t size = 0;

	cl_int err = clGetDeviceInfo(dev, CL_DEVICE_VERSION, 0, NULL, &size);
	if (err != CL_SUCCESS || size == 0)
		return std::wstring();

	char * info = new char[size];

	err = clGetDeviceInfo(dev, CL_DEVICE_VERSION, size, (void*)info, NULL);
	if (err != CL_SUCCESS)
		return std::wstring();

	return conv.from_bytes(info);
}

std::wstring DHOCLInit::GetDeviceExtensions(cl_device_id dev) const
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;

	std::size_t size = 0;

	cl_int err = clGetDeviceInfo(dev, CL_DEVICE_EXTENSIONS, 0, NULL, &size);
	if (err != CL_SUCCESS || size == 0)
		return std::wstring();

	char * info = new char[size];

	err = clGetDeviceInfo(dev, CL_DEVICE_EXTENSIONS, size, (void*)info, NULL);
	if (err != CL_SUCCESS)
		return std::wstring();

	return conv.from_bytes(info);
}

DHOCLHost::DHOCLHost(cl_platform_id pl, cl_device_id dev)
{
	m_Device = dev;
	m_Platform = pl;
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
