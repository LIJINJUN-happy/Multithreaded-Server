--每个Actor Uid对应一个coroutine对象
local actorCoroutineMap_ = {}

--接口函数
local function Interface_()
end

--交互函数（C++调用此函数进行通信）
function Main_()
	if actorCoroutineMap_[uid] and type(actorCoroutineMap_[uid]) == "thread" and coroutine.status(actorCoroutineMap_[uid]) ~= "dead" then
		return coroutine.resume(actorCoroutineMap_[uid],arg)
	elseif (not actorCoroutineMap_[uid]) or (type(actorCoroutineMap_[uid]) == "thread" and coroutine.status(actorCoroutineMap_[uid]) == "dead") then
		actorCoroutineMap_[uid] = coroutine.create(Interface)
		return coroutine.resume(actorCoroutineMap_[uid],arg)
	end
end

--模块初始化函数
function DoInit_()
	return true
end
