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
#ifndef _ADK_H_
#define _ADK_H_

#include <stdint.h>
typedef void (*adkPutcharF)(char c);

class ADK {
public:

    //generic
        void adkInit(void);
	void adkSetPutchar(adkPutcharF f);
        void adkEventProcess(void); //call this often
	void getUniqueId(uint32_t* id);
	uint64_t getUptime(void);	//in ms

    //USB
	void usbStart();
	void usbSetAccessoryStringVendor(const char *str);
	void usbSetAccessoryStringName(const char *str);
	void usbSetAccessoryStringLongname(const char *str);
	void usbSetAccessoryStringVersion(const char *str);
	void usbSetAccessoryStringUrl(const char *str);
	void usbSetAccessoryStringSerial(const char *str);

    //USB accessory
	int accessoryConnected();
	int accessorySend(const void *buf, unsigned int len);
	int accessoryReceive(void *buf, unsigned int len);
};



#endif


