[root@OBU2 Code]# ./ObdTest /dev/ttyUSB2
open device /dev/ttyUSB2
send:atz

ObdRecvMsg:185 n = 4
atz
61 74 7a 0d 
ObdRecvMsg:185 n = 16
>LM327 v1.5
0d 0d 45 4c 4d 33 32 37 20 76 31 2e 35 0d 0d 3e 
send:atsp0

ObdRecvMsg:185 n = 11
>.5p0
61 74 73 70 30 0d 4f 4b 0d 0d 3e 
send:0100

ObdRecvMsg:185 n = 18
SEARCHING...
30 31 30 30 0d 53 45 41 52 43 48 49 4e 47 2e 2e 2e 0d 
ObdRecvMsg:185 n = 21
>1 00 98 1A 80 13 
34 31 20 30 30 20 39 38 20 31 41 20 38 30 20 31 33 20 0d 0d 3e 
obd init error,return.
1^H
cmd = 1
type is 1
send:0105

ObdRecvMsg:185 n = 5
0105
30 31 30 35 0d 
ObdRecvMsg:185 n = 12
>1 05 65 
34 31 20 30 35 20 36 35 20 0d 0d 3e 





2
cooler temperature is 61¡æ
cmd = 2
type is 2
send:010c

ObdRecvMsg:185 n = 5
010c
30 31 30 63 0d 
ObdRecvMsg:185 n = 15
>1 0C 0C 73 
34 31 20 30 43 20 30 43 20 37 33 20 0d 0d 3e 




3
engine rotation speed is 796 rpm
cmd = 3
type is 3
send:010a

ObdRecvMsg:185 n = 5
010a
30 31 30 61 0d 
ObdRecvMsg:185 n = 10
>O DATA
4e 4f 20 44 41 54 41 0d 0d 3e 
obd get date error.




4
cmd = 4
type is 4
send:010d

ObdRecvMsg:185 n = 5
010d
30 31 30 64 0d 
ObdRecvMsg:185 n = 12
>1 0D 00 
34 31 20 30 44 20 30 30 20 0d 0d 3e 
obd get date error.



5
cmd = 5
type is 5
send:010f

ObdRecvMsg:185 n = 5
010f
30 31 30 66 0d 
ObdRecvMsg:185 n = 12
>1 0F 56 
34 31 20 30 46 20 35 36 20 0d 0d 3e 
obd get date error.




6
cmd = 6
type is 6
send:011f

ObdRecvMsg:185 n = 5
011f
30 31 31 66 0d 
ObdRecvMsg:185 n = 15
>1 1F 04 F1 
34 31 20 31 46 20 30 34 20 46 31 20 0d 0d 3e 
obd get date error.
^C
[root@OBU2 Code]# 