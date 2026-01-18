local SpawnPoint = {}

function SpawnPoint.New(x, y, z, qx, qy, qz, qw)
	return {
		x = x,
		y = y,
		z = z,
		qx = qx,
		qy = qy,
		qz = qz,
		qw = qw
	}
end

local SpawnPoints = {
	SpawnPoint.New(2454.5952148438, 40.045989990234, -4359.3793945312, 0.0, 0.99991106987, 0.0, -0.013335934840143),
	SpawnPoint.New(-2189.3334960938, 0.067904889583588, 3787.9995117188, 0.0, 0.73034512996674, 0.0, -0.68307834863663),
	SpawnPoint.New(4563.1616210938, 19.669435501099, -539.35107421875, 0.0, 0.71786147356033, 0.0, 0.69618600606918)
}

local function Spawn(character, spawnPoint, health)
	character:Revive()
	character:SetPosition(spawnPoint.x, spawnPoint.y, spawnPoint. z)
	character:SetRotation(spawnPoint.qx, spawnPoint.qy, spawnPoint.qz, spawnPoint.qw)
	character:SetHealth(health)
end

UI.ShowNotification("Dev Mode", 5.0, true)
local ply = CharacterManager.GetPlayerCharacter()
	Spawn(ply, SpawnPoints[math.random(#SpawnPoints)], 1.0)

local function RespawnRoutine()
	coroutine.yield(3.0)
	local ply = CharacterManager.GetPlayerCharacter()
	Spawn(ply, SpawnPoints[math.random(#SpawnPoints)], 1.0)
end

local function OnFrameUpdate(delta)
	if (Input.KeyPressed(118)) then
		local ply = CharacterManager.GetPlayerCharacter()
		
		local rx, ry, rz, rw = ply:GetRotation()
		local x, y, z = ply:GetPosition()
		
		print("Pos: ( " .. x .. ", " .. y .. ", " .. z .. " )")
		print("Rot: ( " .. rx .. ", " .. ry .. ", " .. rz .. ", " .. rw .. " )")
	end
end

Hooks.AddHook(Hooks.OnLocalPlayerDead, RespawnRoutine)
Hooks.AddHook(Hooks.OnFrameUpdate, OnFrameUpdate)