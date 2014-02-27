/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define ADK_INTERNAL
extern "C"{
    #include "fwk.h"
    #include "dbg.h"
    #include "coop.h"
}
#include "usbh.h"
#include "ADK.h"

void ADK::adkInit(void){

    ::fwkInit();
    ::coopInit();
    ::usbh_init();
}

void ADK::adkEventProcess(void){
    ::coopYield();
}

void ADK::getUniqueId(uint32_t* ID){

    ::cpuGetUniqId(ID);
}

uint64_t ADK::getUptime(void){	//in ms

    return ::fwkGetUptime();
}

void ADK::adkSetPutchar(adkPutcharF f){

    ::dbgSetPutchar(f);
}

// USB
void ADK::usbStart()
{
    ::usbh_start();
}

void ADK::usbSetAccessoryStringVendor(const char *str)
{
    ::usbh_set_accessory_string_vendor(str);
}

void ADK::usbSetAccessoryStringName(const char *str)
{
    ::usbh_set_accessory_string_name(str);
}

void ADK::usbSetAccessoryStringLongname(const char *str)
{
    ::usbh_set_accessory_string_longname(str);
}

void ADK::usbSetAccessoryStringVersion(const char *str)
{
    ::usbh_set_accessory_string_version(str);
}

void ADK::usbSetAccessoryStringUrl(const char *str)
{
    ::usbh_set_accessory_string_url(str);
}

void ADK::usbSetAccessoryStringSerial(const char *str)
{
    ::usbh_set_accessory_string_serial(str);
}

int ADK::accessoryConnected()
{
    return ::usbh_accessory_connected();
}

int ADK::accessorySend(const void *buf, unsigned int len)
{
    return ::accessory_send(buf, len);
}

int ADK::accessoryReceive(void *buf, unsigned int len)
{
    return ::accessory_receive(buf, len);
}


