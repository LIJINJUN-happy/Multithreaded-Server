--ÿ��Actor Uid��Ӧһ��coroutine����
local actorCoroutineMap_ = {}

--�ӿں���
local function Interface_()
end

--����������C++���ô˺�������ͨ�ţ�
function Main_()
	if actorCoroutineMap_[uid] and type(actorCoroutineMap_[uid]) == "thread" and coroutine.status(actorCoroutineMap_[uid]) ~= "dead" then
		return coroutine.resume(actorCoroutineMap_[uid],����)
	elseif (not actorCoroutineMap_[uid]) or (type(actorCoroutineMap_[uid]) == "thread" and coroutine.status(actorCoroutineMap_[uid]) == "dead") then
		actorCoroutineMap_[uid] = coroutine.create(Interface)
		return coroutine.resume(actorCoroutineMap_[uid],����)
	end
end

--ģ���ʼ������
function DoInit_()
	return true
end
