公司网络硬盘：
YFB-YLF
YFB-YLF@#!qwa

svn：
https://49.235.190.180/svn/S01-CY-MQTT/
YuanLF
YuanLF@0112

https://git.xciotapp.com:3000	//在这注册git账号
Git： Github: GOgs：
XSylf2019
XSylf2019110lock

在git里输入cd SO1指向这个项目

git克隆项目步骤：
1.git config --global user.name "XSylf2019"		//设置用户名与邮箱
2.git config --global user.email 641133321@qq.com		//设置用户名与邮箱
3.git config --global http.sslVerify false			//SSL证书的问题，可以忽略证书继续重新执行

4.git clone https://git.xciotapp.com:3000/smartdevice/S01.git	//拉取代码	(这是默认路径)
/***********************************************/
4.git clone https://git.xciotapp.com:3000/smartdevice/S01.git /g/project/20191017	//指定克隆路径
/***********************************************/
5.到本地查看代码

git拉取项目步骤：
1.git pull

git提交项目步骤：
1.cd S01

1.cd /G/project/S01-CY/20191021/S01	//指向文件的路径
2.git status		//查看改动的文件(改动文件变红)
3.git add SmartLock_S01/LOCK/password_process/password_process.c	//提交改动的文件。
							//这里如果只有一个文件改动之传一次，如果是多个文件改动就重复多次这样操作
4.git status		//查看提交结果(改动文件变绿)
5.git commit -m "xxxxx"	//x..x 是输入修改的备注(优化虚位密码比对)
6.git push			//提交代码

第一次提交代码到Github
1.创建文件夹(新建一个本地仓库)
2.然后进去该文件夹。鼠标右键，打开git -> Git Bash Here -> git init
3.新建test.txt文本
4.git status 显示红色的就是添加的文件 N01_v1.1/
5.git add N01_v1.1/
6.git commit -m "测试代码"
7.去github上创建自己的Repository
8.git remote add origin https://github.com/XSylf2019/STM.git
9.git push -u origin master
以后提交:
1.复制要提交的工程到文件夹
2.git status
3.git add ****
4.git commit -m "提交描述"
5.git push