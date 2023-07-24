#ifndef _DBCOMMAND_H_
#define _DBCOMMAND_H_

#include <string>

namespace DBCommand
{

/*-----------------------------------------ACTOR--------------------------------------------------------------------------*/
	//�鿴���ݿ��Ƿ��������ע���
	const std::string JudegeEmailBrandNew = "SELECT actor.account FROM actor.actor WHERE actor.emailAddress=";
	
	//�����û���������
	const std::string MakeAccount = "INSERT INTO actor.actor VALUES()";

	//��֤�˺�����
	const std::string CheckoutPassword = "SELECT actor.actorId FROM actor.actor WHERE actor.account='' AND actor.password=''";

	//����ĳ��ģ������ݣ��������ݿ���ĳ���Ӧ��json��ʽ��DB���ݣ�
	const std::string LoadLuaDataFromMysql = "SELECT .script FROM actor. WHERE .actorId=''";

	//����ĳ��ģ�������,���ݿ������ݣ���Ҫ���� insert��redis's json to mysql��
	const std::string SaveLuaDataWithInsert = "INSERT INTO actor. VALUES('','')";

	//����ĳ��ģ�������,���ݿ������ݣ�ֻ����� update��redis's json to mysql��
	const std::string SaveLuaDataWithUpdate = "UPDATE actor. SET .script='' WHERE .actorId=''";



/*-----------------------------------------OFFLINEDATA--------------------------------------------------------------------------*/
	//������������
	const std::string loadOffLine = "SELECT * FROM offlinedata.offlinedata";

	//������������(��һ�δ���)
	const std::string saveOffLineWithInsert = "INSERT INTO offlinedata.offlinedata VALUES(_)";

	//������������(����)
	const std::string saveOffLineWithUpdate = "UPDATE offlinedata.offlinedata SET offlinedata.script='_' WHERE offlinedata.actorId='_'";

	//����ĳ���û���������
	const std::string checkOffLineWithSelect = "SELECT * FROM offlinedata.offlinedata WHERE offlinedata.actorId=_";
	
}

#endif
