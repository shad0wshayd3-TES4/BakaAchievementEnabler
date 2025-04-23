#include "xinput/xinput.h"

namespace REV
{
	void safe_fill(std::uintptr_t a_dst, std::uint8_t a_value, std::size_t a_count)
	{
		std::uint32_t old{ 0 };
		bool success = REX::W32::VirtualProtect(
			reinterpret_cast<void*>(a_dst),
			a_count,
			0x40,
			std::addressof(old));
		if (success)
		{
			std::fill_n(reinterpret_cast<std::uint8_t*>(a_dst), a_count, a_value);
			success = REX::W32::VirtualProtect(
				reinterpret_cast<void*>(a_dst),
				a_count,
				old,
				std::addressof(old));
		}

		assert(success);
	}
}

namespace Hooks
{
	namespace
	{
		void EnableAchievements()
		{
			static REL::Relocation target{ REL::Offset(0x499AE20) };
			REV::safe_fill(target.address() + 0x00, REL::NOP, 0x09);
		}

		void PreventBreakingAchievements()
		{
			static REL::Relocation target{ REL::Offset(0x48CC9E0) };
			REV::safe_fill(target.address() + 0xED, 0x31, 0x01);
			REV::safe_fill(target.address() + 0xEE, 0xC0, 0x01);
			REV::safe_fill(target.address() + 0xEF, REL::NOP, 0x05);
		}
	}

	static void Install()
	{
		auto ver = REL::GetFileVersion(L"OblivionRemastered-Win64-Shipping.exe"sv);
		if (ver.has_value() && ver.value() > OBSE::RUNTIME_LATEST)
		{
			auto msg = std::vformat(
				"""Baka Achievement Enabler has been DISABLED.\n"""
				"""This version of the mod (1.0) was built for game version {}.\n"""
				"""You are running a later version of the game: {}\n\n"""
				"""You can continue playing, but achievements may not unlock!\n"""
				"""Check the mod page for an update!""",
				std::make_format_args(OBSE::RUNTIME_LATEST, ver.value()));
			
			REX::W32::MessageBoxA(NULL, msg.c_str(), "Baka Achievement Enabler", 0);
			return;
		}
		
		EnableAchievements();
		PreventBreakingAchievements();
	}
}

std::uint32_t DllMain([[maybe_unused]] REX::W32::HMODULE a_inst, std::uint32_t a_reason, void*)
{
	switch (a_reason)
	{
	case 1:  // DLL_PROCESS_ATTACH
		// REX::W32::MessageBoxA(NULL, "Loaded", "Loaded", 0);
		Hooks::Install();
		break;
	case 2:  // DLL_THREAD_ATTACH
	case 3:  // DLL_THREAD_DETACH
	case 0:  // DLL_PROCESS_DETACH
		break;
	}

	return 1;
}
