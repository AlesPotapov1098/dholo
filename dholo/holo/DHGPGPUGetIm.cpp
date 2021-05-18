#include "pch.h"
#include "DHGPGPUGetIm.h"

dholo::gpgpu::DHGPGPUGetIm::DHGPGPUGetIm(const dholo::img::DHImgLoader& img)
{
	DHGPGPUGetPrmtrs::DHGPGPUGetPrmtrs(img);
	m_ProgramPath = "IM.cl";
	m_KernelName = "IM";
}
