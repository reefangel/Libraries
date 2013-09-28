/*
 * Copyright 2013 Reef Angel
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

#ifndef __RA_PCA9685_H__
#define __RA_PCA9685_H__

#include <Globals.h>
#include <InternalEEPROM.h>
#include <Wire.h>

class RA_PCA9685
{
  public:
    RA_PCA9685(byte address);

    void inline SetChannel(byte Channel, byte Value) { if (Channel < 16) PWMChannel[Channel] = Value; };
    void inline SetChannelOverride(byte Channel, byte Value) { if (Channel < 16) PWMChannelOverride[Channel] = Value; };

    void SetAllChannelsPercent(byte p);
    void SetChannelPercent(byte Channel, byte p);
    void SetPercentForChannels(int Channels, byte p);

    void SetSlopesForChannels(int Channels, uint8_t Start, uint8_t End, uint8_t Duration);
    void SetSlopesForChannels(uint8_t onHour, uint8_t onMinute, uint8_t offHour, uint8_t offMinute, int Channels, uint8_t Start, uint8_t End, uint8_t Duration);
    void SetSlopesForChannels(int Channels, uint8_t Start, uint8_t End, uint8_t Duration, uint8_t MinuteOffset);

    void SetParabolasForChannels(int Channels, uint8_t Start, uint8_t End);
    void SetParabolasForChannels(uint8_t onHour, uint8_t onMinute, uint8_t offHour, uint8_t offMinute, int Channels, uint8_t Start, uint8_t End);
    void SetParabolasForChannels(int Channels, uint8_t Start, uint8_t End, uint8_t MinuteOffset);

    byte GetChannelValue(byte Channel);
    void SendData(byte Channel, byte Data);
    void Write();

    void Channel0PWMSlope();
    void Channel1PWMSlope();
    void Channel2PWMSlope();
    void Channel3PWMSlope();
    void Channel4PWMSlope();
    void Channel5PWMSlope();
    void Channel6PWMSlope();
    void Channel7PWMSlope();
    void Channel8PWMSlope();
    void Channel9PWMSlope();
    void Channel10PWMSlope();
    void Channel11PWMSlope();
    void Channel12PWMSlope();
    void Channel13PWMSlope();
    void Channel14PWMSlope();
    void Channel15PWMSlope();

    void Channel0PWMSlope(uint8_t MinuteOffset);
    void Channel1PWMSlope(uint8_t MinuteOffset);
    void Channel2PWMSlope(uint8_t MinuteOffset);
    void Channel3PWMSlope(uint8_t MinuteOffset);
    void Channel4PWMSlope(uint8_t MinuteOffset);
    void Channel5PWMSlope(uint8_t MinuteOffset);
    void Channel6PWMSlope(uint8_t MinuteOffset);
    void Channel7PWMSlope(uint8_t MinuteOffset);
    void Channel8PWMSlope(uint8_t MinuteOffset);
    void Channel9PWMSlope(uint8_t MinuteOffset);
    void Channel10PWMSlope(uint8_t MinuteOffset);
    void Channel11PWMSlope(uint8_t MinuteOffset);
    void Channel12PWMSlope(uint8_t MinuteOffset);
    void Channel13PWMSlope(uint8_t MinuteOffset);
    void Channel14PWMSlope(uint8_t MinuteOffset);
    void Channel15PWMSlope(uint8_t MinuteOffset);

    void ChannelPWMSlope(byte Channel, uint8_t Start, uint8_t End, uint8_t Duration);
    void ChannelPWMSlope(byte Channel, uint8_t Start, uint8_t End, uint8_t Duration, uint8_t MinuteOffset);
    void ChannelPWMSlope(uint8_t onHour, uint8_t onMinute, uint8_t offHour, uint8_t offMinute, byte Channel, uint8_t Start, uint8_t End, uint8_t Duration);

    void Channel0PWMParabola();
    void Channel1PWMParabola();
    void Channel2PWMParabola();
    void Channel3PWMParabola();
    void Channel4PWMParabola();
    void Channel5PWMParabola();
    void Channel6PWMParabola();
    void Channel7PWMParabola();
    void Channel8PWMParabola();
    void Channel9PWMParabola();
    void Channel10PWMParabola();
    void Channel11PWMParabola();
    void Channel12PWMParabola();
    void Channel13PWMParabola();
    void Channel14PWMParabola();
    void Channel15PWMParabola();

    void Channel0PWMParabola(uint8_t MinuteOffset);
    void Channel1PWMParabola(uint8_t MinuteOffset);
    void Channel2PWMParabola(uint8_t MinuteOffset);
    void Channel3PWMParabola(uint8_t MinuteOffset);
    void Channel4PWMParabola(uint8_t MinuteOffset);
    void Channel5PWMParabola(uint8_t MinuteOffset);
    void Channel6PWMParabola(uint8_t MinuteOffset);
    void Channel7PWMParabola(uint8_t MinuteOffset);
    void Channel8PWMParabola(uint8_t MinuteOffset);
    void Channel9PWMParabola(uint8_t MinuteOffset);
    void Channel10PWMParabola(uint8_t MinuteOffset);
    void Channel11PWMParabola(uint8_t MinuteOffset);
    void Channel12PWMParabola(uint8_t MinuteOffset);
    void Channel13PWMParabola(uint8_t MinuteOffset);
    void Channel14PWMParabola(uint8_t MinuteOffset);
    void Channel15PWMParabola(uint8_t MinuteOffset);

    void ChannelPWMParabola(byte Channel, uint8_t Start, uint8_t End);
    void ChannelPWMParabola(byte Channel, uint8_t Start, uint8_t End, uint8_t MinuteOffset);
    void ChannelPWMParabola(uint8_t onHour, uint8_t onMinute, uint8_t offHour, uint8_t offMinute, uint8_t Channel, uint8_t Start, uint8_t End);

    byte PWMChannel[16];
    byte PWMChannelOverride[16];

  private:
    byte _address;
    unsigned long _lastWrite;
};

#endif  // __RA_PCA9685_H__
