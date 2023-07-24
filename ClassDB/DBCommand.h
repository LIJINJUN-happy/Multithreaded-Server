#ifndef _DBCOMMAND_H_
#define _DBCOMMAND_H_

#include <string>

namespace DBCommand
{

/*-----------------------------------------ACTOR--------------------------------------------------------------------------*/
	//查看数据库是否此邮箱已注册过
	const std::string JudegeEmailBrandNew = "SELECT actor.account FROM actor.actor WHERE actor.emailAddress=";
	
	//创建用户插入数据
	const std::string MakeAccount = "INSERT INTO actor.actor VALUES()";

	//验证账号密码
	const std::string CheckoutPassword = "SELECT actor.actorId FROM actor.actor WHERE actor.account='' AND actor.password=''";

	//加载某个模块的数据（返回数据库中某块对应的json格式的DB数据）
	const std::string LoadLuaDataFromMysql = "SELECT .script FROM actor. WHERE .actorId=''";

	//保存某个模块的数据,数据库无数据，需要插入 insert（redis's json to mysql）
	const std::string SaveLuaDataWithInsert = "INSERT INTO actor. VALUES('','')";

	//保存某个模块的数据,数据库有数据，只需更新 update（redis's json to mysql）
	const std::string SaveLuaDataWithUpdate = "UPDATE actor. SET .script='' WHERE .actorId=''";



/*-----------------------------------------OFFLINEDATA--------------------------------------------------------------------------*/
	//加载离线数据
	const std::string loadOffLine = "SELECT * FROM offlinedata.offlinedata";

	//存入离线数据(第一次存入)
	const std::string saveOffLineWithInsert = "INSERT INTO offlinedata.offlinedata VALUES(_)";

	//存入离线数据(更新)
	const std::string saveOffLineWithUpdate = "UPDATE offlinedata.offlinedata SET offlinedata.script='_' WHERE offlinedata.actorId='_'";

	//查找某个用户离线数据
	const std::string checkOffLineWithSelect = "SELECT * FROM offlinedata.offlinedata WHERE offlinedata.actorId=_";
	
}

#endif
