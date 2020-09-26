# 编写自动提交的脚本
```shell
在root目录下创建run.sh脚本
vim run.sh
# 文件内容如下
#!/bin/sh
cd /Users/BuleSky/Documents/code/repositoryname
git add .
git commit -m "备注"
git push
# 保存
```

# 设置crontab
```shell
crontab -e
# 文件内容如下
# 分 小时 天 月 星期 要执行的命令
*/1 * * * * bash /root/run.sh  # 每分钟执行脚本run.sh,注意路径从根目录开始
10 6 * * * bash /root/run.sh  # 每t天6点10分执行脚本run.sh
```
# 重启crontab服务
```
service cron restart
```