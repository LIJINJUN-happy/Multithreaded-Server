#ifndef _DBCOMMAND_H_
#define _DBCOMMAND_H_

#include <string>

namespace DBCommand
{
	//�鿴���ݿ��Ƿ��������ע���
	const std::string JudegeEmailBrandNew = "SELECT account.account FROM actor.account WHERE account.emailAddress=";
	
	//�����û���������
	const std::string MakeAccount = "INSERT INTO actor.account VALUES()";

	//��֤�˺�����
	const std::string CheckoutPassword = "SELECT account.actorId FROM actor.account WHERE account.account='' AND account.password=''";
}

#endif
