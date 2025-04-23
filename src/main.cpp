#include "xinput/xinput.h"

namespace REV
{
	static void safe_fill(std::uintptr_t a_dst, std::uint8_t a_value, std::size_t a_count)
	{
		std::uint32_t protect{ 0 };
		bool success = REX::W32::VirtualProtect(reinterpret_cast<void*>(a_dst), a_count, REX::W32::PAGE_EXECUTE_READWRITE, std::addressof(protect));
		if (success)
		{
			std::fill_n(reinterpret_cast<std::uint8_t*>(a_dst), a_count, a_value);
			success = REX::W32::VirtualProtect(reinterpret_cast<void*>(a_dst), a_count, protect, std::addressof(protect));
		}

		assert(success);
	}
}

namespace Hooks
{
	namespace
	{
		template <std::uintptr_t ID, std::ptrdiff_t OFF, std::size_t SIZE>
		class hkNop
		{
		public:
			static void Install()
			{
				static REL::Relocation target{ REL::Offset(ID) };
				REV::safe_fill(target.address() + OFF, REL::NOP, SIZE);
			}
		};

		void EnableAchievements()
		{
			hkNop<0x49991C0, 0x09C, 0x0D>::Install();  // cmp rbx+2B1, r13b
			hkNop<0x499AE20, 0x000, 0x09>::Install();  // mov rcx+2B0, 0x101
			hkNop<0x499E550, 0x000, 0x10>::Install();  // mov rcx+2B1, r8b
		}

		void PreventBreakingAchievements()
		{
			// quicksave/autosave +0xED; movzx eax, rbx+2B1
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
				""
				"Baka Achievement Enabler has been DISABLED.\n"
				""
				""
				"This version of the mod (1.1) was built for game version {}.\n"
				""
				""
				"You are running a later version of the game: {}\n\n"
				""
				""
				"You can continue playing, but achievements may not unlock!\n"
				""
				""
				"Check the mod page for an update!"
				"",
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
		Hooks::Install();
		break;
	case 2:  // DLL_THREAD_ATTACH
	case 3:  // DLL_THREAD_DETACH
	case 0:  // DLL_PROCESS_DETACH
		break;
	}

	return 1;
}
