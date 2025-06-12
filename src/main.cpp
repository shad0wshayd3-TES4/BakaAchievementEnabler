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
				static REL::Relocation target{ REL::ID(ID) };
				target.write_fill<OFF>(REL::NOP, SIZE);
			}
		};

		static void InstallNOP()
		{
			hkNop<311459, 0x09C, 0x0D>::Install();  // cmp rbx+2B1, r13b
			hkNop<311488, 0x000, 0x09>::Install();  // mov rcx+2B0, 0x101
			hkNop<311537, 0x000, 0x10>::Install();  // mov rcx+2B1, r8b
		}

		static void InstallXOR()
		{
			// quicksave/autosave +0xED; movzx eax, rbx+2B1
			static REL::Relocation target{ REL::ID(308463) };
			target.write<0xED>({ 0x31, 0xC0, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}

		static void InstallNoMsg()
		{
			static REL::Relocation target{ REL::ID(311250) };
			static constexpr auto  TARGET_ADDR{ 0x19C };
			static constexpr auto  TARGET_RETN{ 0x1D0 };
			static constexpr auto  TARGET_FILL{ TARGET_RETN - TARGET_ADDR };
			target.write_fill<TARGET_ADDR>(REL::NOP, TARGET_FILL);
		}
	}

	static void Install()
	{
		InstallNOP();
		InstallXOR();
		InstallNoMsg();
	}
}

namespace
{
	void MessageHandler(OBSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case OBSE::MessagingInterface::kPostLoad:
			Hooks::Install();
			break;
		default:
			break;
		}
	}
}

OBSE_PLUGIN_LOAD(OBSE::LoadInterface* a_obse)
{
	OBSE::Init(a_obse);
	OBSE::GetMessagingInterface()->RegisterListener(MessageHandler);
	return true;
}
