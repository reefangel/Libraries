/*
 * Copyright 2010 Reef Angel / Roberto Imai
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

/*
 * Updated by:  Curt Binder
 * Updates Released under Apache License, Version 2.0
 */

#include "RA_PCA9685.h"

RA_PCA9685::RA_PCA9685(byte address)
{
  LightsOverride = false;
  _address = address;
  _lastWrite = millis();

  for (byte a = 0; a < 16; a++)
  {
    PWMChannel[a] = 0;
  }
}

void RA_PCA9685::SendData(byte Channel, byte Data)
{
  if (Channel < 16) PWMChannel[Channel] = Data;
  int newdata = get12BitLuminanceValue(Data);

  Wire.beginTransmission(_address);
  Wire.write(0x8 + (4 * Channel));
  Wire.write(newdata & 0xff);
  Wire.write(newdata >> 8);
  Wire.endTransmission();
}

void RA_PCA9685::SetAllChannelsPercent(byte p)
{
  for (byte a = 0; a < 16; a++)
  {
    PWMChannel[a] = p;
  }
}

void RA_PCA9685::SetChannelPercent(byte Channel, byte p)
{
  PWMChannel[Channel] = p;
}

void RA_PCA9685::SetPercentForChannels(int Channels, byte p)
{
  for (byte i = 0; i < 16; i++)
  {
    if (bitRead(Channels, i))
    {
      PWMChannel[i] = p;
    }
  }
}

void RA_PCA9685::SetSlopesForChannels(int Channels, uint8_t Start, uint8_t End, uint8_t Duration)
{
  for (int i = 0; i < 16; i++)
  {
    if (bitRead(Channels, i))
    {
      ChannelPWMSlope(i, Start, End, Duration);
    }
  }
}

void RA_PCA9685::SetSlopesForChannels(uint8_t onHour, uint8_t onMinute, uint8_t offHour, uint8_t offMinute, int Channels, uint8_t Start, uint8_t End, uint8_t Duration)
{
  for (int i = 0; i < 16; i++)
  {
    if (bitRead(Channels, i))
    {
      ChannelPWMSlope(onHour, onMinute, offHour, offMinute, i, Start, End, Duration);
    }
  }
}

void RA_PCA9685::SetSlopesForChannels(int Channels, uint8_t Start, uint8_t End, uint8_t Duration, uint8_t MinuteOffset)
{
  for (int i = 0; i < 16; i++)
  {
    if (bitRead(Channels, i))
    {
      ChannelPWMSlope(i, Start, End, Duration, MinuteOffset);
    }
  }
}

void RA_PCA9685::SetParabolasForChannels(int Channels, uint8_t Start, uint8_t End)
{
  for (int i = 0; i < 16; i++)
  {
    if (bitRead(Channels, i))
    {
      ChannelPWMParabola(i, Start, End);
    }
  }
}

void RA_PCA9685::SetParabolasForChannels(uint8_t onHour, uint8_t onMinute, uint8_t offHour, uint8_t offMinute, int Channels, uint8_t Start, uint8_t End)
{
  for (int i = 0; i < 16; i++)
  {
    if (bitRead(Channels, i))
    {
      ChannelPWMParabola(onHour, onMinute, offHour, offMinute, i, Start, End);
    }
  }
}

void RA_PCA9685::SetParabolasForChannels(int Channels, uint8_t Start, uint8_t End, uint8_t MinuteOffset)
{
  for (int i = 0; i < 16; i++)
  {
    if (bitRead(Channels, i))
    {
      ChannelPWMParabola(i, Start, End, MinuteOffset);
    }
  }
}

void RA_PCA9685::Write()
{
  if (millis() - _lastWrite > 1000)
  {
    _lastWrite = millis();

    // setup PCA9685 for data receive
    // we need this to make sure it will work if connected ofter controller is booted, so we need to send it all the time.
    Wire.beginTransmission(_address);
    Wire.write(0);
    Wire.write(0xa1);
    Wire.endTransmission();

    for (byte a = 0; a < 16; a++)
    {
      SendData(a, PWMChannel[a]);
    }
  }
}

byte RA_PCA9685::GetChannelValue(byte Channel)
{
  if (PWMChannelOverride[Channel] < 100)
  {
    return PWMChannelOverride[Channel];
  }
  else
  {
    return PWMChannel[Channel];
  }
}

