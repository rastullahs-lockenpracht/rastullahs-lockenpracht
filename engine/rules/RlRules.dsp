# Microsoft Developer Studio Project File - Name="RlRules" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RlRules - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "RlRules.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "RlRules.mak" CFG="RlRules - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "RlRules - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RlRules - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RlRules - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "lib\Debug"
# PROP BASE Intermediate_Dir "obj\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib\Debug"
# PROP Intermediate_Dir "obj\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /I "..\..\dependencies\ogre\Dependencies\include" /I "..\..\dependencies\ogre\PlugIns\GuiElements\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "RASTULLAH_EXPORTS" /D "_MBCS" /I /GZ "" ..\..\dependencies\ruby\lib\ruby\1.8\i386-mswin32"" PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I ".\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /I "..\..\dependencies\ogre\Dependencies\include" /I "..\..\dependencies\ogre\PlugIns\GuiElements\include" /I "..\..\dependencies\ruby\lib\ruby\1.8\i386-mswin32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "RASTULLAH_EXPORTS" /D "_MBCS" /I /GZ /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt-ruby18.lib OgreMain.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /implib:"$(OutDir)/RlRules.lib" /pdbtype:sept /libpath:"..\..\dependencies\ruby\lib" /libpath:""..\..\dependencies\ogre\OgreMain\lib\$(ConfigurationName)"" /libpath:""..\..\dependencies\ogre\PlugIns\GuiElements\bin\$(ConfigurationName)""
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt-ruby18.lib OgreMain.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /implib:"$(OutDir)/RlRules.lib" /pdbtype:sept /libpath:"..\..\dependencies\ruby\lib" /libpath:""..\..\dependencies\ogre\OgreMain\lib\Debug"" /libpath:""..\..\dependencies\ogre\PlugIns\GuiElements\bin\Debug""
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RlRules - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "lib\Release"
# PROP BASE Intermediate_Dir "obj\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib\Release"
# PROP Intermediate_Dir "obj\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /I ".\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /I "..\..\dependencies\ogre\Dependencies\include" /I "..\..\dependencies\ogre\PlugIns\GuiElements\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "RASTULLAH_EXPORTS" /D "_MBCS" ..\..\dependencies\ruby\lib\ruby\1.8\i386-mswin32"" PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MD /W3 /GX /Zi /I ".\include" /I "..\common\include" /I "..\..\dependencies\ogre\OgreMain\include" /I "..\..\dependencies\ogre\Dependencies\include" /I "..\..\dependencies\ogre\PlugIns\GuiElements\include" /I "..\..\dependencies\ruby\lib\ruby\1.8\i386-mswin32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "RASTULLAH_EXPORTS" /D "_MBCS" /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt-ruby18.lib OgreMain.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /implib:"$(OutDir)/RlRules.lib" /pdbtype:sept /libpath:"..\..\dependencies\ruby\lib" /libpath:""..\..\dependencies\ogre\OgreMain\lib\$(ConfigurationName)"" /libpath:""..\..\dependencies\ogre\PlugIns\GuiElements\bin\$(ConfigurationName)"" /opt:ref /opt:icf
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib msvcrt-ruby18.lib OgreMain.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /implib:"$(OutDir)/RlRules.lib" /pdbtype:sept /libpath:"..\..\dependencies\ruby\lib" /libpath:""..\..\dependencies\ogre\OgreMain\lib\Release"" /libpath:""..\..\dependencies\ogre\PlugIns\GuiElements\bin\Release"" /opt:ref /opt:icf
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RlRules - Win32 Debug"
# Name "RlRules - Win32 Release"
# Begin Group "Quelldateien"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=.\src\Capability.cpp
DEP_CPP_CAPAB=\
	"..\..\dependencies\ogre\OgreMain\include\Ogre.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreActionEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreActionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimation.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimationState.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimationTrack.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreArchiveEx.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreArchiveManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAutoParamDataSource.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAxisAlignedBox.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBillboard.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBillboardSet.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBlendMode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBone.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreCamera.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreColourValue.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreCommon.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreConfig.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreConfigOptionMap.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreController.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreControllerManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreDataChunk.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreDynLibManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEntity.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventDispatcher.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventListeners.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventProcessor.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreException.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreFrameListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreFrustum.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGpuProgram.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGpuProgramManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiContainer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiElement.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiElementCommands.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareBufferManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareIndexBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareVertexBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHighLevelGpuProgram.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHighLevelGpuProgramManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreImage.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreInput.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreInputEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreIteratorWrappers.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreKeyFrame.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreKeyTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLight.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLog.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLogManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterial.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterialManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterialSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMath.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMatrix3.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMatrix4.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMemoryMacros.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMemoryManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshSerializerImpl.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseMotionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMovableObject.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreNode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreOverlay.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreOverlayManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticle.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleAffector.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleEmitter.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleEmitterCommands.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleIterator.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleSystem.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleSystemManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePass.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePatchMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePatchSurface.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlane.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlatform.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlatformManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePositionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePredefinedControllers.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePrerequisites.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreProfiler.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreProgressiveMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreQuaternion.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRay.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderable.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderOperation.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderQueue.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderQueueListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderSystem.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderSystemCapabilities.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTargetListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTexture.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderWindow.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreResource.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreResourceManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRoot.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRotationalSpline.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneManagerEnumerator.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneNode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneQuery.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreScrollEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSDDataChunk.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSharedPtr.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSimpleRenderable.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSimpleSpline.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSingleton.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeleton.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeletonManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeletonSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSphere.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStdHeaders.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreString.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringConverter.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringInterface.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringVector.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSubEntity.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSubMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTargetManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTechnique.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTexture.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTextureManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTextureUnitState.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTimer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreUserDefinedObject.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVector3.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVector4.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVertexBoneAssignment.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVertexIndexData.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreViewport.h"\
	"..\..\dependencies\ruby\lib\ruby\1.8\i386-mswin32\config.h"\
	"..\common\include\Exception.h"\
	"..\common\include\RastullahPrerequisites.h"\
	".\include\Capability.h"\
	{$(INCLUDE)}"config\_epilog.h"\
	{$(INCLUDE)}"config\_msvc_warnings_off.h"\
	{$(INCLUDE)}"config\_prolog.h"\
	{$(INCLUDE)}"config\stl_apcc.h"\
	{$(INCLUDE)}"config\stl_apple.h"\
	{$(INCLUDE)}"config\stl_as400.h"\
	{$(INCLUDE)}"config\stl_bc.h"\
	{$(INCLUDE)}"config\stl_como.h"\
	{$(INCLUDE)}"config\stl_confix.h"\
	{$(INCLUDE)}"config\stl_dec.h"\
	{$(INCLUDE)}"config\stl_dec_vms.h"\
	{$(INCLUDE)}"config\stl_fujitsu.h"\
	{$(INCLUDE)}"config\stl_gcc.h"\
	{$(INCLUDE)}"config\stl_hpacc.h"\
	{$(INCLUDE)}"config\stl_ibm.h"\
	{$(INCLUDE)}"config\stl_intel.h"\
	{$(INCLUDE)}"config\stl_kai.h"\
	{$(INCLUDE)}"config\stl_msvc.h"\
	{$(INCLUDE)}"config\stl_mwerks.h"\
	{$(INCLUDE)}"config\stl_mycomp.h"\
	{$(INCLUDE)}"config\stl_sco.h"\
	{$(INCLUDE)}"config\stl_select_lib.h"\
	{$(INCLUDE)}"config\stl_sgi.h"\
	{$(INCLUDE)}"config\stl_solaris.h"\
	{$(INCLUDE)}"config\stl_sunpro.h"\
	{$(INCLUDE)}"config\stl_symantec.h"\
	{$(INCLUDE)}"config\stl_watcom.h"\
	{$(INCLUDE)}"config\stl_wince.h"\
	{$(INCLUDE)}"config\stlcomp.h"\
	{$(INCLUDE)}"config\vc_select_lib.h"\
	{$(INCLUDE)}"hash_map"\
	{$(INCLUDE)}"hash_set"\
	{$(INCLUDE)}"pthread.h"\
	{$(INCLUDE)}"stl\_abbrevs.h"\
	{$(INCLUDE)}"stl\_algobase.c"\
	{$(INCLUDE)}"stl\_algobase.h"\
	{$(INCLUDE)}"stl\_alloc.c"\
	{$(INCLUDE)}"stl\_alloc.h"\
	{$(INCLUDE)}"stl\_alloc_old.h"\
	{$(INCLUDE)}"stl\_bvector.h"\
	{$(INCLUDE)}"stl\_config.h"\
	{$(INCLUDE)}"stl\_config_compat.h"\
	{$(INCLUDE)}"stl\_config_compat_post.h"\
	{$(INCLUDE)}"stl\_construct.h"\
	{$(INCLUDE)}"stl\_epilog.h"\
	{$(INCLUDE)}"stl\_function_base.h"\
	{$(INCLUDE)}"stl\_hash_fun.h"\
	{$(INCLUDE)}"stl\_hash_map.h"\
	{$(INCLUDE)}"stl\_hash_set.h"\
	{$(INCLUDE)}"stl\_hashtable.c"\
	{$(INCLUDE)}"stl\_hashtable.h"\
	{$(INCLUDE)}"stl\_iterator.h"\
	{$(INCLUDE)}"stl\_iterator_base.h"\
	{$(INCLUDE)}"stl\_iterator_old.h"\
	{$(INCLUDE)}"stl\_pair.h"\
	{$(INCLUDE)}"stl\_prolog.h"\
	{$(INCLUDE)}"stl\_pthread_alloc.c"\
	{$(INCLUDE)}"stl\_pthread_alloc.h"\
	{$(INCLUDE)}"stl\_ptrs_specialize.h"\
	{$(INCLUDE)}"stl\_range_errors.h"\
	{$(INCLUDE)}"stl\_relops_cont.h"\
	{$(INCLUDE)}"stl\_site_config.h"\
	{$(INCLUDE)}"stl\_sparc_atomic.h"\
	{$(INCLUDE)}"stl\_threads.c"\
	{$(INCLUDE)}"stl\_threads.h"\
	{$(INCLUDE)}"stl\_uninitialized.h"\
	{$(INCLUDE)}"stl\_vector.c"\
	{$(INCLUDE)}"stl\_vector.h"\
	{$(INCLUDE)}"stl\debug\_debug.c"\
	{$(INCLUDE)}"stl\debug\_debug.h"\
	{$(INCLUDE)}"stl\debug\_hashtable.h"\
	{$(INCLUDE)}"stl\debug\_iterator.h"\
	{$(INCLUDE)}"stl\debug\_vector.h"\
	{$(INCLUDE)}"stl\type_traits.h"\
	{$(INCLUDE)}"stl\wrappers\_hash_map.h"\
	{$(INCLUDE)}"stl\wrappers\_hash_set.h"\
	{$(INCLUDE)}"stl\wrappers\_vector.h"\
	{$(INCLUDE)}"stl_user_config.h"\
	
