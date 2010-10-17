des
===
## Description
Yet another C implementation of DES.

## Usage
Example of usage :

$ ./des 
usage: des [-de] [-i infile] [-o outfile] [-k=key]
-e       encrypt infile to outfile using key
-d       decrypt infile to outfile using key
$ ./des -e -i /bin/ls -o /tmp/ls_encrypted -k "hello!"
$ ./des -d -i /bin/ls_encrypted -o /tmp/ls -k "hello!"
$ diff /tmp/ls /bin/ls
$
