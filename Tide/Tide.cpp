#include <stdlib.h>
#include <math.h>
#include <Time.h>
#include "Tide.h"
#include "Globals.h"

Tide::Tide()
{
	m_WaveLength=12*SECS_PER_HOUR;
	m_MaxOffset = 50;
	m_MinOffset = 10;
	m_Speed = 50;
	m_Amp = 1;
	m_CurrSpeed=m_Speed;
}

void Tide::Init(int Speed, int minOffset, int maxOffset)
{
	SetSpeed(Speed);
	SetOffset(minOffset,maxOffset);
}

void Tide::SetSpeed(int Speed)
{
	m_Speed = Speed;
}

void Tide::SetWaveLength(double WaveLength)
{ 
	m_WaveLength = WaveLength;
}

void Tide::SetOffset(int minOffset, int maxOffset)
{
	// We want these in half since they will be multiplied by amplitude (+1/-1)
	m_MinOffset = minOffset/2;
	m_MaxOffset = maxOffset/2;
}

/*
Spring tides occur during the full moon and the new moon.
At these times, the high tides are very high and the low tides are very low.

Neap tides occur during quarter moons.
The result is a smaller difference between high and low tides

MoonPhase 0-25 = Spring
MoonPhase 26-74 = Neap
MoonPhase 75-100 = Spring

So, the effect of the Moon will be a cosine wave.
*/

int Tide::CalcTide() {
  double moonOffset; // gap between high and low
  double amplitude;  // tide curve

  // Calculate the gap between high and low tide based on MoonPhase()
  moonOffset=cos(((2*PI)/100)*MoonPhase());
  moonOffset=((moonOffset+1)/2)*100; // Convert to percentage
  moonOffset=map(moonOffset,0,100,m_MinOffset,m_MaxOffset);


  // Find out the current tidal height
  amplitude=sin(((2*PI)/m_WaveLength)*now()); 
  amplitude=amplitude*moonOffset; 
  
  // Update Ebb/Flood
  m_PrevAmp=m_Amp;
  m_Amp=amplitude;

  if (m_Amp>m_PrevAmp) {
    m_Flood=true;
    m_Ebb=false;
  } else if (m_Amp<m_PrevAmp) {
    m_Flood=false;
    m_Ebb=true;
  }

  // Adjust the calculate speed to be in our adjusted range
  m_CurrSpeed=constrain(m_Speed+amplitude,0,100);

  return m_CurrSpeed;
}
