/**
 *  Sentinel Software GmbH
 *  Copyright (C) 2022 Steven Inacio
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/**
 * @addtogroup CharonUDS
 * @{
 * @defgroup BusinessLogic Business Logic
 * @{
 * @file  uds_InputOutputControlFunctionalUnit.c.h
 * 
 *
 * $Id:  $
 * $URL:  $
 * @}
 * @}
 * 
 */
/*****************************************************************************/

/* Includes ******************************************************************/

#include "charon_InputOutputControlFunctionalUnit.h"
#include "HSDI/charon_interface_debug.h"
#include "ComLogic/charon_SessionAndSerivceControl.h"
#include "Common/charon_negativeResponse.h"
#include <stdint.h>
#include <string.h>
/* Imports *******************************************************************/

/* Constants *****************************************************************/

/* Macros ********************************************************************/

#define IO_CTRL_RETURN_CONTROL_TO_ECU 0x00
#define IO_CTRL_RESET_TO_DEFAULT 0x01
#define IO_CTRL_FREEZE_CURRENT_STATE 0x02
#define IO_CTRL_SHORT_TERM_ADJUSTMENT 0x03

#if STATIC_IO_CONTROL == 1
IO_DID_Array_Entry_t *findDID(uint16_t DID);
#elif DYNAMIC_IO_CONTROL == 1
IO_DID_List_Entry_t *findDID(uint16_t DID);
#else
#error "You must use either static or dynamic memory location for io control."
#endif
/* Types *********************************************************************/

/* Variables *****************************************************************/

static IO_DID_List_Entry_t *io_control_list;

/* Private Function Definitions **********************************************/

static uds_responseCode_t handleFlag(IO_DID_List_Entry_t* did_entry, uint16_t currentMaskPosition, uint8_t receivedCommand, const uint8_t *data, uint32_t data_length);

static uds_responseCode_t handleNegative(char *message, uint16_t DID, uint16_t currentMaskPosition, uds_responseCode_t responseCode);

/* Interfaces  ***************************************************************/

uds_responseCode_t charon_InputOutputControlFunctionalUnit_InputOutputControlByIdentifier(const uint8_t * receiveBuffer, uint32_t receiveBufferSize)
{
    const uint8_t *saved_states;
    uds_responseCode_t retVal = uds_responseCode_PositiveResponse;
    if (receiveBufferSize < 4)
    {
        CHARON_ERROR("Unexpected message length.");
        charon_sendNegativeResponse(uds_responseCode_IncorrectMessageLengthOrInvalidFormat, uds_sid_InputOutputControlByIdentifier);
        return uds_responseCode_IncorrectMessageLengthOrInvalidFormat;
    }
    // CHECK IO DID
    // Maybe code a helper module that converts byte-arrays to uint16_t and vice versa
    uint16_t DID = receiveBuffer[1] << 8 | receiveBuffer[2];
    IO_DID_List_Entry_t *did_entry = findDID(DID);
    // IS IO DID KNOWN?
    if (NULL == did_entry)
    {
        // NO
        CHARON_WARNING("Could not find the IO DID 0x%04x", DID);
        // SEND NEGATIVE RESPONSE (REQUEST OUT OF RANGE)
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_InputOutputControlByIdentifier);
        // RETURN
        return uds_responseCode_RequestOutOfRange;
    }
    if (4 <= receiveBuffer[3])
    {
        CHARON_WARNING("Value of inputOutputControlParameter (0x%x) is invalid.", receiveBuffer[3]);
        charon_sendNegativeResponse(uds_responseCode_RequestOutOfRange, uds_sid_InputOutputControlByIdentifier);
        return uds_responseCode_RequestOutOfRange;
    }
    uint16_t mask_pointer = 4;
    // SUBFUNCTION USES CONTROL STATES?
    if (IO_CTRL_SHORT_TERM_ADJUSTMENT == receiveBuffer[3])
    {
        // YES
        // SAVE STATES
        saved_states = &receiveBuffer[4];
        // INCREASE MASK POINTER BY STATES SIZE
        mask_pointer += did_entry->data_size;
    }
    else 
    {
        saved_states = NULL;
    }
    // CHECK PRESENCE OF MASK
    const uint8_t *mask_flags;
    uint16_t mask_flags_length = 1;
    if (receiveBufferSize > mask_pointer)
    {
        mask_flags = &receiveBuffer[mask_pointer];
        mask_flags_length = receiveBufferSize - mask_pointer + 1;
    }
    else {
        mask_flags = (uint8_t[]){0x01};
    }
    uint16_t currentMaskPosition = 1;
    uint8_t currentFlag;
    for (uint16_t i = 0; i < mask_flags_length; i++)
    {
        currentFlag = mask_flags[i];
        for (uint8_t j = 0; j < 8; currentFlag <<= 1, j++)
        {
            if ((currentFlag & 0x80) == 0x80)
            {
                retVal = handleFlag(did_entry, currentMaskPosition, receiveBuffer[3], saved_states, did_entry->data_size);
                if (retVal != uds_responseCode_PositiveResponse)
                {
                    return retVal;
                }
            }
            currentMaskPosition++;
        }
    }
    uint8_t message[4 + did_entry->data_size];
    message[0] = uds_sid_InputOutputControlByIdentifier + uds_sid_PositiveResponseMask;
    message[1] = DID >> 8;
    message[2] = DID;
    message[3] = receiveBuffer[3];
    memcpy(&message[4], did_entry->data, did_entry->data_size);
    charon_sscTxMessage(message, 4 + did_entry->data_size);
    return retVal;
}

