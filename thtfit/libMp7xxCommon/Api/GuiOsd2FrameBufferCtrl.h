#if	!defined(_GUI_OSD2_FRAMEBUFFER_CTRL_H_)
#define	_GUI_OSD2_FRAMEBUFFER_CTRL_H_
#if defined(__mips__)
#include <BaseTypeDef.h>

class CGuiOsd2FrameBufferCtrl
{
public:
	CGuiOsd2FrameBufferCtrl();
	~CGuiOsd2FrameBufferCtrl();
	INT_t getSurfaceInfo(OUT UINT8_t * & pSurfaceLumaVirtAddr, OUT UINT32_t & surfaceWidth, OUT UINT32_t & surfaceHeight);
protected:
	INT_t GuiOsd2_AddRef();
	INT_t GuiOsd2_mmapSurface();
	INT_t GuiOsd2_munmapSurface();
	INT_t GuiOsd2_RemoveRef();	
	VOID GuiOsd2_removeNonexistingProcessRef();
private:
	void * m_pRua;
	BOOL_t m_bHasAddRef;
	BOOL_t m_bSurfaceLumaAddrIsLocked;
	BOOL_t m_bSurfaceLumaAddrIsMapped;
	UINT8_t * m_pLumaVirtAddr;
	UINT32_t m_surfaceLumaSize;
	UINT32_t m_surfaceWidth;
	UINT32_t m_surfaceHeight;
};

#endif
#endif	//_GUI_OSD2_FRAMEBUFFER_CTRL_H_

