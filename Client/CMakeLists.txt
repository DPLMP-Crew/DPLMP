add_library(DPLMPClient SHARED)

target_compile_definitions(DPLMPClient
    PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
)

target_sources(DPLMPClient
    PRIVATE
        src/dllmain.cpp
        src/Core.cpp
	    src/dpl/CLifeEventDataManager.cpp
        src/scan.cpp
        src/dpl/SpoolableResourceManager.cpp
        src/dpl/CLoadingScreen.cpp
        src/dpl/CCharacter.cpp
        src/dpl/CCharacterManager.cpp
        src/dpl/CWeapon.cpp
        src/dpl/CPCViewport.cpp
        src/dpl/CVehicleManager.cpp
        src/dpl/CVehicle.cpp
        src/dpl/LifeAcquirableVehicleManager.cpp
        src/dpl/AIManager.cpp
        src/dpl/CharacterAIManager.cpp
        src/dpl/CLifeInstance_Character.cpp
        src/dpl/Matrix.cpp
        src/D3DHooks.cpp
        src/dpl/GameCamera.cpp
        src/dpl/CLifePlayer.cpp
        src/dpl/CLifeSystem.cpp
        src/dpl/CPlayerDriverBehaviour.cpp
        src/dpl/CWipeManager.cpp
        src/LuaEngine.cpp
        src/dpl/UINotification.cpp
        src/LuaUI.cpp
        src/LuaHooks.cpp
        src/LuaCallback.cpp
        src/LuaScheduler.cpp
        src/LuaInput.cpp
        src/LuaCore.cpp
        src/IdManager.cpp
        src/LuaCharacter.cpp
        src/ResourceManager.cpp
        src/LuaVehicle.cpp
        src/VehicleSpawner.cpp
        src/dpl/CHandling.cpp
)

target_include_directories(DPLMPClient
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
)

target_link_directories(DPLMPClient 
    PRIVATE
        "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
)

target_link_libraries(DPLMPClient PRIVATE 
    safetyhook
    d3d9
    d3dx9
    glm
    lua::lua
    sol2
    xxHashStatic
)