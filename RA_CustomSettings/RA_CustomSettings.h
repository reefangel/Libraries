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

// Cloud Wifi Attachment
// Max number of characters for username and password is 32
const char WIFI_SSID[] PROGMEM = "testing2"; //  your network SSID (name)
const char WIFI_PASS[] PROGMEM = "a01b02c03d"; // your network password
const char PORTALSERVER[] PROGMEM = "forum.reefangel.com"; // Portal server address
const char MQTTSERVER[] PROGMEM ="forum.reefangel.com"; // MQTT server address
#define MQTTPORT 1883 // MQTT server port

// Cloud authentication
// Max number of characters for username and password is 16
const char CLOUD_USERNAME[] PROGMEM = "cloudtest2"; // your forum username
const char CLOUD_PASSWORD[] PROGMEM = "123456"; // your forum password

#endif  // __RA_CUSTOMSETTINGS_H__
