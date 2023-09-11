#!/bin/bash

#set -ex

echo "...Checking zkstake.conf"

if [ ! -e "$HOME/.zkstake/zkstake.conf" ]; then
    mkdir -p $HOME/.zkstake

    echo "...Creating zkstake.conf"
    cat <<EOF > $HOME/.zkstake/zkstake.conf
rpcuser=${rpcuser:-zkstakerpc}
rpcpassword=${rpcpassword:-`dd if=/dev/urandom bs=33 count=1 2>/dev/null | base64`}
txindex=1
bind=${listenip:-127.0.0.1}
rpcbind=${listenip:-127.0.0.1}
EOF

    cat $HOME/.zkstake/zkstake.conf
fi

echo "...Checking fetch-params"
$HOME/zcutil/fetch-params.sh
echo "Initialization completed successfully"
echo

# ToDo: Needs some rework. I was sick
if [ $# -gt 0 ]; then

    args=("$@")

elif [  -z ${assetchain+x} ]; then

    args=("-gen -genproclimit=${genproclimit:-2} -pubkey=${pubkey}")

else

    args=("-pubkey=${pubkey} -ac_name=${assetchain} -addnode=${seednode}")

fi

echo
echo "****************************************************"
echo "Running: zkstaked ${args[@]}"
echo "****************************************************"

exec zkstaked ${args[@]}
