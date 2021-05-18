#include "pch.h"
#include "DHGPGPUGetRe.h"

dholo::gpgpu::DHGPGPUGetRe::DHGPGPUGetRe(const img::DHImgLoader& img)
{
	DHGPGPUGetPrmtrs::DHGPGPUGetPrmtrs(img);
	m_ProgramPath = "RE.cl";
	m_KernelName = "RE";
}
