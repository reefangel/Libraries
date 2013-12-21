#ifndef __TIDE_H__
#define __TIDE_H__
#define PI 3.141593 

#include <Globals.h>

class Tide
{
public:
  Tide();
  int  CalcTide();
  void Init(int Speed, int minOffset, int maxOffset);

  void SetOffset(int minOffset, int maxOffset);
  void SetWaveLength(double WaveLength);

  void SetSpeed(int Speed);
   
  inline int GetSpeed() { return m_Speed; }
  inline boolean isIncoming() { return m_Flood; } 
  inline boolean isOutgoing() { return m_Ebb; } 
  
private:
  int m_Speed;
  int m_CurrSpeed;
  int m_MinOffset;
  int m_MaxOffset;
  double m_Amp, m_PrevAmp;
  double m_WaveLength;
  boolean m_Ebb,m_Flood;
};


#endif  // __TIDE_H__
