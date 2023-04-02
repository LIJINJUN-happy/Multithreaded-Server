--------------ACTOR--------------------------------------------

--用户的co变量，每次任务都是一次co的运行
local actorCoroutine_ = nil

--接口容器
local InterfaceFunMap_ = {
  -----------------------------pto-begin------------------------
  
  
  -----------------------------pto-end--------------------------
  
  }

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
    return InterfaceFunMap_[fun](uid, arg)
  else
    print("fun Erro "..tostring(fun))
    return
  end
end

--交互函数（C++调用此函数进行通信）
function Main_(uid, call, called, fun, arg)
	if actorCoroutine_ and type(actorCoroutine_) == "thread" and coroutine.status(actorCoroutine_) ~= "dead" then
		return coroutine.resume(actorCoroutine_,uid, call, called, fun, arg)
	elseif (not actorCoroutine_) or (type(actorCoroutine_) == "thread" and coroutine.status(actorCoroutine_) == "dead") then
		actorCoroutine_ = coroutine.create(Interface_)
		return coroutine.resume(actorCoroutine_,uid, call, called, fun, arg)
	end
end

--模块初始化函数
function DoInit_(path)
	return true
end


----------------------------------------------------Moudle-FUN-------------------------------------------------------

--用户模块
ACTOR = {}

--获取信息
function ACTOR:GetData()
end

--改变名字
function ACTOR:ChangeName(newName)
end

