#!/usr/bin/expect -f
set timeout 30
spawn scp cc_dbg root@192.168.2.38:/tmp/
expect "password:"
send "admin\r"
expect eof
exit
