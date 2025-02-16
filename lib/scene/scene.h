/***************************************************************
**
** NanoKit Library Header File
**
** File         :  scene.h
** Module       :  scene
** Author       :  SH
** Created      :  2025-02-15 (YYYY-MM-DD)
** License      :  MIT
** Description  :  3D Scene API
**
***************************************************************/

#ifndef SCENE_H
#define SCENE_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdint.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef uintptr_t SceneHandle;
typedef uintptr_t SceneObjectHandle;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

SceneHandle InitScene();
void FreeScene(SceneHandle scene);

SceneObjectHandle CreateCube(SceneHandle scene, float x, float y, float z, float size);
SceneObjectHandle CreateSphere(SceneHandle scene, float x, float y, float z, float radius);
SceneObjectHandle CreateCylinder(SceneHandle scene, float x, float y, float z, float radius, float height);





#endif /* SCENE_H */