void RA_PCA9685::Channel0PWMSlope()
{
  ChannelPWMSlope(0, InternalMemory.PWMSlopeStart0_read(), InternalMemory.PWMSlopeEnd0_read(), InternalMemory.PWMSlopeDuration0_read());
}

void RA_PCA9685::Channel1PWMSlope()
{
  ChannelPWMSlope(1, InternalMemory.PWMSlopeStart1_read(), InternalMemory.PWMSlopeEnd1_read(), InternalMemory.PWMSlopeDuration1_read());
}

void RA_PCA9685::Channel2PWMSlope()
{
  ChannelPWMSlope(2, InternalMemory.PWMSlopeStart2_read(), InternalMemory.PWMSlopeEnd2_read(), InternalMemory.PWMSlopeDuration2_read());
}

void RA_PCA9685::Channel3PWMSlope()
{
  ChannelPWMSlope(3, InternalMemory.PWMSlopeStart3_read(), InternalMemory.PWMSlopeEnd3_read(), InternalMemory.PWMSlopeDuration3_read());
}

void RA_PCA9685::Channel4PWMSlope()
{
  ChannelPWMSlope(4, InternalMemory.PWMSlopeStart4_read(), InternalMemory.PWMSlopeEnd4_read(), InternalMemory.PWMSlopeDuration4_read());
}

void RA_PCA9685::Channel5PWMSlope()
{
  ChannelPWMSlope(5, InternalMemory.PWMSlopeStart5_read(), InternalMemory.PWMSlopeEnd5_read(), InternalMemory.PWMSlopeDuration5_read());
}

void RA_PCA9685::Channel6PWMSlope()
{
  ChannelPWMSlope(6, InternalMemory.PWMSlopeStart6_read(), InternalMemory.PWMSlopeEnd6_read(), InternalMemory.PWMSlopeDuration6_read());
}

void RA_PCA9685::Channel7PWMSlope()
{
  ChannelPWMSlope(7, InternalMemory.PWMSlopeStart7_read(), InternalMemory.PWMSlopeEnd7_read(), InternalMemory.PWMSlopeDuration7_read());
}

void RA_PCA9685::Channel8PWMSlope()
{
  ChannelPWMSlope(8, InternalMemory.PWMSlopeStart8_read(), InternalMemory.PWMSlopeEnd8_read(), InternalMemory.PWMSlopeDuration8_read());
}

void RA_PCA9685::Channel9PWMSlope()
{
  ChannelPWMSlope(9, InternalMemory.PWMSlopeStart9_read(), InternalMemory.PWMSlopeEnd9_read(), InternalMemory.PWMSlopeDuration9_read());
}

void RA_PCA9685::Channel10PWMSlope()
{
  ChannelPWMSlope(10, InternalMemory.PWMSlopeStart10_read(), InternalMemory.PWMSlopeEnd10_read(), InternalMemory.PWMSlopeDuration10_read());
}

void RA_PCA9685::Channel11PWMSlope()
{
  ChannelPWMSlope(11, InternalMemory.PWMSlopeStart11_read(), InternalMemory.PWMSlopeEnd11_read(), InternalMemory.PWMSlopeDuration11_read());
}

void RA_PCA9685::Channel12PWMSlope()
{
  ChannelPWMSlope(12, InternalMemory.PWMSlopeStart12_read(), InternalMemory.PWMSlopeEnd12_read(), InternalMemory.PWMSlopeDuration12_read());
}

void RA_PCA9685::Channel13PWMSlope()
{
  ChannelPWMSlope(13, InternalMemory.PWMSlopeStart13_read(), InternalMemory.PWMSlopeEnd13_read(), InternalMemory.PWMSlopeDuration13_read());
}

void RA_PCA9685::Channel14PWMSlope()
{
  ChannelPWMSlope(14, InternalMemory.PWMSlopeStart14_read(), InternalMemory.PWMSlopeEnd14_read(), InternalMemory.PWMSlopeDuration14_read());
}

void RA_PCA9685::Channel15PWMSlope()
{
  ChannelPWMSlope(15, InternalMemory.PWMSlopeStart15_read(), InternalMemory.PWMSlopeEnd15_read(), InternalMemory.PWMSlopeDuration15_read());
}

void RA_PCA9685::Channel0PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(0, InternalMemory.PWMSlopeStart0_read(), InternalMemory.PWMSlopeEnd0_read(), InternalMemory.PWMSlopeDuration0_read(), MinuteOffset);
}

