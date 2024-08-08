#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>
#include <thread>
#include <iostream>
#include <sstream>

namespace offset
{
	const DWORD localPlayer = 0x10F4F4;
	const DWORD entityList = 0x10F4F8;

	const DWORD healthOffset = 0xF8;
	const DWORD armorOffset = 0xFC;
	const DWORD speedOffset = 0x80;
	const DWORD jumpPowerOffset = 0x18;

	const DWORD arAmmoOffset = 0x150;
	const DWORD smgAmmoOffset = 0x148;
	const DWORD shotgunAmmoOffset = 0x144;
	const DWORD marksmanAmmoOffset = 0x140;
	const DWORD akimboAmmoOffset = 0x15C;

	const DWORD sniperAmmoOffset = 0x14C;
	const DWORD pistolAmmoOffset = 0x13C;
	const DWORD fastPistolAmmoOffset = 0x15C;
	const DWORD grenadesOffset = 0x158;

	const DWORD sniperTimerOffset = 0x174;
	const DWORD knifeTimerOffset = 0x160;
	const DWORD carbineTimerOffset = 0x168;
	const DWORD shotgunTimerOffset = 0x16C;
	const DWORD machinegunTimerOffset = 0x170;
	const DWORD pistolTimerOffset = 0x164;
	const DWORD arTimerOffset = 0x178;
	const DWORD grenadeTimerOffset = 0x180;

	const DWORD canShotOffset = 0x70;
}

// hack fn
void InfAmmo(const HMODULE instance) noexcept
{
	const auto client = reinterpret_cast<std::uintptr_t>(GetModuleHandle("ac_client.exe"));

	const auto localPlayer = *reinterpret_cast<std::uintptr_t*>(client + offset::localPlayer);


	// hack loop

	while (!GetAsyncKeyState(VK_END))
	{
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::knifeTimerOffset) = 0;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::sniperTimerOffset) = 0;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::arTimerOffset) = 0;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::pistolTimerOffset) = 0;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::grenadeTimerOffset) = 0;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::machinegunTimerOffset) = 0;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::shotgunTimerOffset) = 0;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::carbineTimerOffset) = 0;

		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::healthOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::armorOffset) = 1337;

		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::arAmmoOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::pistolAmmoOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::fastPistolAmmoOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::sniperAmmoOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::grenadesOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::shotgunAmmoOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::smgAmmoOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::marksmanAmmoOffset) = 1337;
		*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::akimboAmmoOffset) = 1337;


		

		if (GetAsyncKeyState('W') & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000) {
			*reinterpret_cast<std::uintptr_t*>(localPlayer + offset::speedOffset) = 5;
		}
	}

	HWND dummyWindow = CreateWindowEx(
		0,                      // Optional window styles
		TEXT("STATIC"),         // Window class
		TEXT("Dummy"),          // Window text
		WS_POPUP,               // Window style
		0, 0, 0, 0,             // Size and position
		nullptr,                // Parent window
		nullptr,                // Menu
		nullptr,                // Instance handle
		nullptr                 // Additional application data
	);

	if (dummyWindow)
	{
		SetForegroundWindow(dummyWindow);

		MessageBoxA(dummyWindow, "Uninjected successfully.", "Uninject", MB_OK | MB_ICONINFORMATION);

		DestroyWindow(dummyWindow); // Cleanup dummy window
	}


	// uninject
	FreeLibraryAndExitThread(instance, 0);
}


unsigned char new_bytes[3] = { 0xDD, 0xD8, 0x90 };

// entry point
int __stdcall DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	DWORD old_protect;
	unsigned char* hook_location = (unsigned char*)0x45BAAD;

	if (reason == DLL_PROCESS_ATTACH) {
		VirtualProtect((void*)hook_location, 3, PAGE_EXECUTE_READWRITE, &old_protect);
		for (int i = 0; i < sizeof(new_bytes); i++) {
			*(hook_location + i) = new_bytes[i];
		}
	}

	// DLL_PROCESS_ATTACH
	if (reason == 1)
	{
		DisableThreadLibraryCalls(instance);

		// create hack thread
		const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(InfAmmo), instance, 0, nullptr);

		if (thread)
		{
			CloseHandle(thread);
		}
	}

	return 1;
}