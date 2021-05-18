#include "pch.h"
#include "DHGPGPUGetAmpl.h"

dholo::gpgpu::DHGPGPUGetAmpl::DHGPGPUGetAmpl(const img::DHImgLoader& img)
{
	DHGPGPUGetPrmtrs::DHGPGPUGetPrmtrs(img);
	m_ProgramPath = "AMPL.cl";
	m_KernelName = "AMPL";
}
