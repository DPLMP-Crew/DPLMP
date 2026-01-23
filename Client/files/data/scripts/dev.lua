local MaxCars = 5

local SpawnedCars = {}
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

local Skins = { 
	0x52,
	0x53,
	0x54,
	0x3D,
	0x6F,
	0x3E,
	0x70,
	0x71,
	0xA7,
	0x23,
	0xB6,
	0xB5,
	0xC5,
	0xC4,
	0x98,
	0x8E,
	0x9C,
	0x8F,
	0xB2,
	0xBA,
	0xB9,
	0xC9
}

local function Spawn(character, spawnPoint, health)
	character:Revive()
	character:SetPosition(spawnPoint.x, spawnPoint.y, spawnPoint. z)
	character:SetRotation(spawnPoint.qx, spawnPoint.qy, spawnPoint.qz, spawnPoint.qw)
	character:SetHealth(health)
	local skin = Skins[math.random(#Skins)]
	character:SwapSkin(skin, skin)
end

local function RespawnRoutine()
	coroutine.yield(3)
	local ply = CharacterManager.GetPlayerCharacter()
	Spawn(ply, SpawnPoints[math.random(#SpawnPoints)], 1.0)
end

UI.ShowNotification("Dev Mode", 5.0, true)
local ply = CharacterManager.GetPlayerCharacter()
Spawn(ply, SpawnPoints[math.random(#SpawnPoints)], 1.0)

local vehicleSpawnCoroutine = 0
local vehicleSpawnRequest = nil

local function VehicleSpawnLoop()
	local vehId = 0
	while true do
		UI.ShowNotification(vehId .. " (Arrow Keys to Cycle)", 5.0, false)
	
		if (Input.KeyPressed(0x27)) then
			vehId = vehId + 1
			if (vehId > 97) then
				vehId = 0
			end
			if (vehId == 55) then
				vehId = vehId + 1
			end
			if (vehicleSpawnRequest ~= nil) then
				VehicleManager.DestroyVehicle(vehicleSpawnRequest)
				vehicleSpawnRequest = nil
			end
		end
		
		if (Input.KeyPressed(0x25)) then
			vehId = vehId - 1
			if (vehId < 0) then
				vehId = 97
			end
			if (vehId == 55) then
				vehId = vehId - 1
			end
			if (vehicleSpawnRequest ~= nil) then
				VehicleManager.DestroyVehicle(vehicleSpawnRequest)
				vehicleSpawnRequest = nil
			end
		end
		
		if (vehicleSpawnRequest == nil) then
			local ply = CharacterManager.GetPlayerCharacter()
			local x, y, z = ply:GetPosition()
			
			vehicleSpawnRequest = VehicleManager.CreateVehicle(vehId, x, y, z - 3, 1.5708, true)
		end

		coroutine.yield(0)
	end
end

local function OnFrameUpdate(delta)
	if (Input.KeyPressed(118)) then
		if (vehicleSpawnCoroutine == 0) then
			vehicleSpawnCoroutine = Core.CreateCoroutine(VehicleSpawnLoop)
		else
			Core.AbortCoroutine(vehicleSpawnCoroutine)
			vehicleSpawnCoroutine = 0
			SpawnedCars[#SpawnedCars + 1] = vehicleSpawnRequest
			if (#SpawnedCars > MaxCars) then
				VehicleManager.DestroyVehicle(SpawnedCars[1])
				table.remove(SpawnedCars, 1)
			end
			vehicleSpawnRequest = nil
			UI.ClearNotification()
		end
	end
end

Hooks.AddHook(Hooks.OnLocalPlayerDead, RespawnRoutine)
Hooks.AddHook(Hooks.OnFrameUpdate, OnFrameUpdate)