/** @file
  This sample application bases on HelloWorld PCD setting 
  to print "UEFI Hello World!" to the UEFI Console.

  Copyright (c) 2006 - 2008, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials                          
  are licensed and made available under the terms and conditions of the BSD License         
  which accompanies this distribution.  The full text of the license may be found at        
  http://opensource.org/licenses/bsd-license.php                                            

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,                     
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.             

**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PrintLib.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimplePointer.h>
#include <Protocol/GraphicsOutput.h>

#define MOUSE_ERROR_STR L"No mouse! (%08x)\r\n\0"
#define MOUSE_OUT_STR L"x: %04d y: %04d z: %04d lmb: %u rmb: %u\r\n\0"

#define DISPLAY_OUT_STR L"%04u x %04u\r\n"

void UefiMouseInteraction(void)
{
    CHAR16     outStr[128]; 
    EFI_STATUS Status;

    EFI_INPUT_KEY key;

    EFI_SIMPLE_POINTER_PROTOCOL *mouse;
    EFI_SIMPLE_POINTER_STATE     mouseState;
    EFI_EVENT                    events[2];
    UINTN                        recv_event;

    gST->ConOut->OutputString(gST->ConOut, L"Getting mouse...\r\n");

    Status = gBS->LocateProtocol(&gEfiSimplePointerProtocolGuid,
                                 NULL,
                                 (VOID*)&mouse);

    if (Status != EFI_SUCCESS)
    {
        UnicodeSPrint(outStr, 32 * sizeof(CHAR16), MOUSE_ERROR_STR, Status);
        gST->ConOut->OutputString(gST->ConOut, outStr);
        return;
    }

    events[0] = gST->ConIn->WaitForKey;
    events[1] = mouse->WaitForInput;

    while (1)
    {
        gBS->WaitForEvent(2, events, &recv_event);

        switch(recv_event)
        {
            case 0:
                gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
                if (key.ScanCode == 0x17)
                {
                    gST->ConOut->OutputString(gST->ConOut, L"Saw ESC, moving on...\r\n");
                    return;
                }
                break;
            case 1:
                mouse->GetState(mouse, &mouseState);
                UnicodeSPrint(outStr,
                              64 * sizeof(CHAR16),
                              MOUSE_OUT_STR,
                              mouseState.RelativeMovementX / mouse->Mode->ResolutionX,
                              mouseState.RelativeMovementY / mouse->Mode->ResolutionY,
                              mouseState.RelativeMovementZ / mouse->Mode->ResolutionZ,
                              mouseState.LeftButton,
                              mouseState.RightButton);
        
                gST->ConOut->OutputString(gST->ConOut, outStr);
                break;
            default:
                /* What */
                break;
        }
    } 
}

void UefiGraphicsInteraction(void)
{
    CHAR16     outStr[128]; 
    EFI_STATUS Status;

    EFI_INPUT_KEY key;
//    UINTN         keyevent;

    EFI_GRAPHICS_OUTPUT_PROTOCOL         *gop;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
    UINTN                                 infosize;
    UINT32                                nextmode;

    Status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid,
                                 NULL,
                                 (VOID*)&gop);

    if (Status != EFI_SUCCESS)
    {
        gST->ConOut->OutputString(gST->ConOut, L"No GOP Objects!\r\n");
    }

    nextmode = 0;

    do {
        gop->SetMode(gop, nextmode);
        gop->QueryMode(gop,
                       nextmode,
                       &infosize,
                       &info); 

        UnicodeSPrint(outStr,
                      128*sizeof(CHAR16),  
                      DISPLAY_OUT_STR,
                      info->HorizontalResolution,
                      info->VerticalResolution);

        gBS->FreePool(info); 
                
        gST->ConOut->OutputString(gST->ConOut, outStr);

        gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, NULL);
        gST->ConIn->ReadKeyStroke(gST->ConIn, &key);

        nextmode++;
    } while (nextmode < gop->Mode->MaxMode);
}

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS EFIAPI UefiMain (IN EFI_HANDLE         ImageHandle,
                            IN EFI_SYSTEM_TABLE  *SystemTable)
{
    UefiMouseInteraction();
    UefiGraphicsInteraction();

    return EFI_SUCCESS;
}