NODEP_CPP_CAPAB=\
	"..\..\..\..\usr\include\pthread.h"\
	
# End Source File
# Begin Source File

SOURCE=src\DsaManager.cpp
DEP_CPP_DSAMA=\
	"..\..\dependencies\ogre\OgreMain\include\Ogre.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreActionEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreActionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimation.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimationState.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimationTrack.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreArchiveEx.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreArchiveManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAutoParamDataSource.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAxisAlignedBox.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBillboard.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBillboardSet.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBlendMode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBone.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreCamera.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreColourValue.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreCommon.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreConfig.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreConfigOptionMap.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreController.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreControllerManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreDataChunk.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreDynLibManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEntity.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventDispatcher.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventListeners.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventProcessor.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreException.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreFrameListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreFrustum.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGpuProgram.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGpuProgramManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiContainer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiElement.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiElementCommands.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareBufferManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareIndexBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareVertexBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHighLevelGpuProgram.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHighLevelGpuProgramManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreImage.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreInput.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreInputEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreIteratorWrappers.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreKeyFrame.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreKeyTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLight.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLog.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLogManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterial.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterialManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterialSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMath.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMatrix3.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMatrix4.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMemoryMacros.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMemoryManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshSerializerImpl.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseMotionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMovableObject.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreNode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreOverlay.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreOverlayManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticle.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleAffector.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleEmitter.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleEmitterCommands.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleIterator.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleSystem.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleSystemManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePass.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePatchMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePatchSurface.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlane.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlatform.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlatformManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePositionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePredefinedControllers.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePrerequisites.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreProfiler.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreProgressiveMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreQuaternion.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRay.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderable.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderOperation.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderQueue.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderQueueListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderSystem.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderSystemCapabilities.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTargetListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTexture.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderWindow.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreResource.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreResourceManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRoot.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRotationalSpline.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneManagerEnumerator.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneNode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneQuery.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreScrollEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSDDataChunk.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSharedPtr.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSimpleRenderable.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSimpleSpline.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSingleton.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeleton.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeletonManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeletonSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSphere.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStdHeaders.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreString.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringConverter.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringInterface.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringVector.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSubEntity.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSubMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTargetManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTechnique.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTexture.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTextureManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTextureUnitState.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTimer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreUserDefinedObject.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVector3.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVector4.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVertexBoneAssignment.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVertexIndexData.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreViewport.h"\
	"..\..\dependencies\ruby\lib\ruby\1.8\i386-mswin32\config.h"\
	"..\common\include\RastullahPrerequisites.h"\
	".\include\DsaManager.h"\
	{$(INCLUDE)}"config\_epilog.h"\
	{$(INCLUDE)}"config\_msvc_warnings_off.h"\
	{$(INCLUDE)}"config\_prolog.h"\
	{$(INCLUDE)}"config\stl_apcc.h"\
	{$(INCLUDE)}"config\stl_apple.h"\
	{$(INCLUDE)}"config\stl_as400.h"\
	{$(INCLUDE)}"config\stl_bc.h"\
	{$(INCLUDE)}"config\stl_como.h"\
	{$(INCLUDE)}"config\stl_confix.h"\
	{$(INCLUDE)}"config\stl_dec.h"\
	{$(INCLUDE)}"config\stl_dec_vms.h"\
	{$(INCLUDE)}"config\stl_fujitsu.h"\
	{$(INCLUDE)}"config\stl_gcc.h"\
	{$(INCLUDE)}"config\stl_hpacc.h"\
	{$(INCLUDE)}"config\stl_ibm.h"\
	{$(INCLUDE)}"config\stl_intel.h"\
	{$(INCLUDE)}"config\stl_kai.h"\
	{$(INCLUDE)}"config\stl_msvc.h"\
	{$(INCLUDE)}"config\stl_mwerks.h"\
	{$(INCLUDE)}"config\stl_mycomp.h"\
	{$(INCLUDE)}"config\stl_sco.h"\
	{$(INCLUDE)}"config\stl_select_lib.h"\
	{$(INCLUDE)}"config\stl_sgi.h"\
	{$(INCLUDE)}"config\stl_solaris.h"\
	{$(INCLUDE)}"config\stl_sunpro.h"\
	{$(INCLUDE)}"config\stl_symantec.h"\
	{$(INCLUDE)}"config\stl_watcom.h"\
	{$(INCLUDE)}"config\stl_wince.h"\
	{$(INCLUDE)}"config\stlcomp.h"\
	{$(INCLUDE)}"config\vc_select_lib.h"\
	{$(INCLUDE)}"hash_map"\
	{$(INCLUDE)}"hash_set"\
	{$(INCLUDE)}"pthread.h"\
	{$(INCLUDE)}"stl\_abbrevs.h"\
	{$(INCLUDE)}"stl\_algobase.c"\
	{$(INCLUDE)}"stl\_algobase.h"\
	{$(INCLUDE)}"stl\_alloc.c"\
	{$(INCLUDE)}"stl\_alloc.h"\
	{$(INCLUDE)}"stl\_alloc_old.h"\
	{$(INCLUDE)}"stl\_bvector.h"\
	{$(INCLUDE)}"stl\_config.h"\
	{$(INCLUDE)}"stl\_config_compat.h"\
	{$(INCLUDE)}"stl\_config_compat_post.h"\
	{$(INCLUDE)}"stl\_construct.h"\
	{$(INCLUDE)}"stl\_epilog.h"\
	{$(INCLUDE)}"stl\_function_base.h"\
	{$(INCLUDE)}"stl\_hash_fun.h"\
	{$(INCLUDE)}"stl\_hash_map.h"\
	{$(INCLUDE)}"stl\_hash_set.h"\
	{$(INCLUDE)}"stl\_hashtable.c"\
	{$(INCLUDE)}"stl\_hashtable.h"\
	{$(INCLUDE)}"stl\_iterator.h"\
	{$(INCLUDE)}"stl\_iterator_base.h"\
	{$(INCLUDE)}"stl\_iterator_old.h"\
	{$(INCLUDE)}"stl\_pair.h"\
	{$(INCLUDE)}"stl\_prolog.h"\
	{$(INCLUDE)}"stl\_pthread_alloc.c"\
	{$(INCLUDE)}"stl\_pthread_alloc.h"\
	{$(INCLUDE)}"stl\_ptrs_specialize.h"\
	{$(INCLUDE)}"stl\_range_errors.h"\
	{$(INCLUDE)}"stl\_relops_cont.h"\
	{$(INCLUDE)}"stl\_site_config.h"\
	{$(INCLUDE)}"stl\_sparc_atomic.h"\
	{$(INCLUDE)}"stl\_threads.c"\
	{$(INCLUDE)}"stl\_threads.h"\
	{$(INCLUDE)}"stl\_uninitialized.h"\
	{$(INCLUDE)}"stl\_vector.c"\
	{$(INCLUDE)}"stl\_vector.h"\
	{$(INCLUDE)}"stl\debug\_debug.c"\
	{$(INCLUDE)}"stl\debug\_debug.h"\
	{$(INCLUDE)}"stl\debug\_hashtable.h"\
	{$(INCLUDE)}"stl\debug\_iterator.h"\
	{$(INCLUDE)}"stl\debug\_vector.h"\
	{$(INCLUDE)}"stl\type_traits.h"\
	{$(INCLUDE)}"stl\wrappers\_hash_map.h"\
	{$(INCLUDE)}"stl\wrappers\_hash_set.h"\
	{$(INCLUDE)}"stl\wrappers\_vector.h"\
	{$(INCLUDE)}"stl_user_config.h"\
	
