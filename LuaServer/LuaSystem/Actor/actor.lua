--------------ACTOR--------------------------------------------

--用户的co变量，每次任务都是一次co的运行
local actorCoroutine_ = nil

--接口容器
local InterfaceFunMap_ = {}
local function loadInterFace()
	InterfaceFunMap_["AddScore"] = ACTOR.AddScore
	InterfaceFunMap_["GetScore"] = ACTOR.GetScore
end

--[[
接口函数:
call      @传递方
called    @接收方
fun       @处理协议/调用接口
arg       @参数
--]]
local function Interface_(uid, call, called, fun, arg)
	--判断模块名
	if _G[called] == nil then
		print("Called Erro "..tostring(called))
		return
	end

	--判断处理接口
	if InterfaceFunMap_[fun] and type(InterfaceFunMap_[fun]) == "function" then
		return InterfaceFunMap_[fun](_G[called], uid, arg)
	else
		print("fun Erro "..tostring(fun))
		return
	end
end

--交互函数（C++调用此函数进行通信）
function Main_(uid, call, called, fun, arg)
	if actorCoroutine_ and type(actorCoroutine_) == "thread" and coroutine.status(actorCoroutine_) ~= "dead" then
		return coroutine.resume(actorCoroutine_, uid, call, called, fun, arg)
	elseif (not actorCoroutine_) or (type(actorCoroutine_) == "thread" and coroutine.status(actorCoroutine_) == "dead") then
		actorCoroutine_ = coroutine.create(Interface_)
		return coroutine.resume(actorCoroutine_, uid, call, called, fun, arg)
	end
end

--模块初始化函数
function DoInit_(serPath)
	--JSON
	JSON = dofile(serPath .. "LuaServer/luaLib/json.lua")

	--Bag
	dofile(serPath .. "LuaServer/LuaSystem/Bag/bagMgr.lua")

	--Email
	dofile(serPath .. "LuaServer/LuaSystem/Email/email.lua")

	--Redis
	--REDISMOUDLE = require("redis")
	--local REDIS = REDISMOUDLE.connect('127.0.0.1', 6379)

	--加载接口函数
	loadInterFace()
	return true
end

--登录时加载数据进Redis缓存
function LoadDbData_(...)
	local list = table.pack(...)
	for _,dataString in ipairs(list) do
		local moudleName,data = string.match(dataString, "(%a+)%s*::%s*(%a+)")
		print("dataString === ",dataString)
		print("moudleName === ",moudleName," data === ",data)
		--REDIS:set("k1","v1")
	end
	return true
end

--用户下线的数据保存处理
function SaveDbData_(uid, moudle)
	local josnDataString = ""
	return josnDataString
end

----------------------------------------------------Moudle-FUN-------------------------------------------------------

--用户模块
ACTOR = {
	["score"] = 0
}

--获取信息
function ACTOR:GetData(uid)
	
end

--改变名字
function ACTOR:ChangeName(newName)
end

--添加积分
function ACTOR:AddScore(uid, arg)
	local info = JSON:decode(arg)
	local num = info.score or 0
	self.score = self.score + num
	print("添加后当前积分为:", self.score)
	coroutine.yield(tostring(self.score), "AddScore", "RANK", "ACTOR", tostring(uid))
	print("RANK BACK-----")
	local res = LuaSendMessage(num, "Get Message from Server")
	print("LuaSendMessage = "..tostring(res))
	return self.score
end

--show score
function ACTOR:GetScore(uid, arg)
	print("The CurScore is ", self.score)
	return self.score
end

