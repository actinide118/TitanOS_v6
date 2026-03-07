Pour le français rendez vous sur le fichier README.MD
# Dependencies
You need "gcc","nasm" and "qemu" 
# Build
After cloning the repository run the command "make build"
# Running 
Execute "make run" or "make all"
# Result
home screen:

<img width="730" height="463" alt="titanOS1" src="https://github.com/user-attachments/assets/f6a70591-013e-4589-a29c-80e45694a97d" />

<img width="730" height="463" alt="titanOS2" src="https://github.com/user-attachments/assets/3feff538-8fbd-46b9-b58a-03676aa9b60c" />

# Commands
### end
turn off the CPU but doesn't turn off the graphic card
### clear 
clear screen
### version
Show os version
### read
Show in hexadecimal a disk sector 
### tick
Print the number of CPU clock tick since it's initialization 
### cat 
Is supposed to print the text variant of a disk sector
### page
give a range of free adress ( it doesn't provide any security because CPU paging is not active )
### int 
Make the system crash ( expected result )
### 2Dtest
Show a char art "image" composed of two distinct elements
### hist 
Doesn't work
### fat_format
Useless ( I'm not even sure it works )
### touppercase
It's in the name
### tolowercase
It's in the name
### video
Switch the graphic card mode from text 16 color 3h to graphic 256 color linear 13h, and print the alphanumerics characters of the system in this mode
Note it's not possible to go back and if you want to go to the text mode you need to reboot the VM.