cat > tasks.crontab <<EOF
10 19 * * * /bin/sh  /root/suto_git_commit.sh 
EOF
crontab ./tasks.crontab
service cron restart
rm -rf tasks.crontab