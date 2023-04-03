--------------ROOM---------------------------------------------

--每个Actor Uid对应一个coroutine对象
local actorCoroutineMap_ = {}

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
	if actorCoroutineMap_[uid] and type(actorCoroutineMap_[uid]) == "thread" and coroutine.status(actorCoroutineMap_[uid]) ~= "dead" then
		return coroutine.resume(actorCoroutineMap_[uid], uid, call, called, fun, arg)
	elseif (not actorCoroutineMap_[uid]) or (type(actorCoroutineMap_[uid]) == "thread" and coroutine.status(actorCoroutineMap_[uid]) == "dead") then
		actorCoroutineMap_[uid] = coroutine.create(Interface_)
		return coroutine.resume(actorCoroutineMap_[uid], uid, call, called, fun, arg)
	end
end

--模块初始化函数
function DoInit_(serPath)
	return true
end


----------------------------------------------------Moudle-FUN-------------------------------------------------------
RANK = {}