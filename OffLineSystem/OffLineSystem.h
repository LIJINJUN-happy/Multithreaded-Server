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

public:

	//��Mysql�����������ݽ�Redis
	void DoLoad();

	//��Redisȡ���������ݴ洢��Mysql
	void DoSave();

} OffLineData;
//--------------------------END------����������(��ʹ�û�������,Ҳ���Դ�����ȡ������)





//--------------------------BEGIN------��¼���������ݣ��û���¼��ʱ��������鿴��
typedef struct LoginOffLineMsg
{
private:

	std::map<std::string, std::string> loginOffLineMsgMap;		//UID---�����������ַ���
	std::map<std::string, std::string>* loginOffLineMsgMapPtr;

public:

	LoginOffLineMsg();

	~LoginOffLineMsg();

	//����������ַ
	std::map<std::string, std::string>* GetLoginOffLineMsgMapPtr();

	//�жϸ��û��Ƿ���������Ҫ�ڵ�¼�����û������ߵ��ǻ�������,�����ߵ�ʱ���ٴ���
	bool CheckLoginOffLineData(std::string uid);

	//��ȡ���û���¼���账�����ݵ�����
	std::string& GetLoginOffLineData(std::string uid);

	//ˢ�µ�¼����������
	void UpdateLoginOffLineData(std::string uid, std::string data);	

	//������¼����������
	void AddLoginOffLineData(std::string uid, std::string data);

	//���ز�ɾ�������������еĵ�һ������
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