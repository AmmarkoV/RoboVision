#ifndef GDDGWEBINTERFACE_H_INCLUDED
#define GDDGWEBINTERFACE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <wx/bitmap.h>

int OpenWebInterface();
int CloseWebInterface();
int WebIntNeedsNewSnapshot();
int UpdateNetworkInterface(wxBitmap * pica,wxBitmap * picb,wxBitmap * picc,wxBitmap * picd,unsigned int xsize,unsigned int ysize);

#endif // WEBINTERFACE_H_INCLUDED
