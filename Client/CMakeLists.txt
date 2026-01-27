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
        src/dpl/CCharacterCategoryManager.cpp
        src/dpl/CPlaylist.cpp
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
    d3d9
    d3dx9
    glm
    lua::lua
    sol2
    xxHashStatic
    d3dcompiler
    ENetStatic
    Shared
)

if (DPLMP_USE_SAFETYHOOK)
target_link_libraries(DPLMPClient PRIVATE
    safetyhook
)
target_compile_definitions(DPLMPClient PRIVATE LDSCAN_SAFETYHOOK)
else ()
target_link_libraries(DPLMPClient PRIVATE
    minhook
)
target_include_directories(DPLMPClient
    PRIVATE
        ${minhook_SOURCE_DIR}/include
)
target_compile_definitions(DPLMPClient PRIVATE LDSCAN_BRUTEFORCE_MINHOOK)
target_compile_definitions(DPLMPClient PRIVATE LDSCAN_SUSPEND_MINHOOK)
endif ()

add_custom_target(BuildLauncher
    COMMAND dotnet build
        "${CMAKE_SOURCE_DIR}/Launcher/Launcher.sln"
        -c $<IF:$<CONFIG:Debug>,Debug,Release>
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Launcher"
)

add_dependencies(DPLMPClient BuildLauncher)

set(LAUNCHER_OUT
    "${CMAKE_SOURCE_DIR}/Launcher/Launcher/bin/x86/$<IF:$<CONFIG:Debug>,Debug,Release>/net48"
)

set(CLIENT_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/files"
)

install(CODE [[
  file(REMOVE_RECURSE "${CMAKE_INSTALL_PREFIX}")
]])

install(TARGETS DPLMPClient
    RUNTIME DESTINATION .
)

install(DIRECTORY
    ${LAUNCHER_OUT}/
    DESTINATION .
    FILES_MATCHING
        PATTERN "*.exe"
        PATTERN "*.dll"
)

install(DIRECTORY
    ${CLIENT_FILES}/
    DESTINATION .
)