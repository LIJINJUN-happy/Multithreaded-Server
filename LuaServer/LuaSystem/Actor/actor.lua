--�û���co������ÿ��������һ��co������
local actorCoroutine_ = nil

--�ӿں���
local function Interface_()
end

--����������C++���ô˺�������ͨ�ţ�
function Main_()
	if actorCoroutine_ and type(actorCoroutine_) == "thread" and coroutine.status(actorCoroutine_) ~= "dead" then
		return coroutine.resume(actorCoroutine_,����)
	elseif (not actorCoroutine_) or (type(actorCoroutine_) == "thread" and coroutine.status(actorCoroutine_) == "dead") then
		actorCoroutine_ = coroutine.create(Interface)
		return coroutine.resume(actorCoroutine_,����)
	end
end

--ģ���ʼ������
function DoInit_()
	return true
end

--�û�ģ��
ACTOR = {}