协议定义的描述：
协议使用的是：JSON格式协议

格式：
{
	"Moudle": "模块名",
	"Protocol": "协议名",
	"参数字段1": "数据1", 
	"参数字段2": "数据2", 
	...
}

例子：
//登录请求
{
	"Moudle": "GATE",
	"Protocol": "c_login_request",
	"Account": "string",
	"Password": "string"
}

##文件名全小写
##文件内容里的模块名使用全大写,如："Moudle": "GATE",
##协议名 客户端发送给服务器：c_xxxx  反之：s_xxxx