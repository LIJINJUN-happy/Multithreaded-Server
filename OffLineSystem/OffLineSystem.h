//�������ݴ���ģ��
#ifndef _OFFLINESYSTEM_H_
#define _OFFLINESYSTEM_H_

#include "../GlobalVariable.h"


//--------------------------BEGIN------����������(��ʹ�û�������,Ҳ���Դ�����ȡ������)
typedef struct OffLineData
{
public:
	std::map<std::string, std::string> offLineDataMap; //UID---JSON�����ַ���
	std::map<std::string, std::string>* offLineDataMapPtr;

public:
	OffLineData();
	~OffLineData();

	std::map<std::string, std::string>* GetOffLineDataMapPtr();	//������������������ַ
	bool CheckoutActorOffLineData(std::string uid);				//�ж��Ƿ��и��û��������ݴ���
	std::string& FindoutActorOffLineData(std::string uid);		//��ȡ���û��������ݵ�����
	void AddData(std::string uid);

} OffLineData;
//--------------------------END------����������(��ʹ�û�������,Ҳ���Դ�����ȡ������)





//--------------------------BEGIN------��¼�������ݣ��û���¼��ʱ��������鿴��
typedef struct LoginOffLineMsg
{

} LoginOffLineMsg;
//--------------------------END------��¼�������ݣ��û���¼��ʱ��������鿴��






//--------------------------BEGIN------����ϵͳ
class OffLineSystem
{
public:


protected:

private:

};
//--------------------------END------����ϵͳ

#endif-