NODEP_CPP_DSAMA=\
	"..\..\..\..\usr\include\pthread.h"\
	
# End Source File
# Begin Source File

SOURCE=.\src\GameObject.cpp
DEP_CPP_GAMEO=\
	"..\..\dependencies\ogre\OgreMain\include\Ogre.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreActionEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreActionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimation.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimationState.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimationTrack.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreArchiveEx.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreArchiveManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAutoParamDataSource.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAxisAlignedBox.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBillboard.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBillboardSet.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBlendMode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBone.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreCamera.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreColourValue.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreCommon.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreConfig.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreConfigOptionMap.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreController.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreControllerManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreDataChunk.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreDynLibManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEntity.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventDispatcher.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventListeners.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventProcessor.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreException.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreFrameListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreFrustum.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGpuProgram.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGpuProgramManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiContainer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiElement.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiElementCommands.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareBufferManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareIndexBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareVertexBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHighLevelGpuProgram.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHighLevelGpuProgramManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreImage.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreInput.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreInputEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreIteratorWrappers.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreKeyFrame.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreKeyTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLight.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLog.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLogManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterial.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterialManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterialSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMath.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMatrix3.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMatrix4.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMemoryMacros.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMemoryManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshSerializerImpl.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseMotionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMovableObject.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreNode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreOverlay.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreOverlayManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticle.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleAffector.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleEmitter.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleEmitterCommands.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleIterator.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleSystem.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleSystemManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePass.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePatchMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePatchSurface.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlane.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlatform.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlatformManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePositionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePredefinedControllers.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePrerequisites.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreProfiler.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreProgressiveMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreQuaternion.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRay.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderable.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderOperation.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderQueue.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderQueueListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderSystem.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderSystemCapabilities.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTargetListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTexture.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderWindow.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreResource.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreResourceManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRoot.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRotationalSpline.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneManagerEnumerator.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneNode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneQuery.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreScrollEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSDDataChunk.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSharedPtr.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSimpleRenderable.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSimpleSpline.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSingleton.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeleton.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeletonManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeletonSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSphere.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStdHeaders.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreString.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringConverter.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringInterface.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringVector.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSubEntity.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSubMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTargetManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTechnique.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTexture.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTextureManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTextureUnitState.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTimer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreUserDefinedObject.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVector3.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVector4.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVertexBoneAssignment.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVertexIndexData.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreViewport.h"\
	"..\..\dependencies\ruby\lib\ruby\1.8\i386-mswin32\config.h"\
	"..\common\include\RastullahPrerequisites.h"\
	".\include\GameObject.h"\
	{$(INCLUDE)}"config\_epilog.h"\
	{$(INCLUDE)}"config\_msvc_warnings_off.h"\
	{$(INCLUDE)}"config\_prolog.h"\
	{$(INCLUDE)}"config\stl_apcc.h"\
	{$(INCLUDE)}"config\stl_apple.h"\
	{$(INCLUDE)}"config\stl_as400.h"\
	{$(INCLUDE)}"config\stl_bc.h"\
	{$(INCLUDE)}"config\stl_como.h"\
	{$(INCLUDE)}"config\stl_confix.h"\
	{$(INCLUDE)}"config\stl_dec.h"\
	{$(INCLUDE)}"config\stl_dec_vms.h"\
	{$(INCLUDE)}"config\stl_fujitsu.h"\
	{$(INCLUDE)}"config\stl_gcc.h"\
	{$(INCLUDE)}"config\stl_hpacc.h"\
	{$(INCLUDE)}"config\stl_ibm.h"\
	{$(INCLUDE)}"config\stl_intel.h"\
	{$(INCLUDE)}"config\stl_kai.h"\
	{$(INCLUDE)}"config\stl_msvc.h"\
	{$(INCLUDE)}"config\stl_mwerks.h"\
	{$(INCLUDE)}"config\stl_mycomp.h"\
	{$(INCLUDE)}"config\stl_sco.h"\
	{$(INCLUDE)}"config\stl_select_lib.h"\
	{$(INCLUDE)}"config\stl_sgi.h"\
	{$(INCLUDE)}"config\stl_solaris.h"\
	{$(INCLUDE)}"config\stl_sunpro.h"\
	{$(INCLUDE)}"config\stl_symantec.h"\
	{$(INCLUDE)}"config\stl_watcom.h"\
	{$(INCLUDE)}"config\stl_wince.h"\
	{$(INCLUDE)}"config\stlcomp.h"\
	{$(INCLUDE)}"config\vc_select_lib.h"\
	{$(INCLUDE)}"hash_map"\
	{$(INCLUDE)}"hash_set"\
	{$(INCLUDE)}"pthread.h"\
	{$(INCLUDE)}"stl\_abbrevs.h"\
	{$(INCLUDE)}"stl\_algobase.c"\
	{$(INCLUDE)}"stl\_algobase.h"\
	{$(INCLUDE)}"stl\_alloc.c"\
	{$(INCLUDE)}"stl\_alloc.h"\
	{$(INCLUDE)}"stl\_alloc_old.h"\
	{$(INCLUDE)}"stl\_bvector.h"\
	{$(INCLUDE)}"stl\_config.h"\
	{$(INCLUDE)}"stl\_config_compat.h"\
	{$(INCLUDE)}"stl\_config_compat_post.h"\
	{$(INCLUDE)}"stl\_construct.h"\
	{$(INCLUDE)}"stl\_epilog.h"\
	{$(INCLUDE)}"stl\_function_base.h"\
	{$(INCLUDE)}"stl\_hash_fun.h"\
	{$(INCLUDE)}"stl\_hash_map.h"\
	{$(INCLUDE)}"stl\_hash_set.h"\
	{$(INCLUDE)}"stl\_hashtable.c"\
	{$(INCLUDE)}"stl\_hashtable.h"\
	{$(INCLUDE)}"stl\_iterator.h"\
	{$(INCLUDE)}"stl\_iterator_base.h"\
	{$(INCLUDE)}"stl\_iterator_old.h"\
	{$(INCLUDE)}"stl\_pair.h"\
	{$(INCLUDE)}"stl\_prolog.h"\
	{$(INCLUDE)}"stl\_pthread_alloc.c"\
	{$(INCLUDE)}"stl\_pthread_alloc.h"\
	{$(INCLUDE)}"stl\_ptrs_specialize.h"\
	{$(INCLUDE)}"stl\_range_errors.h"\
	{$(INCLUDE)}"stl\_relops_cont.h"\
	{$(INCLUDE)}"stl\_site_config.h"\
	{$(INCLUDE)}"stl\_sparc_atomic.h"\
	{$(INCLUDE)}"stl\_threads.c"\
	{$(INCLUDE)}"stl\_threads.h"\
	{$(INCLUDE)}"stl\_uninitialized.h"\
	{$(INCLUDE)}"stl\_vector.c"\
	{$(INCLUDE)}"stl\_vector.h"\
	{$(INCLUDE)}"stl\debug\_debug.c"\
	{$(INCLUDE)}"stl\debug\_debug.h"\
	{$(INCLUDE)}"stl\debug\_hashtable.h"\
	{$(INCLUDE)}"stl\debug\_iterator.h"\
	{$(INCLUDE)}"stl\debug\_vector.h"\
	{$(INCLUDE)}"stl\type_traits.h"\
	{$(INCLUDE)}"stl\wrappers\_hash_map.h"\
	{$(INCLUDE)}"stl\wrappers\_hash_set.h"\
	{$(INCLUDE)}"stl\wrappers\_vector.h"\
	{$(INCLUDE)}"stl_user_config.h"\
	
