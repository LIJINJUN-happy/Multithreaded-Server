#ifndef _DBCOMMAND_H_
#define _DBCOMMAND_H_

#include <string>

namespace DBCommand
{
	//查看数据库是否此邮箱已注册过
	const std::string JudegeEmailBrandNew = "SELECT account.account FROM actor.account WHERE account.emailAddress=";
	
	//创建用户插入数据
	const std::string MakeAccount = "INSERT INTO actor.account VALUES()";

	//验证账号密码
	const std::string CheckoutPassword = "SELECT account.actorId FROM actor.account WHERE account.account='' AND account.password=''";
}

#endif
