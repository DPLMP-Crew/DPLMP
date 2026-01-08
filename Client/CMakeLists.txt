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
)

target_include_directories(DPLMPClient
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(DPLMPClient PRIVATE 
    safetyhook
    d3d9
)