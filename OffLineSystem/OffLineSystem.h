//�������ݴ���ģ��
#ifndef _OFFLINESYSTEM_H_
#define _OFFLINESYSTEM_H_

#include "../GlobalVariable.h"


//--------------------------BEGIN------����������(��ʹ�û�������,Ҳ���Դ�����ȡ������)
typedef struct OffLineData
{
private:
	std::map<std::string, std::string> offLineDataMap; //UID---JSON�����ַ���
	std::map<std::string, std::string>* offLineDataMapPtr;

public:
	OffLineData();
	~OffLineData();

public:
	std::map<std::string, std::string>* GetOffLineDataMapPtr();	//������������������ַ
	bool CheckoutActorOffLineData(std::string uid);				//�ж��Ƿ��и��û��������ݴ���
	std::string& FindoutActorOffLineData(std::string uid);		//��ȡ���û��������ݵ�����
	void AddData(std::string uid);								//������������
	void UpdateData(std::string uid, std::string jsonData);		//������������

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

public:
	std::map<std::string, std::string>* GetLoginOffLineMsgMapPtr();	//����������ַ
	bool CheckLoginOffLineData(std::string uid);					//�жϸ��û��Ƿ���������Ҫ�ڵ�¼�����û������ߵ��ǻ�������,�����ߵ�ʱ���ٴ���
	std::string& GetLoginOffLineData(std::string uid);				//��ȡ���û���¼���账�����ݵ�����
	void AddLoginOffLineData(std::string uid, std::string data);	//��ӵ�¼����������

} LoginOffLineMsg;
//--------------------------END------��¼���������ݣ��û���¼��ʱ��������鿴��






//--------------------------BEGIN------����ϵͳ
class OffLineSystem
{
public:
	OffLineSystem();
	~OffLineSystem();

public:
	OffLineData* GetOffLineDataPtr();
	LoginOffLineMsg* GetLoginOffLineMsgPtr();

private:
	OffLineData* OffLineDataPtr;
	LoginOffLineMsg* LoginOffLineMsgPtr;
};
//--------------------------END------����ϵͳ

#endif-