NODEP_CPP_GAMEO=\
	"..\..\..\..\usr\include\pthread.h"\
	
# End Source File
# Begin Source File

SOURCE=.\src\ManipulableObject.cpp
DEP_CPP_MANIP=\
	"..\..\dependencies\ogre\OgreMain\include\Ogre.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreActionEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreActionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimation.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimationState.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAnimationTrack.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreArchiveEx.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreArchiveManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAutoParamDataSource.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreAxisAlignedBox.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBillboard.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBillboardSet.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBlendMode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreBone.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreCamera.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreColourValue.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreCommon.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreConfig.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreConfigOptionMap.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreController.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreControllerManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreDataChunk.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreDynLibManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEntity.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventDispatcher.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventListeners.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventProcessor.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreEventTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreException.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreFrameListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreFrustum.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGpuProgram.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGpuProgramManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiContainer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiElement.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiElementCommands.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreGuiManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareBufferManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareIndexBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHardwareVertexBuffer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHighLevelGpuProgram.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreHighLevelGpuProgramManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreImage.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreInput.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreInputEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreIteratorWrappers.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreKeyFrame.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreKeyTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLight.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLog.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreLogManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterial.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterialManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMaterialSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMath.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMatrix3.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMatrix4.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMemoryMacros.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMemoryManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMeshSerializerImpl.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseMotionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMouseTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreMovableObject.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreNode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreOverlay.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreOverlayManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticle.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleAffector.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleEmitter.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleEmitterCommands.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleIterator.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleSystem.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreParticleSystemManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePass.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePatchMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePatchSurface.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlane.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlatform.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePlatformManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePositionTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePredefinedControllers.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgrePrerequisites.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreProfiler.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreProgressiveMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreQuaternion.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRay.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderable.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderOperation.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderQueue.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderQueueListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderSystem.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderSystemCapabilities.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTarget.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTargetListener.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderTexture.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRenderWindow.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreResource.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreResourceManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRoot.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreRotationalSpline.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneManagerEnumerator.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneNode.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSceneQuery.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreScrollEvent.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSDDataChunk.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSharedPtr.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSimpleRenderable.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSimpleSpline.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSingleton.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeleton.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeletonManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSkeletonSerializer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSphere.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStdHeaders.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreString.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringConverter.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringInterface.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreStringVector.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSubEntity.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreSubMesh.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTargetManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTechnique.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTexture.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTextureManager.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTextureUnitState.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreTimer.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreUserDefinedObject.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVector3.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVector4.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVertexBoneAssignment.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreVertexIndexData.h"\
	"..\..\dependencies\ogre\OgreMain\include\OgreViewport.h"\
	"..\..\dependencies\ruby\lib\ruby\1.8\i386-mswin32\config.h"\
	"..\common\include\RastullahPrerequisites.h"\
	".\include\GameObject.h"\
	".\include\ManipulableObject.h"\
	{$(INCLUDE)}"config\_epilog.h"\
	{$(INCLUDE)}"config\_msvc_warnings_off.h"\
	{$(INCLUDE)}"config\_prolog.h"\
	{$(INCLUDE)}"config\stl_apcc.h"\
	{$(INCLUDE)}"config\stl_apple.h"\
	{$(INCLUDE)}"config\stl_as400.h"\
	{$(INCLUDE)}"config\stl_bc.h"\
	{$(INCLUDE)}"config\stl_como.h"\
	{$(INCLUDE)}"config\stl_confix.h"\
	{$(INCLUDE)}"config\stl_dec.h"\
	{$(INCLUDE)}"config\stl_dec_vms.h"\
	{$(INCLUDE)}"config\stl_fujitsu.h"\
	{$(INCLUDE)}"config\stl_gcc.h"\
	{$(INCLUDE)}"config\stl_hpacc.h"\
	{$(INCLUDE)}"config\stl_ibm.h"\
	{$(INCLUDE)}"config\stl_intel.h"\
	{$(INCLUDE)}"config\stl_kai.h"\
	{$(INCLUDE)}"config\stl_msvc.h"\
	{$(INCLUDE)}"config\stl_mwerks.h"\
	{$(INCLUDE)}"config\stl_mycomp.h"\
	{$(INCLUDE)}"config\stl_sco.h"\
	{$(INCLUDE)}"config\stl_select_lib.h"\
	{$(INCLUDE)}"config\stl_sgi.h"\
	{$(INCLUDE)}"config\stl_solaris.h"\
	{$(INCLUDE)}"config\stl_sunpro.h"\
	{$(INCLUDE)}"config\stl_symantec.h"\
	{$(INCLUDE)}"config\stl_watcom.h"\
	{$(INCLUDE)}"config\stl_wince.h"\
	{$(INCLUDE)}"config\stlcomp.h"\
	{$(INCLUDE)}"config\vc_select_lib.h"\
	{$(INCLUDE)}"hash_map"\
	{$(INCLUDE)}"hash_set"\
	{$(INCLUDE)}"pthread.h"\
	{$(INCLUDE)}"stl\_abbrevs.h"\
	{$(INCLUDE)}"stl\_algobase.c"\
	{$(INCLUDE)}"stl\_algobase.h"\
	{$(INCLUDE)}"stl\_alloc.c"\
	{$(INCLUDE)}"stl\_alloc.h"\
	{$(INCLUDE)}"stl\_alloc_old.h"\
	{$(INCLUDE)}"stl\_bvector.h"\
	{$(INCLUDE)}"stl\_config.h"\
	{$(INCLUDE)}"stl\_config_compat.h"\
	{$(INCLUDE)}"stl\_config_compat_post.h"\
	{$(INCLUDE)}"stl\_construct.h"\
	{$(INCLUDE)}"stl\_epilog.h"\
	{$(INCLUDE)}"stl\_function_base.h"\
	{$(INCLUDE)}"stl\_hash_fun.h"\
	{$(INCLUDE)}"stl\_hash_map.h"\
	{$(INCLUDE)}"stl\_hash_set.h"\
	{$(INCLUDE)}"stl\_hashtable.c"\
	{$(INCLUDE)}"stl\_hashtable.h"\
	{$(INCLUDE)}"stl\_iterator.h"\
	{$(INCLUDE)}"stl\_iterator_base.h"\
	{$(INCLUDE)}"stl\_iterator_old.h"\
	{$(INCLUDE)}"stl\_pair.h"\
	{$(INCLUDE)}"stl\_prolog.h"\
	{$(INCLUDE)}"stl\_pthread_alloc.c"\
	{$(INCLUDE)}"stl\_pthread_alloc.h"\
	{$(INCLUDE)}"stl\_ptrs_specialize.h"\
	{$(INCLUDE)}"stl\_range_errors.h"\
	{$(INCLUDE)}"stl\_relops_cont.h"\
	{$(INCLUDE)}"stl\_site_config.h"\
	{$(INCLUDE)}"stl\_sparc_atomic.h"\
	{$(INCLUDE)}"stl\_threads.c"\
	{$(INCLUDE)}"stl\_threads.h"\
	{$(INCLUDE)}"stl\_uninitialized.h"\
	{$(INCLUDE)}"stl\_vector.c"\
	{$(INCLUDE)}"stl\_vector.h"\
	{$(INCLUDE)}"stl\debug\_debug.c"\
	{$(INCLUDE)}"stl\debug\_debug.h"\
	{$(INCLUDE)}"stl\debug\_hashtable.h"\
	{$(INCLUDE)}"stl\debug\_iterator.h"\
	{$(INCLUDE)}"stl\debug\_vector.h"\
	{$(INCLUDE)}"stl\type_traits.h"\
	{$(INCLUDE)}"stl\wrappers\_hash_map.h"\
	{$(INCLUDE)}"stl\wrappers\_hash_set.h"\
	{$(INCLUDE)}"stl\wrappers\_vector.h"\
	{$(INCLUDE)}"stl_user_config.h"\
	
NODEP_CPP_MANIP=\
	"..\..\..\..\usr\include\pthread.h"\
	
# End Source File
# End Group
# Begin Group "Headerdateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc;xsd"
# Begin Source File

SOURCE=.\include\Capability.h
# End Source File
# Begin Source File

SOURCE=.\include\DsaManager.h
# End Source File
# Begin Source File

SOURCE=.\include\GameObject.h
# End Source File
# Begin Source File

SOURCE=.\include\ManipulableObject.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe;resx"
# End Group
# End Target
# End Project
