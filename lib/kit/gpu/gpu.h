/***************************************************************
**
** NanoKit Library Header File
**
** File         :  gpu.h
** Module       :  gpu
** Author       :  SH
** Created      :  2025-03-02 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit GPU API
**
***************************************************************/

#ifndef GPU_H
#define GPU_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdint.h>
#include <stdbool.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef uintptr_t ShaderHandle;
typedef uintptr_t BufferHandle;
typedef uintptr_t TextureHandle;

typedef struct 
{

} GpuContext;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

/* create a GPU context with default shaders */
/* on a per-window basis */
GpuContext *CreateGpuContext();

void DestroyGpuContext(GpuContext *context);

void SetContextCurrent(GpuContext *context);

TextureHandle CreateTexture(uint32_t width, uint32_t height);
void ResizeTexture(TextureHandle texture, uint32_t width, uint32_t height);
void DestroyTexture(TextureHandle texture);

void BeginDeferredRender(TextureHandle texture);
void EndDeferredRender();

/* Render texture assuming orthographic compositing */
void CompositeTexture(TextureHandle texture, float x, float y, float width, float height);

#endif /* GPU_H */