#include "GCLC.h"

// ----------------------------------------------------------------------------

GCLCError CGCLC::get_layer() {
  GCLCError iRv;
  double num;
  int l;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;

  l = (int)num;
  if (l < 0)
    l = 0;
  SetLayer(l);
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::hide_layer() {
  GCLCError iRv;
  double num;
  int l;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;

  l = (int)num;
  if (l >= 0 && l <= 1000)
    HideLayer(l);
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::hide_layersfrom() {
  GCLCError iRv;
  double num;
  int l;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;

  l = (int)num;
  if (l < 0)
    HideLayersFrom(0);
  if (l > 1000)
    return rvGCLCOK;
  HideLayersFrom(l);
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------

GCLCError CGCLC::hide_layersto() {
  GCLCError iRv;
  double num;
  int l;

  if ((iRv = ReadNumber(num)) != rvGCLCOK)
    return iRv;

  l = (int)num;
  if (l < 0)
    return rvGCLCOK;
  if (l > 1000)
    HideLayersTo(1000);
  HideLayersTo(l);
  return rvGCLCOK;
}

// ----------------------------------------------------------------------------
