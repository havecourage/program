#!/bin/sh
#ssh连接好难配置，还是使用http连接吧
#向脚本之中加入参数，参数为提交之时所要添加的信息
git add ./* ./*/*
git commit -m $1
git remote add origin https://github.com/havecourage/-.git
git push -u origin master