void RA_PCA9685::Channel1PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(1, InternalMemory.PWMSlopeStart1_read(), InternalMemory.PWMSlopeEnd1_read(), InternalMemory.PWMSlopeDuration1_read(), MinuteOffset);
}

void RA_PCA9685::Channel2PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(2, InternalMemory.PWMSlopeStart2_read(), InternalMemory.PWMSlopeEnd2_read(), InternalMemory.PWMSlopeDuration2_read(), MinuteOffset);
}

void RA_PCA9685::Channel3PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(3, InternalMemory.PWMSlopeStart3_read(), InternalMemory.PWMSlopeEnd3_read(), InternalMemory.PWMSlopeDuration3_read(), MinuteOffset);
}

void RA_PCA9685::Channel4PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(4, InternalMemory.PWMSlopeStart4_read(), InternalMemory.PWMSlopeEnd4_read(), InternalMemory.PWMSlopeDuration4_read(), MinuteOffset);
}

void RA_PCA9685::Channel5PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(5, InternalMemory.PWMSlopeStart5_read(), InternalMemory.PWMSlopeEnd5_read(), InternalMemory.PWMSlopeDuration5_read(), MinuteOffset);
}

void RA_PCA9685::Channel6PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(6, InternalMemory.PWMSlopeStart6_read(), InternalMemory.PWMSlopeEnd6_read(), InternalMemory.PWMSlopeDuration6_read(), MinuteOffset);
}

void RA_PCA9685::Channel7PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(7, InternalMemory.PWMSlopeStart7_read(), InternalMemory.PWMSlopeEnd7_read(), InternalMemory.PWMSlopeDuration7_read(), MinuteOffset);
}

void RA_PCA9685::Channel8PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(8, InternalMemory.PWMSlopeStart8_read(), InternalMemory.PWMSlopeEnd8_read(), InternalMemory.PWMSlopeDuration8_read(), MinuteOffset);
}

void RA_PCA9685::Channel9PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(9, InternalMemory.PWMSlopeStart9_read(), InternalMemory.PWMSlopeEnd9_read(), InternalMemory.PWMSlopeDuration9_read(), MinuteOffset);
}

void RA_PCA9685::Channel10PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(10, InternalMemory.PWMSlopeStart10_read(), InternalMemory.PWMSlopeEnd10_read(), InternalMemory.PWMSlopeDuration10_read(), MinuteOffset);
}

void RA_PCA9685::Channel11PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(11, InternalMemory.PWMSlopeStart11_read(), InternalMemory.PWMSlopeEnd11_read(), InternalMemory.PWMSlopeDuration11_read(), MinuteOffset);
}

void RA_PCA9685::Channel12PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(12, InternalMemory.PWMSlopeStart12_read(), InternalMemory.PWMSlopeEnd12_read(), InternalMemory.PWMSlopeDuration12_read(), MinuteOffset);
}

void RA_PCA9685::Channel13PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(13, InternalMemory.PWMSlopeStart13_read(), InternalMemory.PWMSlopeEnd13_read(), InternalMemory.PWMSlopeDuration13_read(), MinuteOffset);
}

void RA_PCA9685::Channel14PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(14, InternalMemory.PWMSlopeStart14_read(), InternalMemory.PWMSlopeEnd14_read(), InternalMemory.PWMSlopeDuration14_read(), MinuteOffset);
}

void RA_PCA9685::Channel15PWMSlope(uint8_t MinuteOffset)
{
  ChannelPWMSlope(15, InternalMemory.PWMSlopeStart15_read(), InternalMemory.PWMSlopeEnd15_read(), InternalMemory.PWMSlopeDuration15_read(), MinuteOffset);
}

void RA_PCA9685::ChannelPWMSlope(byte Channel, uint8_t Start, uint8_t End, uint8_t Duration)
{
  SetChannel(Channel, PWMSlope(
    InternalMemory.StdLightsOnHour_read(),
    InternalMemory.StdLightsOnMinute_read(),
    InternalMemory.StdLightsOffHour_read(),
    InternalMemory.StdLightsOffMinute_read(),
    Start,
    End,
    Duration,
    PWMChannel[Channel]
  ));
}

