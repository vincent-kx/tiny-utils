#! /usr/bin/expect
set host [lindex $argv 0]
set password [exec /cfs/SNG/gpwd_user $host]
spawn ssh -l user_00 $host
expect { 
    "(yes/no)?" { 
        send "yes\n"
        expect "assword:"
        send "$password\n"
    } 
    "assword:" { 
    send "$password\n"
    } 
 } 
interact
