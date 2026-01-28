#include <efi/efi.h>
#include <efi/eficon.h>
#include <efi/efidef.h>
#include <efi/efierr.h>
#include <efi/efilib.h>
#include <efi/efiapi.h>
#include <efi/x86_64/efibind.h>

void screen_clear()
{
  uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
}

void say_my_name(CHAR16 *name)
{
  Print(L"Hola %s\r\n",name);
}

void show_current_time()
{
  EFI_TIME current_time;
  EFI_STATUS fetchingTimeStatue = uefi_call_wrapper(RT->GetTime, 2, &current_time,NULL);

  if (EFI_ERROR(fetchingTimeStatue)) {
	Print(L"Fail to get time: %r\n", fetchingTimeStatue);
  }

  Print(L"Current Time: %02d:%02d:%02d\n", 
		current_time.Hour, current_time.Minute, current_time.Second);
}

void show_menu()
{
  say_my_name(L"ashish");
  Print(L"╔════════════════════════════════════════╗\r\n");
  Print(L"║ UEFI Dashboard :P                      ║\r\n");
  Print(L"╠════════════════════════════════════════╣\r\n");
  Print(L"║  [1] System Information                ║\r\n");
  // Print(L"║  [2] Memory Map                        ║\r\n");
  // Print(L"║  [3] EFI Variables                     ║\r\n");
  Print(L"╚════════════════════════════════════════╝\r\n");
  show_current_time();
  Print(L"Press 'q' to exit application or 'y' to shutdown\r\n");
}

EFI_INPUT_KEY hendle_keyboard_key_uefi_logic()
{
  EFI_INPUT_KEY key;
  WaitForSingleEvent(ST->ConIn->WaitForKey, 0);
  EFI_STATUS Status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2,ST->ConIn, &key);

  if (EFI_ERROR(Status)) {
	DEBUG((D_ERROR, "Input: error return from ReadKey %x\n", Status));
  }
  return key;
}

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  screen_clear();
  show_menu();


  for (;;) {
	EFI_INPUT_KEY key = hendle_keyboard_key_uefi_logic();

	switch (key.UnicodeChar){
	  case 'q':
		Print(L"Exit to application..\r\n");
		Exit(EFI_SUCCESS,0,NULL);
		break;

	  case 'y':
		Print(L"Shutting down Bye Bye!\r\n");
		uefi_call_wrapper(RT->ResetSystem, 4, EfiResetShutdown, EFI_SUCCESS, 0, NULL);
		break;

	  default:
		screen_clear();
		show_menu();
		continue;
	};
  };

  return EFI_SUCCESS;
}