void RA_PCA9685::ChannelPWMSlope(byte Channel, uint8_t Start, uint8_t End, uint8_t Duration, uint8_t MinuteOffset)
{
  int onTime = NumMins(InternalMemory.StdLightsOnHour_read(), InternalMemory.StdLightsOnMinute_read()) - MinuteOffset;
  int offTime = NumMins(InternalMemory.StdLightsOffHour_read(), InternalMemory.StdLightsOffMinute_read()) + MinuteOffset;
  SetChannel(Channel, PWMSlope(
    onTime / 60,
    onTime % 60,
    offTime / 60,
    offTime % 60,
    Start,
    End,
    Duration,
    PWMChannel[Channel]
  ));
}

void RA_PCA9685::ChannelPWMSlope(uint8_t onHour, uint8_t onMinute, uint8_t offHour, uint8_t offMinute, uint8_t Channel, uint8_t Start, uint8_t End, uint8_t Duration)
{
  SetChannel(Channel, PWMSlope(
    onHour,
    onMinute,
    offHour,
    offMinute,
    Start,
    End,
    Duration,
    PWMChannel[Channel]
  ));
}

void RA_PCA9685::Channel0PWMParabola()
{
  ChannelPWMParabola(0, InternalMemory.PWMSlopeStart0_read(), InternalMemory.PWMSlopeEnd0_read());
}

void RA_PCA9685::Channel1PWMParabola()
{
  ChannelPWMParabola(1, InternalMemory.PWMSlopeStart1_read(), InternalMemory.PWMSlopeEnd1_read());
}

void RA_PCA9685::Channel2PWMParabola()
{
  ChannelPWMParabola(2, InternalMemory.PWMSlopeStart2_read(), InternalMemory.PWMSlopeEnd2_read());
}

void RA_PCA9685::Channel3PWMParabola()
{
  ChannelPWMParabola(3, InternalMemory.PWMSlopeStart3_read(), InternalMemory.PWMSlopeEnd3_read());
}

void RA_PCA9685::Channel4PWMParabola()
{
  ChannelPWMParabola(4, InternalMemory.PWMSlopeStart4_read(), InternalMemory.PWMSlopeEnd4_read());
}

void RA_PCA9685::Channel5PWMParabola()
{
  ChannelPWMParabola(5, InternalMemory.PWMSlopeStart5_read(), InternalMemory.PWMSlopeEnd5_read());
}

void RA_PCA9685::Channel6PWMParabola()
{
  ChannelPWMParabola(6, InternalMemory.PWMSlopeStart6_read(), InternalMemory.PWMSlopeEnd6_read());
}

void RA_PCA9685::Channel7PWMParabola()
{
  ChannelPWMParabola(7, InternalMemory.PWMSlopeStart7_read(), InternalMemory.PWMSlopeEnd7_read());
}

void RA_PCA9685::Channel8PWMParabola()
{
  ChannelPWMParabola(8, InternalMemory.PWMSlopeStart8_read(), InternalMemory.PWMSlopeEnd8_read());
}

void RA_PCA9685::Channel9PWMParabola()
{
  ChannelPWMParabola(9, InternalMemory.PWMSlopeStart9_read(), InternalMemory.PWMSlopeEnd9_read());
}

void RA_PCA9685::Channel10PWMParabola()
{
  ChannelPWMParabola(10, InternalMemory.PWMSlopeStart10_read(), InternalMemory.PWMSlopeEnd10_read());
}

void RA_PCA9685::Channel11PWMParabola()
{
  ChannelPWMParabola(11, InternalMemory.PWMSlopeStart11_read(), InternalMemory.PWMSlopeEnd11_read());
}

void RA_PCA9685::Channel12PWMParabola()
{
  ChannelPWMParabola(12, InternalMemory.PWMSlopeStart12_read(), InternalMemory.PWMSlopeEnd12_read());
}

void RA_PCA9685::Channel13PWMParabola()
{
  ChannelPWMParabola(13, InternalMemory.PWMSlopeStart13_read(), InternalMemory.PWMSlopeEnd13_read());
}

void RA_PCA9685::Channel14PWMParabola()
{
  ChannelPWMParabola(14, InternalMemory.PWMSlopeStart14_read(), InternalMemory.PWMSlopeEnd14_read());
}

void RA_PCA9685::Channel15PWMParabola()
{
  ChannelPWMParabola(15, InternalMemory.PWMSlopeStart15_read(), InternalMemory.PWMSlopeEnd15_read());
}

void RA_PCA9685::Channel0PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(0, InternalMemory.PWMSlopeStart0_read(), InternalMemory.PWMSlopeEnd0_read(), MinuteOffset);
}

