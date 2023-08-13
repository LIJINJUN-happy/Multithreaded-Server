//�������ݴ���ģ��
#ifndef _OFFLINESYSTEM_H_
#define _OFFLINESYSTEM_H_

#include "../GlobalVariable.h"


//--------------------------BEGIN------����������(��ʹ�û�������,Ҳ���Դ�����ȡ������)
typedef struct OffLineData
{
public:

	OffLineData();

	~OffLineData();

	//�ж��Ƿ��и��û��������ݴ���
	bool CheckoutActorOffLineData(std::string uid);

	//��ȡ���û���������
	std::string FindoutActorOffLineData(std::string uid);

	//������������
	void AddData(std::string uid);

	//������������
	void UpdateData(std::string uid, std::string jsonData);

private:
	Redis* redisObj;	//redis���󣨼��غʹ洢�������ݣ�

} OffLineData;
//--------------------------END------����������(��ʹ�û�������,Ҳ���Դ�����ȡ������)





//--------------------------BEGIN------��¼���������ݣ��û���¼��ʱ��������鿴��

struct LoginData
{
public:
	pthread_mutex_t LoginDataLock;	//������ֹͬʱȡ�������¼���
	std::string dataString = "";				//�¼�

	LoginData() {
		pthread_mutex_init(&LoginDataLock, NULL);	//��ʼ����ʱ��������
	};

	~LoginData(){};
};


typedef struct LoginOffLineMsg
{

private:

	std::map<std::string, LoginData*> loginOffLineMsgMap;		//UID---�����������ַ���
	std::map<std::string, LoginData*>* loginOffLineMsgMapPtr;

public:

	LoginOffLineMsg();

	~LoginOffLineMsg();

	//����������ַ
	std::map<std::string, LoginData*>* GetLoginOffLineMsgMapPtr();

	//�жϸ��û��Ƿ���������Ҫ�ڵ�¼�����û������ߵ��ǻ�������,�����ߵ�ʱ���ٴ���
	bool CheckLoginOffLineData(std::string uid);

	//��ȡ���û���¼���账�����ݵ�����
	LoginData* GetLoginOffLineData(std::string uid);

	//ˢ�µ�¼����������
	void UpdateLoginOffLineData(std::string uid, LoginData* data);

	//������¼����������
	void AddLoginOffLineData(std::string uid, std::string data);

	//���ز�ɾ�������������еĵ�һ������(����֮���ã������ŷָ�����)
	std::string GetData(std::string uid);

} LoginOffLineMsg;
//--------------------------END------��¼���������ݣ��û���¼��ʱ��������鿴��






//--------------------------BEGIN------����ϵͳ
class OffLineSystem
{
public:
	OffLineSystem();

	~OffLineSystem();

public:

	//��ȡ���߹�������ַ
	OffLineData* GetOffLineDataPtr();

	//��ȡ��¼���������ݹ�������ַ
	LoginOffLineMsg* GetLoginOffLineMsgPtr();

private:
	OffLineData* OffLineDataPtr;
	LoginOffLineMsg* LoginOffLineMsgPtr;
};
//--------------------------END------����ϵͳ

#endif