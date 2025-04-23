#pragma once

#define XINPUT_LOAD_FROM_DLL(NAME)                                              \
	using func_t = decltype(&NAME);                                             \
	auto hndl = REX::W32::LoadLibraryA("C:\\Windows\\System32\\xinput1_3.dll"); \
	auto func = reinterpret_cast<func_t>(REX::W32::GetProcAddress(hndl, #NAME));

extern "C" std::uint32_t XInputGetState(std::uint32_t dwUserIndex, void* pState)
{
	XINPUT_LOAD_FROM_DLL(XInputGetState);
	return func(dwUserIndex, pState);
}

extern "C" std::uint32_t XInputSetState(std::uint32_t dwUserIndex, void* pState)
{
	XINPUT_LOAD_FROM_DLL(XInputSetState);
	return func(dwUserIndex, pState);
}

extern "C" std::uint32_t XInputGetCapabilities(std::uint32_t dwUserIndex, std::uint32_t dwFlags, void* pCapabilities)
{
	XINPUT_LOAD_FROM_DLL(XInputGetCapabilities);
	return func(dwUserIndex, dwFlags, pCapabilities);
}

extern "C" void XInputEnable(REX::W32::BOOL bEnable)
{
	XINPUT_LOAD_FROM_DLL(XInputEnable);
	return func(bEnable);
}

extern "C" std::uint32_t XInputGetBatteryInformation(std::uint32_t dwUserIndex, std::uint8_t devType, void* pBatteryInformation)
{
	XINPUT_LOAD_FROM_DLL(XInputGetBatteryInformation);
	return func(dwUserIndex, devType, pBatteryInformation);
}

extern "C" std::uint32_t XInputGetKeystroke(std::uint32_t dwUserIndex, std::uint32_t dwReserved, void* pKeystroke)
{
	XINPUT_LOAD_FROM_DLL(XInputGetKeystroke);
	return func(dwUserIndex, dwReserved, pKeystroke);
}

extern "C" std::uint32_t XInputGetDSoundAudioDeviceGuids(std::uint32_t dwUserIndex, void* pDSoundRenderGrid, void* pDSoundCaptureGrid)
{
	XINPUT_LOAD_FROM_DLL(XInputGetDSoundAudioDeviceGuids);
	return func(dwUserIndex, pDSoundRenderGrid, pDSoundCaptureGrid);
}

#undef XINPUT_LOAD_FROM_DLL
