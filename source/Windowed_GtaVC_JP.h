#include "WindowedMode.h"

void WindowedMode::InitGtaVCJP()
{
	inst = new WindowedMode(
		GameTitle::GTA_VC_JP,
		0x9B2F18, // gameState
		0x9B18E8, // rsGlobal
		0x7867B0, // d3dDevice
		0xA0CD14, // d3dPresentParams
		0x7867D8, // rwVideoModes
		0x641B70, // RwEngineGetNumVideoModes
		0x641BD0, // RwEngineGetCurrentVideoMode
		0x866638  // FrontEndMenuManager
	);

	// check for ASI loader
	if (inst->rsGlobal->ps && inst->rsGlobal->ps->window) // app window already created
		ShowError("ASI Loader is required for correct operation of this plugin!");

	strcpy_s(inst->windowClassName, "Grand theft auto 3");
	inst->windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(100));

	// Debug: check if somebody already modified memory we want to hook
#ifdef DEBUG
	VerifyMemory("CreateWindow", 0x5FF826, 5, 0x4B52BC63);
	VerifyMemory("InitPresentationParams", 0x65B0E4, 6, 0x5834618C);
	VerifyMemory("InitD3dDevice", 0x65B512, 6, 0xDEF87126);
	VerifyMemory("Options>Resolution coloring", 0x49EC68, 2, 0x5B93799F);
	VerifyMemory("Options>Resolution disabling", 0x499EF0, 2, 0xC3868E8B);
	VerifyMemory("Options>Resolution hook", 0x499969, 5, 0xB6D59749);
#endif

	// patch call to CreateWindowExA
	injector::MakeNOP(0x5FF826, 5);
	injector::MakeCALL(0x5FF826, WindowedMode::InitWindow);

	// just before D3D device is created
	struct Patch_InitPresentationParams
	{
		void operator()(injector::reg_pack& regs)
		{
			*(DWORD*)(0xA0CD34) = regs.ebx; // original action replaced by the patch

			inst->WindowCalculateGeometry();
		}
	}; injector::MakeInline<Patch_InitPresentationParams>(0x65B0E4, 0x65B0EA);

	// just after D3D device has been created
	struct Path_InitD3dDevice
	{
		void operator()(injector::reg_pack& regs)
		{
			*(DWORD*)(0x786BFC) = regs.ebp; // original action replaced by the patch

			inst->InitD3dDevice();
		}
	}; injector::MakeInline<Path_InitD3dDevice>(0x65B512, 0x65B518);

	injector::WriteMemory(0x49EC68, WORD(0xE990), true); // don't gray out resolution in options menu after game started
	injector::MakeNOP(0x499EF0, 2); // don't disable resolution changes in options menu after game started
	struct Patch_ChangeResolution // user selected new resolution in option menu
	{
		void operator()(injector::reg_pack& regs)
		{
			// restore potentially corrupted entry before reading it
			if (!inst->videoModesBackup.empty() && regs.eax < inst->videoModesBackup.size())
				(*inst->rwVideoModes)[regs.eax] = inst->videoModesBackup[regs.eax];

			auto mode = *inst->rwVideoModes + regs.eax;
			inst->WindowResize({ (LONG)mode->width, (LONG)mode->height });
		}
	}; injector::MakeInline<Patch_ChangeResolution>(0x499969);
}
