On the Master Pi terminal and type:

ssh-keygen -o

This generated a public and private key under 

/home/pi/.ssh

Note: do NOT give your private key to anyone.

Then give the public key to any node in the cluser

ssh-copy-id USERNAME@IP_ADDRESS_OF_DEVICE