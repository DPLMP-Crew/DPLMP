UI.ShowNotification("Dev Mode", 5.0, true)

local function RespawnRoutine()
	coroutine.yield(3.0)
	local ply = CharacterManager.GetPlayerCharacter()
	ply:Revive()
end

local function TestCoroutine()
	print("Hi")
	coroutine.yield(1.0)
	print("Hi again")
end

local function OnPlayerDead()
	print("Start Respawn")
	coroutine.yield(1.0)
	print("Respawning in 3...")
	coroutine.yield(1.0)
	print("Respawning in 2...")
	coroutine.yield(1.0)
	print("Respawning in 1...")
	coroutine.yield(1.0)
	print("RESPAWN!")
end

local function OnFrameUpdate(delta)
	if (Input.KeyPressed(118)) then
		UI.ShowNotification("Hello :)", 5.0, true)
	end
end

Hooks.AddHook(Hooks.OnLocalPlayerDead, OnPlayerDead)
Hooks.AddHook(Hooks.OnFrameUpdate, OnFrameUpdate)

local testCoroutine = Core.CreateCoroutine(TestCoroutine)
Core.AbortCoroutine(testCoroutine)