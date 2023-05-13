#ifndef _DBCOMMAND_H_
#define _DBCOMMAND_H_

#include <string>

namespace DBCommand
{
	//查看数据库是否此邮箱已注册过
	const std::string JudegeEmailBrandNew = "SELECT actor.account FROM actor.actor WHERE actor.emailAddress=";
	
	//创建用户插入数据
	const std::string MakeAccount = "INSERT INTO actor.actor VALUES()";

	//验证账号密码
	const std::string CheckoutPassword = "SELECT actor.actorId FROM actor.actor WHERE actor.account='' AND actor.password=''";

	//加载某个模块的数据（返回数据库中某块对应的json格式的DB数据）
	const std::string LoadLuaDataFromMysql = "SELECT .script FROM actor. WHERE .actorId=''";
}

#endif
