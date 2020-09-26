#!/bin/sh
#ssh连接好难配置，还是使用http连接吧
#向脚本之中加入参数，参数为提交之时所要添加的信息
#git init
git add . #./* ./*/*
git commit -m "$1"
##这里本来是想要运行脚本的时候将信息作为输入来运行的，但在初始版本出现了问题：
#初始版本为：git commit -m $1
#输入sh git_commit.sh "2020.09.18 Second commit"之后出现如下报错:
##root@DESKTOP-D6FCGOJ:/mnt/e/Rodalph/program# sh git_commit.sh "2020.09.18 Second commit"
##error: pathspec 'Second' did not match any file(s) known to git
##error: pathspec 'commit' did not match any file(s) known to git
#shell脚本传入的字符串数组被git以空格为分隔符隔开了




#git remote add origin https://github.com/havecourage/-.git
git push #-u origin master

#    git config --global user.name "Your Name"
#    git config --global user.email you@example.com

#    After doing this, you may fix the identity used for this commit with:

#    git commit --amend --reset-author
