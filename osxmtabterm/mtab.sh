#!/bin/sh

#### provide your credentials here
#### one line - one tab

#: ssh_host user pass port



###### DO NOT EDIT #######

function open_tab {
    
    osascript -e "tell application \"Terminal\"" \
	-e "tell application \"System Events\" to keystroke \"t\" using {command down}" \
	-e "do script \"$0 spawn_expect $1\" in front window" \
	-e "end tell" > /dev/null
}



function normal_start {
    OIFS=${IFS}
    IFS="
"
    for server in `perl -ne 'print "$1\n" if m;^#: ([^\s]+)\s;' $0`;
    do
	echo "Opening: ${server}"
	open_tab ${server}
    done
    IFS=${OIFS}
}

function spawn_expect {
    server="$1"
    echo "THIS FILE $0"
    line=$(grep -m1 "#: ${server}" $0)
    user=$(echo -n "${line}" | perl -ne 'print "$1" if m;(?:\s([^\s]+)){3};g')
    password=$(echo -n "${line}" | perl -ne 'print "$1" if m;(?:\s([^\s]+)){4};g')
    port=$(echo -n "${line}" | perl -ne 'print "$1" if m;(?:\s([^\s]+)){5};g')
    test "x${port}" == "x" && port=22

    echo "$server $user $password $port"

    expect -c '
    proc writepass {password} {
	expect {
            "*connecting*"      {send yes\r\n; expect "*assword*" { send ${password}\n }}
            "*assword*"       {send ${password}\n }  }
    }
    trap {
	set rows [stty rows]
	set cols [stty columns]
	stty rows $rows columns $cols < $spawn_out(slave,name)
    } WINCH
    spawn ssh '${user}'@'${server}' -p '${port}'
    writepass '${password}'
    interact'
}

test "x$1" == "x" && normal_start
test "x$1" == "xspawn_expect" && spawn_expect $2
