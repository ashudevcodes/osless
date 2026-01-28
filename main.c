#include <efi/efi.h>
#include <efi/eficon.h>
#include <efi/efidef.h>
#include <efi/efierr.h>
#include <efi/efilib.h>
#include <efi/efiapi.h>
#include <efi/x86_64/efibind.h>

EFI_STATUS EFIAPI efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
  InitializeLib(ImageHandle, SystemTable);
  Print(L"Hola Ashish \r\n");

  EFI_TIME Time;
  EFI_STATUS status = uefi_call_wrapper(RT->GetTime, 2, &Time, NULL);

  if (EFI_ERROR(status)) {
	Print(L"Fail to get time: %r\n", status);
	return status;
  }

  Print(L"Current Second is: %02d\n",(UINT32)Time.Second);
  Print(L"Did you want to quit N/y: ");

  EFI_INPUT_KEY key;

  WaitForSingleEvent(ST->ConIn->WaitForKey, 0);
  EFI_STATUS Status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2,ST->ConIn, &key);

  if (EFI_ERROR(Status)) {
	DEBUG((D_ERROR, "Input: error return from ReadKey %x\n", Status));
	return Status;
  }

  if (key.UnicodeChar == L'y' || key.UnicodeChar == L'Y') {
	Print(L"Shutting down Bye Bye!\r\n");
	uefi_call_wrapper(RT->ResetSystem, 4, EfiResetShutdown, EFI_SUCCESS, 0, NULL);
  } else {
	Print(L"Continue to UEFI Shell.. \r\n");
  }

  return EFI_SUCCESS;
}
