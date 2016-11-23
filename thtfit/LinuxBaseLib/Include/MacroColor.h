#ifndef	_MACROCOLOR_H_
#define	_MACROCOLOR_H_

#ifndef	RGB
	//MSB first, ABGR
  #define	RGB(clrRed, clrGreen, clrBlue)		((DWORD)(clrRed | clrGreen << 8 | clrBlue << 16))
#endif	//RGB

//MSB first, ABGR
#define	RGBA(clrRed,clrGreen,clrBlue,Alpha)		((DWORD)(clrRed | clrGreen << 8 | clrBlue << 16 | Alpha << 24))

#define	COLOR_BLACK			RGB(0, 0, 0)
#define	COLOR_RED				RGB(255, 0, 0)
#define	COLOR_GREEN			RGB(0, 255, 0)
#define	COLOR_BLUE				RGB(0, 0, 255)
#define	COLOR_WHITE			RGB(255, 255, 255)

#endif	//_MACROCOLOR_H_

