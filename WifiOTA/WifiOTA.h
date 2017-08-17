/* license-start
 * 
 * Copyright (C) 2016 Crispico Resonate, <http://www.crispico.com/>.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details, at <http://www.gnu.org/licenses/>.
 * 
 * license-end
 */

#ifndef __WIFI_OTA_H__
#define __WIFI_OTA_H__

#define DEBUG_WIFI_OTA

#include <Client.h>
#include <IPAddress.h>
#include <Udp.h>
#include <Arduino.h>

#define FLASH_UPDATE_ADDRESS 0x21000

__attribute__ ((section(".flower_boot")))
unsigned char flowerBootCode[340] = { 0x00, 0x80, 0x00, 0x20, 0xDD, 0x20, 0x00,
		0x00, 0x47, 0x20, 0x00, 0x00, 0x47, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x47, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x47, 0x20, 0x00, 0x00, 0x45, 0x20, 0x00, 0x00, 0x01, 0x00, 0x46,
		0x42, 0x70, 0x47, 0xFE, 0xE7, 0xF0, 0xB5, 0x89, 0xB0, 0x6C, 0x46, 0x94,
		0x46, 0x1D, 0x4B, 0x6D, 0x46, 0xC4, 0xCB, 0xC4, 0xC4, 0xC4, 0xCB, 0xC4,
		0xC4, 0x80, 0x27, 0x44, 0xCB, 0x44, 0xC4, 0x62, 0x46, 0x19, 0x4B, 0x03,
		0x32, 0x9C, 0x68, 0x92, 0x08, 0x64, 0x03, 0x64, 0x0F, 0xA4, 0x00, 0x64,
		0x59, 0x9D, 0x68, 0x5D, 0x68, 0xA6, 0x00, 0x3D, 0x43, 0x5D, 0x60, 0xA4,
		0x08, 0x00, 0x2A, 0x1F, 0xD0, 0x75, 0x1E, 0x28, 0x42, 0x02, 0xD0, 0x11,
		0x4D, 0x1D, 0x80, 0x07, 0xE0, 0x45, 0x08, 0xDD, 0x61, 0x0F, 0x4D, 0x1D,
		0x80, 0x1D, 0x7D, 0xED, 0x07, 0xFC, 0xD5, 0xF4, 0xE7, 0x1D, 0x7D, 0xED,
		0x07, 0xFC, 0xD5, 0x15, 0x1B, 0xAA, 0x42, 0x05, 0xD0, 0x00, 0x2A, 0x03,
		0xD0, 0x80, 0xC9, 0x01, 0x3A, 0x80, 0xC0, 0xF7, 0xE7, 0x07, 0x4D, 0x1D,
		0x80, 0x1D, 0x7D, 0xED, 0x07, 0xFC, 0xD5, 0xDD, 0xE7, 0x09, 0xB0, 0xF0,
		0xBD, 0x34, 0x21, 0x00, 0x00, 0x00, 0x40, 0x00, 0x41, 0x44, 0xA5, 0xFF,
		0xFF, 0x02, 0xA5, 0xFF, 0xFF, 0x04, 0xA5, 0xFF, 0xFF, 0x10, 0x4B, 0x07,
		0xB5, 0x5B, 0x88, 0x10, 0x4A, 0x9A, 0x42, 0x0F, 0xD1, 0x88, 0x20, 0xF7,
		0x22, 0x80, 0x01, 0x0E, 0x49, 0x52, 0x02, 0xFF, 0xF7, 0xA9, 0xFF, 0x01,
		0x23, 0x84, 0x20, 0x5B, 0x42, 0x80, 0x02, 0x01, 0xA9, 0x04, 0x22, 0x01,
		0x93, 0xFF, 0xF7, 0xA0, 0xFF, 0x88, 0x23, 0x9B, 0x01, 0x1A, 0x68, 0x82,
		0xF3, 0x08, 0x88, 0x06, 0x4A, 0x93, 0x60, 0x06, 0x4B, 0x1B, 0x68, 0x18,
		0x47, 0x07, 0xBD, 0xC0, 0x46, 0x40, 0x10, 0x02, 0x00, 0x46, 0x42, 0x00,
		0x00, 0x00, 0x12, 0x02, 0x00, 0x00, 0xED, 0x00, 0xE0, 0x04, 0x22, 0x00,
		0x00, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
		0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
		0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00 };

class WifiOTA {

public:

	void begin(Client *client) {
		this->client = client;
	}

	void getUpdate(const char* url) {
		int port;

		const char* host = url;
		
		char* page = strchr(host, '/');
		*page = '\0'; // break string

		char *p = strchr(host, ':');
		if (p != NULL) {
			*p = '\0';
			p++;
			port = atoi(p);
		}
		
		page++;
		getUpdate(host, port, page);
		
		
	}
	
protected:

	Client *client;

