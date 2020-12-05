#include "GCLC.h"

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_animation_frames() {
  GCLCError iRv;
  double num;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  m_nAnimationFrames = num < 0 ? 0 : (int)num;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;
  m_nAnimationSpeed = num < 0 ? 0 : (int)num;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

bool CGCLC::GetAnimationFrames(int &iFrames, int &iSpeed) {
  if (m_nAnimationFrames == 0)
    return false;
  iFrames = m_nAnimationFrames;
  iSpeed = m_nAnimationSpeed;
  return true;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_animation_trace_point() {
  string sPointName;
  GCLC_object o1;
  double r, g, b;
  GCLCError iRv;

  if (ReadToken(sPointName) != rvGCLCOK)
    return rvGCLCIdExpected;

  if ((iRv = Value(sPointName, o1)) != rvGCLCOK)
    return iRv;
  if (o1.type != GCLC_POINT)
    return rvGCLCPointExpected;
  if ((iRv = ReadNumber(r)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(g)) != rvGCLCOK)
    return iRv;
  if ((iRv = ReadNumber(b)) != rvGCLCOK)
    return iRv;

  if (AddTracePoint((unsigned char)r, (unsigned char)g, (unsigned char)b,
                    sPointName) != rvG_OK)
    return rvGCLCOutOfMemory;

  return rvGCLCOK;
}

// ----------------------------------------------------------------------------
