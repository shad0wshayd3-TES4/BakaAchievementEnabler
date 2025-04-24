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
				target.write_fill<OFF>(REL::NOP, SIZE);
			}
		};

		static void InstallNOP()
		{
			hkNop<0x49991C0, 0x09C, 0x0D>::Install();  // cmp rbx+2B1, r13b
			hkNop<0x499AE20, 0x000, 0x09>::Install();  // mov rcx+2B0, 0x101
			hkNop<0x499E550, 0x000, 0x10>::Install();  // mov rcx+2B1, r8b
		}

		static void InstallXOR()
		{
			// quicksave/autosave +0xED; movzx eax, rbx+2B1
			static REL::Relocation target{ REL::Offset(0x48CC9E0) };
			target.write<0xED>({ 0x31, 0xC0, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}

		static void InstallNoMsg()
		{
			static REL::Relocation target{ REL::Offset(0x497F950) };
			target.write_fill<0x19C>(REL::NOP, 0x34);
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
