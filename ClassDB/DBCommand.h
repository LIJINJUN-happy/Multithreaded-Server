#ifndef _DBCOMMAND_H_
#define _DBCOMMAND_H_

#include <string>

namespace DBCommand
{
	//�鿴���ݿ��Ƿ��������ע���
	const std::string JudegeEmailBrandNew = "SELECT account.actorId FROM actor.account WHERE account.emailAddress=";
	const std::string MakeAccount = "INSERT INTO actor.account VALUES()";

}

#endif
