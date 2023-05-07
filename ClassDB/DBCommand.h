#ifndef _DBCOMMAND_H_
#define _DBCOMMAND_H_

#include <string>

namespace DBCommand
{
	//查看数据库是否此邮箱已注册过
	const std::string JudegeEmailBrandNew = "SELECT account.actorId FROM actor.account WHERE account.emailAddress=";
	const std::string MakeAccount = "INSERT INTO actor.account VALUES()";

}

#endif