void RA_PCA9685::Channel1PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(1, InternalMemory.PWMSlopeStart1_read(), InternalMemory.PWMSlopeEnd1_read(), MinuteOffset);
}

void RA_PCA9685::Channel2PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(2, InternalMemory.PWMSlopeStart2_read(), InternalMemory.PWMSlopeEnd2_read(), MinuteOffset);
}

void RA_PCA9685::Channel3PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(3, InternalMemory.PWMSlopeStart3_read(), InternalMemory.PWMSlopeEnd3_read(), MinuteOffset);
}

void RA_PCA9685::Channel4PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(4, InternalMemory.PWMSlopeStart4_read(), InternalMemory.PWMSlopeEnd4_read(), MinuteOffset);
}

void RA_PCA9685::Channel5PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(5, InternalMemory.PWMSlopeStart5_read(), InternalMemory.PWMSlopeEnd5_read(), MinuteOffset);
}

void RA_PCA9685::Channel6PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(6, InternalMemory.PWMSlopeStart6_read(), InternalMemory.PWMSlopeEnd6_read(), MinuteOffset);
}

void RA_PCA9685::Channel7PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(7, InternalMemory.PWMSlopeStart7_read(), InternalMemory.PWMSlopeEnd7_read(), MinuteOffset);
}

void RA_PCA9685::Channel8PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(8, InternalMemory.PWMSlopeStart8_read(), InternalMemory.PWMSlopeEnd8_read(), MinuteOffset);
}

void RA_PCA9685::Channel9PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(9, InternalMemory.PWMSlopeStart9_read(), InternalMemory.PWMSlopeEnd9_read(), MinuteOffset);
}

void RA_PCA9685::Channel10PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(10, InternalMemory.PWMSlopeStart10_read(), InternalMemory.PWMSlopeEnd10_read(), MinuteOffset);
}

void RA_PCA9685::Channel11PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(11, InternalMemory.PWMSlopeStart11_read(), InternalMemory.PWMSlopeEnd11_read(), MinuteOffset);
}

void RA_PCA9685::Channel12PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(12, InternalMemory.PWMSlopeStart12_read(), InternalMemory.PWMSlopeEnd12_read(), MinuteOffset);
}

void RA_PCA9685::Channel13PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(13, InternalMemory.PWMSlopeStart13_read(), InternalMemory.PWMSlopeEnd13_read(), MinuteOffset);
}

void RA_PCA9685::Channel14PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(14, InternalMemory.PWMSlopeStart14_read(), InternalMemory.PWMSlopeEnd14_read(), MinuteOffset);
}

void RA_PCA9685::Channel15PWMParabola(uint8_t MinuteOffset)
{
  ChannelPWMParabola(15, InternalMemory.PWMSlopeStart15_read(), InternalMemory.PWMSlopeEnd15_read(), MinuteOffset);
}

void RA_PCA9685::ChannelPWMParabola(byte Channel, uint8_t Start, uint8_t End)
{
  SetChannel(Channel, PWMParabola(
    InternalMemory.StdLightsOnHour_read(),
    InternalMemory.StdLightsOnMinute_read(),
    InternalMemory.StdLightsOffHour_read(),
    InternalMemory.StdLightsOffMinute_read(),
    Start,
    End,
    PWMChannel[Channel]
  ));
}

void RA_PCA9685::ChannelPWMParabola(byte Channel, uint8_t Start, uint8_t End, uint8_t MinuteOffset)
{
  int onTime = NumMins(InternalMemory.StdLightsOnHour_read(), InternalMemory.StdLightsOnMinute_read()) - MinuteOffset;
  int offTime = NumMins(InternalMemory.StdLightsOffHour_read(), InternalMemory.StdLightsOffMinute_read()) + MinuteOffset;
  SetChannel(Channel, PWMParabola(
    onTime / 60,
    onTime % 60,
    offTime / 60,
    offTime % 60,
    Start,
    End,
    PWMChannel[Channel]
  ));
}

void RA_PCA9685::ChannelPWMParabola(uint8_t onHour, uint8_t onMinute, uint8_t offHour, uint8_t offMinute, byte Channel, uint8_t Start, uint8_t End)
{
  SetChannel(Channel, PWMParabola(
    onHour,
    onMinute,
    offHour,
    offMinute,
    Start,
    End,
    PWMChannel[Channel]
  ));
}
