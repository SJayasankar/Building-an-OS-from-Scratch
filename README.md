# Building-an-OS-from-Scratch
Pre-requisites for the OS:
1. Download Oracle Virtualbox
2. Download virtual hard drive image file OS.VDI

Instructions on setting up the virtualbox image:
Step 1: Download VirtualBox for your computer
Go here:
https://www.virtualbox.org/wiki/Downloads

Step 2: Copy the virtual hard drive image OS.VDI to your
computer. This is a very large image, over 4GB! Give yourself
plenty of time and make sure you have the space. You can get it
from this link:
https://www.dropbox.com/sh/2swm5rj6y5q7qr0/AAC4ms7e4WQIVvG24
c9yD3w0a?dl=0

Step 3: Follow the instructions here to import the image into
virtualbox and make a new virtual machine:
https://blogs.oracle.com/oswald/entry/importing_a_vdi_in_virtualbox
In the options, select Linux as your operating system and Ubuntu 32
bit as your version.

Step 4:
Boot the virtual machine. The Ubuntu desktop should come up
within about one to five minutes, depending on the speed of your
computer.
On the desktop you should see a set of folders labeled Project A, B,
C, D, E, and F. You should also see a file simulator.jar, and at the
top of the screen you should see a button to open the terminal.

Step 5:
Double click on simulator.jar. Press Boot Floppy A: If you see a
screen come up after a minute or so with the message “It Works”,
then you're ready to start the project.

Overview of stages A-E in the Porject:
Project A - Booting
Project B - System Calls
Project C - Loading Files and Executing Programs
Project D - Writing Files and Improving Shell
Project E - Processes and Multitasking