	void getUpdate(const char* address, int port, const char* page) {
		#ifdef DEBUG_WIFI_OTA
		Serial.println("Update command received");
		Serial.println(address);
		Serial.println(port);
		Serial.println(page);
		#endif
		if (!client->connect(address, port)) {
			Serial.println("Unable to connect");
			return;
		}
		Serial.println("Connected");

		client->print("GET /"); client->print(page); client->println(" HTTP/1.1");
		client->print("Host: "); client->println(address);
		client->println("Connection: close");
		client->println();
		client->flush();

		#ifdef DEBUG_WIFI_OTA
		Serial.println("Request sent");
		#endif
		
		bool processHeaders = true;

		const void* flashAddress = (void*) FLASH_UPDATE_ADDRESS;
		const int BUF_SIZE = 0x100;
		int bufPointer = 0;
		char buf[BUF_SIZE];
		unsigned long downloadsize=0;
		unsigned long contentsize=0;
		boolean http_ok=false;

		while (client->connected()) {
			if (client->available()) {
				uint8_t c = client->read();
				if (processHeaders) {
					if (c == '\r') { // skip CR
						continue;
					} else if (c == '\n') {	// line received
						if (bufPointer == 0) { // empty line (end of HTTP header); binary data follows
							processHeaders = false;
						} else { // process header line
							buf[bufPointer] = '\0';
							#ifdef DEBUG_WIFI_OTA
							Serial.print("line: "); Serial.println(buf);
							#endif
							if (strncmp(buf, "HTTP/1.1 200 ", 13) == 0) http_ok=true;
							if (strncmp(buf, "Content-Length: ", 16) == 0)
							{
								for (int a=16;a<bufPointer;a++)
									contentsize=(contentsize*10)+(buf[a]-'0');
							}
							bufPointer = 0;
						}
					} else if (bufPointer < BUF_SIZE - 1) {
						buf[bufPointer++] = c;
					}
				} else { // processHeaders == false
					// write to flash
					buf[bufPointer++] = c;
					downloadsize++;
					if (bufPointer == BUF_SIZE) {
						flashWrite(flashAddress, buf, BUF_SIZE);
						flashAddress += BUF_SIZE;
						bufPointer = 0;
						#ifdef DEBUG_WIFI_OTA
						Serial.print(".");
						#endif
					}
				}
			}
		}
		if (bufPointer > 0) {
			flashWrite(flashAddress, buf, bufPointer);
		}

		client->stop();
		
		#ifdef DEBUG_WIFI_OTA
		Serial.println();
		Serial.print("content size: ");
		Serial.println(contentsize);
		Serial.print("download size: ");
		Serial.println(downloadsize);
		#endif
		
		if (downloadsize==contentsize && http_ok) {
			delay(100);
			#ifdef DEBUG_WIFI_OTA
			Serial.println("\nResetting...\n\n");
			#endif
			NVIC_SystemReset(); // equivalent to this:  *((uint32_t*) 0xE000ED0C) = (uint32_t) 0x05FA0004;
			while (1);
		}
	}

	/**
	 * This code is based on the Arduino bootloader code for writing into flash memory.
	 */
	void flashWrite(const volatile void* address, const volatile void* data, uint32_t size) {
		const uint32_t pageSizes[] = { 8, 16, 32, 64, 128, 256, 512, 1024 };
		const uint32_t PAGE_SIZE = pageSizes[NVMCTRL->PARAM.bit.PSZ];
		const uint32_t ROW_SIZE = PAGE_SIZE * 4;
//      const uint32_t NUM_PAGES = NVMCTRL->PARAM.bit.NVMP;

		size = (size + 3) / 4; // size <-- number of 32-bit integers
		volatile uint32_t* src_addr = (volatile uint32_t*) data;
		volatile uint32_t* dst_addr = (volatile uint32_t*) address;

		// Disable automatic page write
		NVMCTRL->CTRLB.bit.MANW = 1;

		// Do writes in pages
		while (size) {
			// if we are at the beginning of a row, erase it first
			if (((uint32_t) dst_addr & (ROW_SIZE - 1)) == 0) {
				NVMCTRL->ADDR.reg = ((uint32_t) dst_addr) / 2;
				NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY
						| NVMCTRL_CTRLA_CMD_ER;
				while (!NVMCTRL->INTFLAG.bit.READY) {
				}
			}

			// Execute "PBC" Page Buffer Clear
			NVMCTRL->CTRLA.reg =
					NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_PBC;
			while (NVMCTRL->INTFLAG.bit.READY == 0) {
			}

			// Fill page buffer
			uint32_t i;
			for (i = 0; i < (PAGE_SIZE / 4) && size; i++) {
				*dst_addr = *src_addr;
				src_addr++;
				dst_addr++;
				size--;
			}

			// Execute "WP" Write Page
			NVMCTRL->CTRLA.reg = NVMCTRL_CTRLA_CMDEX_KEY | NVMCTRL_CTRLA_CMD_WP;
			while (NVMCTRL->INTFLAG.bit.READY == 0) {
			}
		}

	}

};

#endif
