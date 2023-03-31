--每个Actor Uid对应一个coroutine对象
local actorCoroutineMap = {}

local function Interface()
end

function Main()
	if actorCoroutineMap[uid] and type(actorCoroutineMap[uid]) == "thread" and coroutine.status(actorCoroutineMap[uid]) ~= "dead" then
		return coroutine.resume(actorCoroutineMap[uid],参数)
	elseif not actorCoroutineMap[uid] or (type(actorCoroutineMap[uid]) == "thread" and coroutine.status(actorCoroutineMap[uid]) == "dead") then
		actorCoroutineMap[uid] = coroutine.create(Interface)
		return coroutine.resume(actorCoroutineMap[uid],参数)
	end
end