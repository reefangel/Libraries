/*
 * Copyright 2015 Reef Angel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RA_CUSTOMSETTINGS_H__
#define __RA_CUSTOMSETTINGS_H__

#if defined(ARDUINO_ARCH_SAMD) || defined RA_STAR || defined CLOUD_WIFI

// Portal & Cloud Server
const IPAddress PortalServer(104,36,18,155); // forum.reefangel.com
static byte MQTTServer[] = {104,36,18,211}; // cloud.reefangel.com
const IPAddress WebWizardServer(162,213,37,72); // webwizard.reefangel.com
#define MQTTPORT 1883 // MQTT server port

#if defined(ARDUINO_ARCH_SAMD)
// Cloud Wifi Attachment
// Max number of characters for username and password is 32
static char WIFI_SSID[] = "testing2"; //  your network SSID (name)
static char WIFI_PASS[] = "a01b02c03d"; // your network password
#endif

// Cloud authentication
// Max number of characters for username and password is 16
static char CLOUD_USERNAME[]  = "cloudtest2"; // your forum username
static char CLOUD_PASSWORD[]  = "123456"; // your forum password

// Star board
#define STARPORT	2000 //  internal web server port

#endif

#endif  // __RA_CUSTOMSETTINGS_H__