#if STATIC_IO_CONTROL == 1
IO_DID_Array_Entry_t *findDID(uint16_t DID)
{
    for (int i = 0; i < current_io_control_list_length; i++)
    {
        if (DID == io_control_list[i].DID)
        {
            return &io_control_list[i];
        }
    }
    return NULL;
}
IO_Ctrl_Object_t *findMaskObject(IO_DID_Array_Entry_t *did_object, uint16_t position)
{
    for (int i = 0; i < did_object->)
}
#elif DYNAMIC_IO_CONTROL == 1
bool charon_InputOutputControlFunctionalUnit_addIOEntry(IO_DID_List_Entry_t *entry) {
    if(io_control_list == NULL) {
        io_control_list = entry;
        return true;
    }
    IO_DID_List_Entry_t *nextEntry = io_control_list;
    // To limit the user from adding too many entries a counter could be introduced here.
    // When the counter becomes greater than a threshold the function could return false then.
    while(nextEntry->next != NULL) {
        nextEntry = nextEntry->next;
    }
    nextEntry->next = entry;
    return true;
}

IO_DID_List_Entry_t* charon_InputOutputControlFunctionalUnit_removeIOEntry(uint16_t DID) {
    IO_DID_List_Entry_t *retVal = NULL;
    if(io_control_list->DID == DID) {
        retVal = io_control_list;
        io_control_list = (io_control_list->next == NULL) ? NULL : io_control_list->next;
        return retVal;
    }
    IO_DID_List_Entry_t *nextEntry = io_control_list;
    while(nextEntry->next != NULL) {
        if(nextEntry->next->DID == DID) {
            retVal = nextEntry->next;
            nextEntry->next = (nextEntry->next->next == NULL) ? NULL : nextEntry->next->next;
            return retVal;
        }
        nextEntry = nextEntry->next;
    }
    return NULL;
}

IO_DID_List_Entry_t *findDID(uint16_t DID)
{
    IO_DID_List_Entry_t *currentHead = io_control_list;
    while (currentHead != NULL)
    {
        if (currentHead->DID == DID)
            return currentHead;
        currentHead = currentHead->next;
    }
    return NULL;
}

IO_Ctrl_Object_t *findMaskObject(IO_DID_List_Entry_t *did_object, uint16_t position)
{
    IO_Mask_Entry_t *currentHead = did_object->io_mask_list;
    while (currentHead != NULL)
    {
        if (currentHead->position == position)
        {
            return &currentHead->ioObject;
        }
    }
    return NULL;
}

bool charon_InputOutputControlFunctionalUnit_clearIOEntries() {
    io_control_list = NULL;
    return true;
}
#endif

/* Private Function **********************************************************/

static uds_responseCode_t handleFlag(IO_DID_List_Entry_t* did_entry, uint16_t currentMaskPosition, uint8_t receivedCommand, const uint8_t *data, uint32_t data_length)
{
    // IO OBJECT AT MASK FLAG?
    IO_Ctrl_Object_t *ctrl_object = findMaskObject(did_entry, currentMaskPosition);
    if (ctrl_object == NULL)
    {
        // NOT PRESENT
        return handleNegative("IO DID 0x%04x does not have an IO Control Object at position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_RequestOutOfRange);
    }
    // TODO: How can we simplify this?
    switch (receivedCommand)
    {
    case IO_CTRL_RETURN_CONTROL_TO_ECU:
        if (ctrl_object->returnControlToEcu == NULL)
        {
            return handleNegative("Could not find \"Return Control to ECU\" function of DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_RequestOutOfRange);
        }
        else if (!ctrl_object->returnControlToEcu())
        {
            return handleNegative("Couldn't return control to ECU of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_ConditionsNotCorrect);
        }
    case IO_CTRL_RESET_TO_DEFAULT:
        if (ctrl_object->resetToDefault == NULL)
        {
            return handleNegative("Could not find \"Reset to Default\" function of DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_RequestOutOfRange);
        }
        else if (!ctrl_object->resetToDefault())
        {
            return handleNegative("Couldn't reset the state of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_ConditionsNotCorrect);
        }
    case IO_CTRL_FREEZE_CURRENT_STATE:
        if (ctrl_object->freezeCurrentState == NULL)
        {
            return handleNegative("Could not find \"Freeze Current State \" function of DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_RequestOutOfRange);
        }
        else if (!ctrl_object->freezeCurrentState())
        {
            return handleNegative("Couldn't freeze the state of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_ConditionsNotCorrect);
        }
    case IO_CTRL_SHORT_TERM_ADJUSTMENT:
        if (ctrl_object->shortTermAdjustment == NULL)
        {
            return handleNegative("Could not find \"Short Term Adjustment\" function of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_RequestOutOfRange);
        }
        else if (!ctrl_object->shortTermAdjustment(data, data_length))
        {
            return handleNegative("Couldn't adjust the state of IO DID 0x%04x at mask position %d.", did_entry->DID, currentMaskPosition, uds_responseCode_ConditionsNotCorrect);
        }
    default:
        return uds_responseCode_RequestOutOfRange;
    }

}

static uds_responseCode_t handleNegative(char *message, uint16_t DID, uint16_t currentMaskPosition, uds_responseCode_t responseCode)
{
    // CHARON_WARNING(message, DID, currentMaskPosition); /< @todo (spike) Wut? (<_< )
    charon_sendNegativeResponse(responseCode, uds_sid_InputOutputControlByIdentifier);
    return responseCode;
}

/*---************** (C) COPYRIGHT Sentinel Software GmbH *****END OF FILE*---*/
