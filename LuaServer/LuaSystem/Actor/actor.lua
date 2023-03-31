--用户的co变量，每次任务都是一次co的运行
local actorCoroutine_ = nil

--接口函数
local function Interface_()
end

--交互函数（C++调用此函数进行通信）
function Main_()
	if actorCoroutine_ and type(actorCoroutine_) == "thread" and coroutine.status(actorCoroutine_) ~= "dead" then
		return coroutine.resume(actorCoroutine_,参数)
	elseif (not actorCoroutine_) or (type(actorCoroutine_) == "thread" and coroutine.status(actorCoroutine_) == "dead") then
		actorCoroutine_ = coroutine.create(Interface)
		return coroutine.resume(actorCoroutine_,参数)
	end
end

--模块初始化函数
function DoInit_()
	return true
end

--用户模块
ACTOR = {}