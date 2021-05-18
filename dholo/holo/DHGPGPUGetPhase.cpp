#include "pch.h"
#include "DHGPGPUGetPhase.h"

dholo::gpgpu::DHGPGPUGetPhase::DHGPGPUGetPhase(const img::DHImgLoader& img)
{
	DHGPGPUGetPrmtrs::DHGPGPUGetPrmtrs(img);
	m_ProgramPath = "PHASE.cl";
	m_KernelName = "PHASE";
}
